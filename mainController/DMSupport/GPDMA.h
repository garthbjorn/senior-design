/*
 *  Copyright 2015 Embedded Artists AB
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef GPDMA_H
#define GPDMA_H

#include "mbed.h"
#include "rtos.h"

/**
 * @brief GPDMA request connections
 */
#define GPDMA_CONN_MEMORY           ((0UL))
#define GPDMA_CONN_SDC              ((1UL))      /*!< SD card */

/**
 * @brief Macro defines for DMA channel control registers
 */
#define GPDMA_DMACCxControl_TransferSize(n) (((n & 0xFFF) << 0))  /*!< Transfer size*/
#define GPDMA_DMACCxControl_SBSize(n)       (((n & 0x07) << 12))  /*!< Source burst size*/
#define GPDMA_DMACCxControl_DBSize(n)       (((n & 0x07) << 15))  /*!< Destination burst size*/
#define GPDMA_DMACCxControl_SWidth(n)       (((n & 0x07) << 18))  /*!< Source transfer width*/
#define GPDMA_DMACCxControl_DWidth(n)       (((n & 0x07) << 21))  /*!< Destination transfer width*/
#define GPDMA_DMACCxControl_SI              ((1UL << 26))      /*!< Source increment*/
#define GPDMA_DMACCxControl_DI              ((1UL << 27))      /*!< Destination increment*/
#define GPDMA_DMACCxControl_SrcTransUseAHBMaster1   0
#define GPDMA_DMACCxControl_DestTransUseAHBMaster1  0
#define GPDMA_DMACCxControl_Prot1           ((1UL << 28))      /*!< Indicates that the access is in user mode or privileged mode*/
#define GPDMA_DMACCxControl_Prot2           ((1UL << 29))      /*!< Indicates that the access is bufferable or not bufferable*/
#define GPDMA_DMACCxControl_Prot3           ((1UL << 30))      /*!< Indicates that the access is cacheable or not cacheable*/
#define GPDMA_DMACCxControl_I               ((1UL << 31))      /*!< Terminal count interrupt enable bit */

/**
 * @brief GPDMA Burst size in Source and Destination definitions
 */
#define GPDMA_BSIZE_1   ((0UL))  /*!< Burst size = 1 */
#define GPDMA_BSIZE_4   ((1UL))  /*!< Burst size = 4 */
#define GPDMA_BSIZE_8   ((2UL))  /*!< Burst size = 8 */
#define GPDMA_BSIZE_16  ((3UL))  /*!< Burst size = 16 */
#define GPDMA_BSIZE_32  ((4UL))  /*!< Burst size = 32 */
#define GPDMA_BSIZE_64  ((5UL))  /*!< Burst size = 64 */
#define GPDMA_BSIZE_128 ((6UL))  /*!< Burst size = 128 */
#define GPDMA_BSIZE_256 ((7UL))  /*!< Burst size = 256 */

/**
 * @brief Width in Source transfer width and Destination transfer width definitions
 */
#define GPDMA_WIDTH_BYTE        ((0UL))  /*!< Width = 1 byte */
#define GPDMA_WIDTH_HALFWORD    ((1UL))  /*!< Width = 2 bytes */
#define GPDMA_WIDTH_WORD        ((2UL))  /*!< Width = 4 bytes */

/**
 * @brief Macro defines for DMA Configuration register
 */
#define GPDMA_DMACConfig_E              ((0x01))  /*!< DMA Controller enable*/
#define GPDMA_DMACConfig_M              ((0x02))  /*!< AHB Master endianness configuration*/
#define GPDMA_DMACConfig_BITMASK        ((0x03))

/**
 * @brief Macro defines for DMA Channel Configuration registers
 */
#define GPDMA_DMACCxConfig_E                    ((1UL << 0))      /*!< DMA control enable*/
#define GPDMA_DMACCxConfig_SrcPeripheral(n)     (((n & 0x1F) << 1))    /*!< Source peripheral*/
#define GPDMA_DMACCxConfig_DestPeripheral(n)    (((n & 0x1F) << 6))    /*!< Destination peripheral*/
#define GPDMA_DMACCxConfig_TransferType(n)      (((n & 0x7) << 11))    /*!< This value indicates the type of transfer*/
#define GPDMA_DMACCxConfig_IE                   ((1UL << 14))      /*!< Interrupt error mask*/
#define GPDMA_DMACCxConfig_ITC                  ((1UL << 15))      /*!< Terminal count interrupt mask*/
#define GPDMA_DMACCxConfig_L                    ((1UL << 16))      /*!< Lock*/
#define GPDMA_DMACCxConfig_A                    ((1UL << 17))      /*!< Active*/
#define GPDMA_DMACCxConfig_H                    ((1UL << 18))      /*!< Halt*/

/**
 * @brief GPDMA Linker List Item structure type definition
 */
typedef struct {
    uint32_t SrcAddr;    /**< Source Address */
    uint32_t DstAddr;    /**< Destination address */
    uint32_t NextLLI;    /**< Next LLI address, otherwise set to '0' */
    uint32_t Control;    /**< GPDMA Control of this LLI */
} GPDMA_LLI_Type;

