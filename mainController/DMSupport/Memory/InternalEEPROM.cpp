/*
 *  Copyright 2014 Embedded Artists AB
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
#include "InternalEEPROM.h"


/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

#define EEPROM_INT_ENDOFRW    (1<<26)
#define EEPROM_INT_ENDOFPROG  (1<<28)

#define EEPROM_PWRDWN          (1<<0)

/*
 * Commands (for the LPC_EEPROM->CMD register)
 */
#define EEPROM_CMD_8BITS_READ           (0)     /*!< EEPROM 8-bit read command */
#define EEPROM_CMD_16BITS_READ          (1)     /*!< EEPROM 16-bit read command */
#define EEPROM_CMD_32BITS_READ          (2)     /*!< EEPROM 32-bit read command */
#define EEPROM_CMD_8BITS_WRITE          (3)     /*!< EEPROM 8-bit write command */
#define EEPROM_CMD_16BITS_WRITE         (4)     /*!< EEPROM 16-bit write command */
#define EEPROM_CMD_32BITS_WRITE         (5)     /*!< EEPROM 32-bit write command */
#define EEPROM_CMD_ERASE_PRG_PAGE       (6)     /*!< EEPROM erase/program command */
#define EEPROM_CMD_RDPREFETCH           (1 << 3)/*!< EEPROM read pre-fetch enable */


/******************************************************************************
 * Local variables
 *****************************************************************************/

/******************************************************************************
 * Private Functions
 *****************************************************************************/

void InternalEEPROM::powerUp()
{
  LPC_EEPROM->PWRDWN = 0;
}

void InternalEEPROM::clearInterrupt(uint32_t mask)
{
  LPC_EEPROM->INT_CLR_STATUS = mask;
}

void InternalEEPROM::waitForInterrupt(uint32_t mask)
{
  while ((LPC_EEPROM->INT_STATUS & mask) != mask) {
  }

  clearInterrupt(mask);
}

void InternalEEPROM::setAddr(uint32_t pageAddr, uint32_t pageOffset)
{
  LPC_EEPROM->ADDR = (pageAddr << 6) | pageOffset;
}

void InternalEEPROM::setCmd(uint32_t cmd)
{
  LPC_EEPROM->CMD = cmd;
}

void InternalEEPROM::readPage(uint32_t pageAddr, uint32_t pageOffset, uint8_t* buf, uint32_t size)
{
  clearInterrupt(EEPROM_INT_ENDOFRW);
  setAddr(pageAddr, pageOffset);
  setCmd(EEPROM_CMD_8BITS_READ | EEPROM_CMD_RDPREFETCH);
  for (uint32_t i = 0; i < size; i++) {
    buf[i] = LPC_EEPROM->RDATA & 0xff;
    waitForInterrupt(EEPROM_INT_ENDOFRW);
  }
}

void InternalEEPROM::writePage(uint32_t pageAddr, uint32_t pageOffset, const uint8_t* buf, uint32_t size)
{
  clearInterrupt(EEPROM_INT_ENDOFRW);
  setCmd(EEPROM_CMD_8BITS_WRITE);
  setAddr(pageAddr, pageOffset);
  for (uint32_t i = 0; i < size; i++) {
    LPC_EEPROM->WDATA = buf[i];
    waitForInterrupt(EEPROM_INT_ENDOFRW);
  }
}

void InternalEEPROM::eraseOrProgramPage(uint32_t pageAddr)
{
  // Write data from page register to non-volatile memory
  clearInterrupt(EEPROM_INT_ENDOFPROG);
  setAddr(pageAddr, 0);
  setCmd(EEPROM_CMD_ERASE_PRG_PAGE);
  waitForInterrupt(EEPROM_INT_ENDOFPROG);
}


/******************************************************************************
 * Public Functions
 *****************************************************************************/

InternalEEPROM::InternalEEPROM() : _initialized(false)
{
}

InternalEEPROM::~InternalEEPROM()
{
}

void InternalEEPROM::init()
{
  if (!_initialized) {
    // Bring EEPROM device out of power down mode
    powerUp();
  
    // Setup EEPROM timing to 375KHz based on PCLK rate
    uint32_t clk = SystemCoreClock;
    LPC_EEPROM->CLKDIV = SystemCoreClock / 375000 - 1;
    
    // Setup wait states (ticks needed for 15ms, 55ms and 35ms)
    uint32_t val;
    val  = ((((clk / 1000000) * 15) / 1000) + 1);
    val |= (((((clk / 1000000) * 55) / 1000) + 1) << 8);
    val |= (((((clk / 1000000) * 35) / 1000) + 1) << 16);
    LPC_EEPROM->WSTATE = val;
      
    _initialized = true;
  }
}

void InternalEEPROM::powerDown()
{
  LPC_EEPROM->PWRDWN = EEPROM_PWRDWN;
}

int InternalEEPROM::read(uint32_t addr, uint8_t* data, uint32_t size)
{
  uint32_t numRead = 0;
  uint32_t pageAddr = addr/EEPROM_PAGE_SIZE;
  uint32_t readOffset = (addr & (EEPROM_PAGE_SIZE - 1));
  uint32_t readSize = EEPROM_PAGE_SIZE - readOffset;
  
  // Prevent reading past the end of the memory
  if (addr >= EEPROM_MEMORY_SIZE) {
    return 0;
  }
  if ((size + addr) > EEPROM_MEMORY_SIZE) {
    size = EEPROM_MEMORY_SIZE - addr;
  }

  powerUp();

  // Read and store data in buffer
  while (size) {
    // Make sure we don't write past the end of the buffer
    if (readSize > size) {
      readSize = size;
    }
    
    readPage(pageAddr, readOffset, &(data[numRead]), readSize);
    numRead += readSize;
    size -= readSize;

    // Change to next page
    pageAddr++;
    readOffset = 0;
    readSize = EEPROM_PAGE_SIZE;
  }
  return numRead;
}

int InternalEEPROM::write(uint32_t addr, const uint8_t* data, uint32_t size)
{
  uint32_t numWritten = 0;
  uint32_t pageAddr = addr/EEPROM_PAGE_SIZE;
  uint32_t writeOffset = (addr & (EEPROM_PAGE_SIZE - 1));
  uint32_t writeSize = EEPROM_PAGE_SIZE - writeOffset;

  // Prevent writing past the end of the memory
  if (addr >= EEPROM_MEMORY_SIZE) {
    return 0;
  }
  if ((size + addr) > EEPROM_MEMORY_SIZE) {
    size = EEPROM_MEMORY_SIZE - addr;
  }

  powerUp();

  // Read and store data in buffer
  while (size) {
    // Make sure we don't read past the end of the buffer
    if (writeSize > size) {
      writeSize = size;
    }
    
    writePage(pageAddr, writeOffset, &(data[numWritten]), writeSize);
    eraseOrProgramPage(pageAddr);
    numWritten += writeSize;
    size -= writeSize;

    // Change to next page
    pageAddr++;
    writeOffset = 0;
    writeSize = EEPROM_PAGE_SIZE;
  }
  return numWritten;
}
