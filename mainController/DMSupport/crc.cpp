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
 

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "mbed.h"
#include "crc.h"


/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

#define NUM_CRC_BUFF_ENTRIES  (100)

#define CRC32_INIT()  do { LPC_CRC->MODE = 0x00000036; LPC_CRC->SEED = 0xffffffff; } while(0)

#define CRC32_WRITE8(__val)   LPC_CRC->WR_DATA_BYTE.DATA = (uint8_t)(__val)
#define CRC32_WRITE16(__val)  LPC_CRC->WR_DATA_WORD.DATA = (uint16_t)(__val)
#define CRC32_WRITE32(__val)  LPC_CRC->WR_DATA_DWORD.DATA = (__val)

#define CRC32_SUM()           LPC_CRC->SUM

/******************************************************************************
 * External global variables
 *****************************************************************************/

/******************************************************************************
 * Local variables
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/


/******************************************************************************
 * Public Functions
 *****************************************************************************/

uint32_t crc_File(FILE* f)
{
  uint32_t* buff = (uint32_t*)malloc(sizeof(uint32_t)*NUM_CRC_BUFF_ENTRIES);
  if (buff != NULL) {
    CRC32_INIT();
    fseek(f, 0, SEEK_SET);
    memset(buff, 0, sizeof(uint32_t)*NUM_CRC_BUFF_ENTRIES);
    int numRead = fread(buff, sizeof(uint32_t), NUM_CRC_BUFF_ENTRIES, f);
    while (numRead > 0) {
      for (int i = 0; i < numRead; i++) {
        CRC32_WRITE32(buff[i]);
      }
      numRead = fread(buff, sizeof(uint32_t), NUM_CRC_BUFF_ENTRIES, f);
    }
    free(buff);
    return CRC32_SUM();
  }
  return 0;
}

uint32_t crc_Buffer(const uint32_t* data, uint32_t size)
{
  CRC32_INIT();
  for (uint32_t i = 0; i < size; i++) {
    CRC32_WRITE32(data[i]);
  }
  return CRC32_SUM();
}