/**
 * Wrapper for the General Purpose DMA controller.
 *
 * The GPDMA class allows multiple users of DMA to claim one or more
 * of the eight available DMA channels and to register handlers for the
 * interrupts. This centralized control is needed as there are eight
 * channels but only one DMA IRQ.
 *
 * By having to request a channel to use it is possible to avoid having
 * hardcoded DMA channel number in the code
 *
 * An example of use:
 *
 * @code
 * void handler() {
 *   _eventSuccess = LPC_GPDMA->IntTCStat & (1<<ch);
 *   LPC_GPDMA->IntTCClear = (1<<ch);
 * }
 *
 * void foo() {
 *    GPDMA* dma = &GPDMA::instance();
 *    dma->init();
 *
 *    DMAChannels ch = dma->acquireChannel(handler)
 *    if (ch == DMACh_Unavailable) {
 *      // handle error
 *    }
 *
 *    // prepare the channel data (see e.g. gpdma_transfer_from_mci() in MCIFileSystem)
 *    ...
 *
 *    // start the transfer
 *    dma->transfer(&cfg, ctrl_word, 0, GPDMA_CONN_SDC, GPDMA_CONN_MEMORY);
 *
 *    // wait for transfer to complete (e.g. by handler() setting a flag)
 *    ...
 *
 *    // return the channel now that it is no longer needed
 *    dma->releaseChannel(ch);
 * }
 * @endcode
 */
class GPDMA {
public:
    enum DMAChannels {
        DMACh0,   /* DMA channel 0 has the highest priority */
        DMACh1,
        DMACh2,
        DMACh3,
        DMACh4,
        DMACh5,
        DMACh6,
        DMACh7,   /* DMA channel 7 has the lowest priority */
        NumDMAChannels,
        DMACh_Unavailable = NumDMAChannels,
    };
    enum FlowControl {
        FlowControl_M2M_Ctrl_DMA,         /* Memory to memory - DMA control */
        FlowControl_M2P_Ctrl_DMA,         /* Memory to peripheral - DMA control */
        FlowControl_P2M_Ctrl_DMA,         /* Peripheral to memory - DMA control */
        FlowControl_P2P_Ctrl_DMA,         /* Source peripheral to destination peripheral - DMA control */
        FlowControl_P2P_Ctrl_DestPeriph,  /* Source peripheral to destination peripheral - destination peripheral control */
        FlowControl_M2P_Ctrl_Periph,      /* Memory to peripheral - peripheral control */
        FlowControl_P2M_Ctrl_Periph,      /* Peripheral to memory - peripheral control */
        FlowControl_P2P_Ctrl_SrcPeriph,   /* Source peripheral to destination peripheral - source peripheral control */
    };
    typedef struct {
        DMAChannels ChannelNum;
        uint32_t    TransferSize;  /* Length/Size of transfer */
        uint32_t    TransferWidth; /* Transfer width - used only for M2M */
        uint32_t    SrcAddr;       /* Physical Source Address, only for M2M and M2P */
        uint32_t    DstAddr;       /* Physical Destination Address, only for M2M and P2M */
        FlowControl TransferType;
    } GPDMA_Channel_CFG_T;

    /** Get the only instance of the GPDMA
     *
     *  @returns The Registry
     */
    static GPDMA& instance()
    {
        static GPDMA singleton;
        return singleton;
    }

    /** Initializes the GPDMA
     *
     *  Safe to call even with multiple DMA users
     */
    void init();

    /** Shuts down the GPDMA
     *
     *  Call will be ignored if there are any acquired channels
     */
    void deinit();

    /** Get a free GPDMA channel for one DMA connection
     *
     *  The function will start to look for the suggested channel.
     *  If it is in use then the next, higher, channel number will
     *  be tested. The default is set to use DMACh3 which has
     *  medium priority.
     *
     *  To acquire a channel with the highest available priority
     *  call this function with DMACh0 as parameter.
     *
     *  @param func       the interrupt callback function
     *  @param suggested  the first channel to look for
     *
     *  @returns
     *       DMACh0 to DMACh7 on success
     *       DMACh_Unavailable if there are no channels available
     */
    DMAChannels acquireChannel(void (*irqfunc)(void), DMAChannels suggested = DMACh3);

    /** Release a previously acquired channel
     *
     *  @param ch  the DMA channel to release
     */
    void releaseChannel(DMAChannels ch);

    /** Returns the raw registers for the GPDMA channel for direct manipulation
     *
     *  @param ch    the DMA channel
     *
     *  @returns
     *       The registers on success
     *       NULL if the channel is invalid
     */
    LPC_GPDMACH_TypeDef* getDirectRegisters(DMAChannels ch);

    // Internal use only. Called by the IRQ handler
    void processInterrupt();

    /** Does the actual DMA transfer
     *
     *  @param cfg           the DMA configuration
     *  @param CtrlWord      data for the DMA channel's CONTROL register
     *  @param LinkListItem  address to the linked list or 0 if not used
     *  @param SrcPeripheral source of the transfer (GPDMA_CONN_MEMORY or GPDMA_CONN_SDC)
     *  @param DstPeripheral destination of the transfer (GPDMA_CONN_MEMORY or GPDMA_CONN_SDC)
     *
     *  @returns
     *       True on success, false on failure
     */
    bool transfer(GPDMA_Channel_CFG_T* cfg,
                  uint32_t CtrlWord,
                  uint32_t LinkListItem,
                  uint8_t SrcPeripheral,
                  uint8_t DstPeripheral);

    void stopTransfer(DMAChannels ch);

private:
    typedef struct {
        bool inUse;
        void (*func)(void);
    } channelData_t;

    Mutex _mutex;
    channelData_t _chData[NumDMAChannels];
    int _usedChannels;
    bool _initialized;

    explicit GPDMA();
    // hide copy constructor
    GPDMA(const GPDMA&);
    // hide assign operator
    GPDMA& operator=(const GPDMA&);
    ~GPDMA();
};

#endif
