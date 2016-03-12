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
 
#ifndef __CRC_H
#define __CRC_H

#include <stdio.h>
#include <stdint.h>

/** Calculates the CRC32 checksum on the contents of the file
 *
 *  Note that the position in the file will change as a result of this
 *  call. To get the old position back, use fseek().
 *
 *  @param f   An open file
 *
 *  @returns
 *       checksum on success
 *       0 on failure
 */
uint32_t crc_File(FILE* f);

/** Calculates the CRC32 checksum on the data
 *
 *  @param data   the data
 *  @param size   the number of elements in the data array
 *
 *  @returns
 *       checksum
 */
uint32_t crc_Buffer(const uint32_t* data, uint32_t size);

#endif /* end __CRC_H */
