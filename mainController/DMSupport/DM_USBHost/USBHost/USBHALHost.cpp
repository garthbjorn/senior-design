/* mbed USBHost Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "USBHALHost.h"
#include "dbg.h"

// bits of the USB/OTG clock control register
#define HOST_CLK_EN     (1<<0)
#define DEV_CLK_EN      (1<<1)
#define PORTSEL_CLK_EN  (1<<3)
#define AHB_CLK_EN      (1<<4)

// bits of the USB/OTG clock status register
#define HOST_CLK_ON     (1<<0)
#define DEV_CLK_ON      (1<<1)
#define PORTSEL_CLK_ON  (1<<3)
#define AHB_CLK_ON      (1<<4)

// we need host clock, OTG/portsel clock and AHB clock
#define CLOCK_MASK (HOST_CLK_EN | PORTSEL_CLK_EN | AHB_CLK_EN)

#define HCCA_SIZE sizeof(HCCA)
#define ED_SIZE sizeof(HCED)
#define TD_SIZE sizeof(HCTD)

#define TOTAL_SIZE (HCCA_SIZE + (MAX_ENDPOINT*ED_SIZE) + (MAX_TD*TD_SIZE))

// Put the USB structures in the only memory accessible by the USB stack - the AHBSRAM0
static volatile uint8_t usb_buf[TOTAL_SIZE] __attribute__((section("AHBSRAM0"),aligned(256)));// __attribute__((section("AHBSRAM0"),aligned(256)));  //256 bytes aligned!

// A very basic implementation of malloc to allocate in the AHBSRAM0
#define SAFE_MEM_TO_USE     (10*1024)
#define SAFE_MEM_BLOCK_SIZE (512)
#define SAFE_MEM_NUM_BLOCKS (SAFE_MEM_TO_USE/SAFE_MEM_BLOCK_SIZE)
typedef struct {
    volatile uint8_t* ptr;
    bool used;
} safe_mem_info_t;
static safe_mem_info_t safe_mem_list[SAFE_MEM_NUM_BLOCKS];
static uint8_t safe_mem_data[SAFE_MEM_TO_USE] __attribute__((section("AHBSRAM0"),aligned(256)));//__attribute__((section("AHBSRAM0"),aligned));  //256 bytes aligned!

// To detect when memory outside of the AHBSRAM0 is passed to the USB stack
void assert_mem_region(uint32_t ptr)
{
    if (( ptr != 0) && ((ptr & 0xff000000) != 0x20000000)) {
        USB_ERR("0x%08x not in USB MEM", ptr);
        mbed_die();
    }
}

USBHALHost * USBHALHost::instHost;

USBHALHost::USBHALHost() {
    instHost = this;
    memInit();
    memset((void*)usb_hcca, 0, HCCA_SIZE);
    for (int i = 0; i < MAX_ENDPOINT; i++) {
        edBufAlloc[i] = false;
    }
    for (int i = 0; i < MAX_TD; i++) {
        tdBufAlloc[i] = false;
    }
    for (int i = 0; i < SAFE_MEM_NUM_BLOCKS; i++) {
        safe_mem_list[i].used = false;
        safe_mem_list[i].ptr = safe_mem_data + SAFE_MEM_BLOCK_SIZE*i;
    }
}

uint8_t* USBHALHost::getSafeMem(uint32_t size) {
    uint8_t* result = NULL;
    if (size > 512) {
        USB_ERR("getSafeMem(%u) not supported", size);
    } else {
        safemem_mutex.lock();
        for (int i = 0; i < SAFE_MEM_NUM_BLOCKS; i++) {
            if (!safe_mem_list[i].used) {
                safe_mem_list[i].used = true;
                result = (uint8_t*)safe_mem_list[i].ptr;
                break;
            }
        }
        safemem_mutex.unlock();
    }
    if (result == NULL) {
        USB_ERR("getSafeMem(%u) failed to allocate", size);
    }
    return result;
}

void USBHALHost::returnSafeMem(uint8_t* mem) {
    safemem_mutex.lock();
    int i;
    for (i = 0; i < SAFE_MEM_NUM_BLOCKS; i++) {
        if (safe_mem_list[i].ptr == mem) {
            safe_mem_list[i].used = false;
            break;
        }
    }
    safemem_mutex.unlock();
    if (i == SAFE_MEM_NUM_BLOCKS) {
      USB_ERR("returnSafeMem(%p) not allocated", mem);
    }
}


void USBHALHost::init() {
    NVIC_DisableIRQ(USB_IRQn);

    //Cut power
    LPC_SC->PCONP &= ~(1UL<<31);
    Thread::wait(1000);

    // turn on power for USB
    LPC_SC->PCONP       |= (1UL<<31);

    // Enable USB host clock, port selection and AHB clock
    LPC_USB->USBClkCtrl = 0x19;

    // Wait for clocks to become available
    while ((LPC_USB->USBClkSt & 0x19) != 0x19);

    // it seems the bits[0:1] mean the following
    // 0: U1=device, U2=host
    // 1: U1=host, U2=host
    // 2: reserved
    // 3: U1=host, U2=device
    // NB: this register is only available if OTG clock (aka "port select") is enabled!!
    LPC_USB->OTGStCtrl = 1;

    // configure USB D+/D- pins

    LPC_IOCON->P0_29 &= ~0x9F; // USB_D+1
    LPC_IOCON->P0_29 |=  0x01; // USB_D+1
    LPC_IOCON->P0_30 &= ~0x9F; // USB_D-1
    LPC_IOCON->P0_30 |=  0x01; // USB_D-1

    LPC_IOCON->P0_31 &= ~0x9F; // USB_D+2
    LPC_IOCON->P0_31 |=  0x01; // USB_D+2

    LPC_IOCON->P1_30 &= ~0x1F; // USB_PWRD2
    LPC_IOCON->P1_30 |=  0x01; // USB_PWRD2
    LPC_IOCON->P1_31 &= ~0x1F; // USB_OVRCR2
    LPC_IOCON->P1_31 |=  0x01; // USB_OVRCR2

    LPC_IOCON->P0_14 &= ~0x1F; // USB ID Pin as GPIO, output, high
    LPC_GPIO0->DIR   |= (1<<14);
    LPC_GPIO0->SET    = (1<<14);
    LPC_IOCON->P4_31 &= ~0x1F; // USB CONN Pin as GPIO, output, low
    LPC_GPIO4->DIR   |= (1UL<<31);
    LPC_GPIO4->CLR    = (1UL<<31);

    USB_DBG("initialize OHCI\n");

    // Wait 100 ms before apply reset
    Thread::wait(100);

    LPC_USB->HcControl       = 0; // HARDWARE RESET
    LPC_USB->HcControlHeadED = 0; // Initialize Control list head to Zero
    LPC_USB->HcBulkHeadED    = 0; // Initialize Bulk list head to Zero

    // software reset
    LPC_USB->HcCommandStatus = OR_CMD_STATUS_HCR;

    // Write Fm Interval and Largest Data Packet Counter
    LPC_USB->HcFmInterval    = DEFAULT_FMINTERVAL;
    LPC_USB->HcPeriodicStart = FI * 90 / 100;

    // Put HC in operational state
    LPC_USB->HcControl  = (LPC_USB->HcControl & (~OR_CONTROL_HCFS)) | OR_CONTROL_HC_OPER;

    // Set Global Power
    LPC_USB->HcRhStatus = OR_RH_STATUS_LPSC;

    LPC_USB->HcHCCA = (uint32_t)(usb_hcca);

    // Clear Interrrupt Status
    LPC_USB->HcInterruptStatus |= LPC_USB->HcInterruptStatus;

    LPC_USB->HcInterruptEnable  = OR_INTR_ENABLE_MIE | OR_INTR_ENABLE_WDH | OR_INTR_ENABLE_RHSC;

    // Enable the USB Interrupt
    NVIC_SetVector(USB_IRQn, (uint32_t)(_usbisr));
    LPC_USB->HcRhPortStatus1 = OR_RH_PORT_CSC;
    LPC_USB->HcRhPortStatus1 = OR_RH_PORT_PRSC;
    LPC_USB->HcRhPortStatus2 = OR_RH_PORT_CSC;
    LPC_USB->HcRhPortStatus2 = OR_RH_PORT_PRSC;
    
    resetRootHub();

    NVIC_SetPriority(USB_IRQn, 0);
    NVIC_EnableIRQ(USB_IRQn);

    if (LPC_USB->HcRhPortStatus2 & OR_RH_PORT_CCS) {
        //Device connected
        Thread::wait(150);
        USB_DBG("Device connected (%08x)\n\r", LPC_USB->HcRhPortStatus2);
        deviceConnected(0, 2, LPC_USB->HcRhPortStatus2 & OR_RH_PORT_LSDA);
    }
}

uint32_t USBHALHost::controlHeadED() {
    return LPC_USB->HcControlHeadED;
}

uint32_t USBHALHost::bulkHeadED() {
    return LPC_USB->HcBulkHeadED;
}

uint32_t USBHALHost::interruptHeadED() {
    return usb_hcca->IntTable[0];
}

void USBHALHost::updateBulkHeadED(uint32_t addr) {
    LPC_USB->HcBulkHeadED = addr;
}


void USBHALHost::updateControlHeadED(uint32_t addr) {
    LPC_USB->HcControlHeadED = addr;
}

void USBHALHost::updateInterruptHeadED(uint32_t addr) {
    usb_hcca->IntTable[0] = addr;
}


void USBHALHost::enableList(ENDPOINT_TYPE type) {
    switch(type) {
        case CONTROL_ENDPOINT:
            LPC_USB->HcCommandStatus = OR_CMD_STATUS_CLF;
            LPC_USB->HcControl |= OR_CONTROL_CLE;
            break;
        case ISOCHRONOUS_ENDPOINT:
            break;
        case BULK_ENDPOINT:
            LPC_USB->HcCommandStatus = OR_CMD_STATUS_BLF;
            LPC_USB->HcControl |= OR_CONTROL_BLE;
            break;
        case INTERRUPT_ENDPOINT:
            LPC_USB->HcControl |= OR_CONTROL_PLE;
            break;
    }
}


bool USBHALHost::disableList(ENDPOINT_TYPE type) {
    switch(type) {
        case CONTROL_ENDPOINT:
            if(LPC_USB->HcControl & OR_CONTROL_CLE) {
                LPC_USB->HcControl &= ~OR_CONTROL_CLE;
                return true;
            }
            return false;
        case ISOCHRONOUS_ENDPOINT:
            return false;
        case BULK_ENDPOINT:
            if(LPC_USB->HcControl & OR_CONTROL_BLE){
                LPC_USB->HcControl &= ~OR_CONTROL_BLE;
                return true;
            }
            return false;
        case INTERRUPT_ENDPOINT:
            if(LPC_USB->HcControl & OR_CONTROL_PLE) {
                LPC_USB->HcControl &= ~OR_CONTROL_PLE;
                return true;
            }
            return false;
    }
    return false;
}


void USBHALHost::memInit() {
    usb_hcca = (volatile HCCA *)usb_buf;
    usb_edBuf = usb_buf + HCCA_SIZE;
    usb_tdBuf = usb_buf + HCCA_SIZE + (MAX_ENDPOINT*ED_SIZE);
}

volatile uint8_t * USBHALHost::getED() {
    for (int i = 0; i < MAX_ENDPOINT; i++) {
        if ( !edBufAlloc[i] ) {
            edBufAlloc[i] = true;
            return (volatile uint8_t *)(usb_edBuf + i*ED_SIZE);
        }
    }
    USB_ERR("Could not allocate ED\r\n");
    return NULL;
}

volatile uint8_t * USBHALHost::getTD() {
    int i;
    for (i = 0; i < MAX_TD; i++) {
        if ( !tdBufAlloc[i] ) {
            tdBufAlloc[i] = true;
            return (volatile uint8_t *)(usb_tdBuf + i*TD_SIZE);
        }
    }
    USB_ERR("Could not allocate TD\r\n");
    return NULL;
}


void USBHALHost::freeED(volatile uint8_t * ed) {
    int i;
    i = (ed - usb_edBuf) / ED_SIZE;
    edBufAlloc[i] = false;
}

void USBHALHost::freeTD(volatile uint8_t * td) {
    int i;
    i = (td - usb_tdBuf) / TD_SIZE;
    tdBufAlloc[i] = false;
}


void USBHALHost::resetRootHub() {

    DigitalOut usb2_vbus_en(P0_12);
    usb2_vbus_en = 1;
    Thread::wait(100); /* USB 2.0 spec says at least 50ms delay before port reset */

    // Initiate port reset
    LPC_USB->HcRhPortStatus1 = OR_RH_PORT_PRS;

    while (LPC_USB->HcRhPortStatus1 & OR_RH_PORT_PRS);

    // ...and clear port reset signal
    LPC_USB->HcRhPortStatus1 = OR_RH_PORT_PRSC;

    usb2_vbus_en = 0;

    // Initiate port reset
    LPC_USB->HcRhPortStatus2 = OR_RH_PORT_PRS;

    while (LPC_USB->HcRhPortStatus2 & OR_RH_PORT_PRS);
    
    // ...and clear port reset signal
    LPC_USB->HcRhPortStatus2 = OR_RH_PORT_PRSC;
    
    usb2_vbus_en = 1;
    Thread::wait(200); /* Wait for at least 100 MS after port reset */
}


