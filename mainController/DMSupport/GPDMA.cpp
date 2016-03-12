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

#include "mbed.h"
#include "DMBoard.h"
#include "GPDMA.h"


/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

#define GPDMACH(__x) ((LPC_GPDMACH_TypeDef*)(LPC_GPDMACH0_BASE + (0x20 * (__x))))

#define CH_MASK(__ch)  (((1UL << (__ch)) & 0xFF))

#define GPDMA_ALL_CH_MASK  (0xff)

/******************************************************************************
 * Local variables
 *****************************************************************************/

/******************************************************************************
 * Private Class
 *****************************************************************************/

/******************************************************************************
 * DMA Interrupt Functon
 *****************************************************************************/

static void mydmairq()
{
  GPDMA::instance().processInterrupt();
}

/******************************************************************************
 * Private Functions
 *****************************************************************************/

/******************************************************************************
 * Public Functions
 *****************************************************************************/

GPDMA::GPDMA() : _usedChannels(0), _initialized(false)
{
  for (int i = 0; i < NumDMAChannels; i++) {
    _chData[i].inUse = false;
    _chData[i].func = NULL;
  }

  /* Redirect all DMA IRQs to the local mydmairq() function */
  NVIC_SetVector(DMA_IRQn, (uint32_t) mydmairq);
}

GPDMA::~GPDMA()
{
}

void GPDMA::init()
{
  _mutex.lock();
  if (!_initialized) {
    /* Enable GPDMA master clock */
    LPC_SC->PCONP |= (1<<29);

    /* Reset all channel configuration register */
    for (int i = 0; i < NumDMAChannels; i++) {
      GPDMACH(i)->CConfig = 0;
    }

    /* Clear all DMA interrupt and error flag */
    LPC_GPDMA->IntTCClear = 0xFF;
    LPC_GPDMA->IntErrClr  = 0xFF;

    /* Reset all channel datas */
    for (int i = 0; i < NumDMAChannels; i++) {
      _chData[i].inUse = false;
      _chData[i].func = NULL;
    }

    _initialized = true;
  }
  _mutex.unlock();
}

void GPDMA::deinit()
{
  _mutex.lock();
  if (_usedChannels == 0) {
    /* Disable the real interrupt */
    NVIC_DisableIRQ(DMA_IRQn);

    /* Clear all DMA interrupt and error flag */
    LPC_GPDMA->IntTCClear = 0xFF;
    LPC_GPDMA->IntErrClr  = 0xFF;

    /* Disable GPDMA master clock */
    LPC_SC->PCONP &= ~(1<<29);

    _initialized = false;
  }
  _mutex.unlock();
}

GPDMA::DMAChannels GPDMA::acquireChannel(void (*irqfunc)(void), DMAChannels suggested)
{
  DMAChannels result = DMACh_Unavailable;
  _mutex.lock();
  if (!_initialized) {
    init();
  }
  for (int i = suggested; i < NumDMAChannels; i++) {
    if ((!_chData[i].inUse) && ((LPC_GPDMA->EnbldChns & CH_MASK(i)) == 0)) {
      _chData[i].inUse = true;
      _chData[i].func = irqfunc;
      result = (DMAChannels)i;
      _usedChannels++;
      break;
    }
  }

  _mutex.unlock();
  return result;
}

void GPDMA::releaseChannel(DMAChannels ch)
{
  _mutex.lock();
  if (ch < NumDMAChannels) {
    stopTransfer(ch);
    if (_chData[ch].inUse) {
      _usedChannels--;
    }
    _chData[ch].inUse = false;
    _chData[ch].func = NULL;
  }
  _mutex.unlock();
}

LPC_GPDMACH_TypeDef* GPDMA::getDirectRegisters(DMAChannels ch)
{
  if (ch < NumDMAChannels) {
    return GPDMACH(ch);
  } else {
    return NULL;
  }
}

void GPDMA::processInterrupt()
{
  /* check status of DMA channel interrupts */
  uint32_t intstat = LPC_GPDMA->IntStat;

  for (int i = 0; i < NumDMAChannels; i++) {
    uint32_t mask = CH_MASK(i);
    if (intstat & mask) {
      if (_chData[i].func != NULL) {
        /* Notify registered subscriber */
        _chData[i].func();//GPDMACH(i), (tcstat&mask)!=0, (errstat&mask)!=0);
      }
    }
  }

  if ((LPC_GPDMA->EnbldChns & GPDMA_ALL_CH_MASK) == 0) {
    /* No more enabled DMA channels, so turn of DMA IRQ */
    //NVIC_DisableIRQ(DMA_IRQn);
  }
}

bool GPDMA::transfer(GPDMA_Channel_CFG_T* cfg, uint32_t CtrlWord,
                     uint32_t LinkListItem, uint8_t SrcPeripheral,
                     uint8_t DstPeripheral)
{
  if (LPC_GPDMA->EnbldChns & CH_MASK(cfg->ChannelNum)) {
    /* This channel is enabled, return ERROR, need to release this channel first */
    return false;
  }

  /* Get Channel pointer */
  LPC_GPDMACH_TypeDef* pCh = GPDMACH(cfg->ChannelNum);

  /* Reset the Interrupt status */
  LPC_GPDMA->IntTCClear = CH_MASK(cfg->ChannelNum);
  LPC_GPDMA->IntErrClr = CH_MASK(cfg->ChannelNum);

  /* Clear configuration */
  pCh->CControl = 0x00;
  pCh->CConfig = 0x00;

  /* Assign Linker List Item value */
  pCh->CLLI = LinkListItem;

  pCh->CSrcAddr = cfg->SrcAddr;
  pCh->CDestAddr = cfg->DstAddr;
  pCh->CControl = CtrlWord;

  /* Enable DMA channels, little endian */
  LPC_GPDMA->Config = GPDMA_DMACConfig_E;
  while (!(LPC_GPDMA->Config & GPDMA_DMACConfig_E)) {}

  /* Configure DMA Channel, enable Error Counter and Terminate counter */
  pCh->CConfig = GPDMA_DMACCxConfig_IE
               | GPDMA_DMACCxConfig_ITC
               | GPDMA_DMACCxConfig_TransferType((uint32_t) cfg->TransferType)
               | GPDMA_DMACCxConfig_SrcPeripheral(SrcPeripheral)
               | GPDMA_DMACCxConfig_DestPeripheral(DstPeripheral);

  /* Enable the interrupts */
  NVIC_EnableIRQ(DMA_IRQn);

  /* Start the Channel */
  pCh->CConfig |= GPDMA_DMACCxConfig_E;

  return true;
}

void GPDMA::stopTransfer(DMAChannels ch)
{
  /* Disable channel */
  GPDMACH(ch)->CConfig &= ~GPDMA_DMACCxConfig_E;

  /* Clear pending interrupts */
  LPC_GPDMA->IntTCClear = CH_MASK(ch);
  LPC_GPDMA->IntErrClr = CH_MASK(ch);
}
