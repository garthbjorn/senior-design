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

#ifndef RAMFILESYSTEM_H
#define RAMFILESYSTEM_H

#include "mbed.h"
#include "FATFileSystem.h"
#include "sdram.h"
#include <stdint.h>

/** Creates a FAT file system in SDRAM
 *
 * @code
 * #include "mbed.h"
 * #include "RAMFileSystem.h"
 *
 * RAMFileSystem ramfs(0xA0000000, 4*1024*1024, "ram"); // 4MB of ram starting at 0xA...
 *  
 * int main() {
 *     sdram_init();
 *
 *     FILE *fp = fopen("/ram/myfile.txt", "w");
 *     fprintf(fp, "Hello World!\n");
 *     fclose(fp);
 * }
 * @endcode
 */
class RAMFileSystem : public FATFileSystem {
public:

    /** Create the File System in RAM
     *
     * @param addr Start of memory to use for file system
     * @param size Number of bytes to use for file system
     * @param name The name used to access the virtual filesystem
     */
    RAMFileSystem(uint32_t addr, uint32_t size, const char* name);
    virtual int disk_initialize();
    virtual int disk_status();
    virtual int disk_read(uint8_t * buffer, uint64_t sector, uint8_t count);
    virtual int disk_write(const uint8_t * buffer, uint64_t sector, uint8_t count);
    virtual int disk_sync();
    virtual uint64_t disk_sectors();

    uint64_t disk_size();

protected:

    uint32_t memStart;
    uint32_t memSize;
    int status;
};

#endif