void USBHALHost::_usbisr(void) {
    if (instHost) {
        instHost->UsbIrqhandler();
    }
}

void USBHALHost::UsbIrqhandler() {
    if( LPC_USB->HcInterruptStatus & LPC_USB->HcInterruptEnable ) //Is there something to actually process?
    {
        uint32_t int_status = LPC_USB->HcInterruptStatus & LPC_USB->HcInterruptEnable;
        
        // Root hub status change interrupt
        if (int_status & OR_INTR_STATUS_RHSC) {
            if (LPC_USB->HcRhPortStatus2 & OR_RH_PORT_CSC) {
                if (LPC_USB->HcRhStatus & OR_RH_STATUS_DRWE) {
                    // When DRWE is on, Connect Status Change
                    // means a remote wakeup event.
                } else {

                    //Root device connected
                    if (LPC_USB->HcRhPortStatus2 & OR_RH_PORT_CCS) {

                        // wait 150ms to avoid bounce
                        wait_ms(150);

                        //Hub 0 (root hub), Port 2 (count starts at 1), Low or High speed
                        deviceConnected(0, 2, LPC_USB->HcRhPortStatus2 & OR_RH_PORT_LSDA);
                    }

                    //Root device disconnected
                    else {

                        if (!(int_status & OR_INTR_STATUS_WDH)) {
                            usb_hcca->DoneHead = 0;
                        }

                        // wait 200ms to avoid bounce
                        wait_ms(200);

                        deviceDisconnected(0, 2, NULL, usb_hcca->DoneHead & 0xFFFFFFFE);

                        if (int_status & OR_INTR_STATUS_WDH) {
                            usb_hcca->DoneHead = 0;
                            LPC_USB->HcInterruptStatus = OR_INTR_STATUS_WDH;
                        }
                    }
                }
                LPC_USB->HcRhPortStatus2 = OR_RH_PORT_CSC;
            }
            if (LPC_USB->HcRhPortStatus2 & OR_RH_PORT_PRSC) {
                LPC_USB->HcRhPortStatus2 = OR_RH_PORT_PRSC;
            }
            LPC_USB->HcInterruptStatus = OR_INTR_STATUS_RHSC;
        }

        // Writeback Done Head interrupt
        if (int_status & OR_INTR_STATUS_WDH) {
            transferCompleted(usb_hcca->DoneHead & 0xFFFFFFFE);
            LPC_USB->HcInterruptStatus = OR_INTR_STATUS_WDH;
        }
    }
}
