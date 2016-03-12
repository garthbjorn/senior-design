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

#include "RAMFileSystem.h"
#include "DMBoard.h"


#define RAMFS_DBG(...)
//#define RAMFS_DBG(...) DMBoard::instance().logger()->isr_printf(__VA_ARGS__)

#define SECTOR_SIZE   512

RAMFileSystem::RAMFileSystem(uint32_t addr, uint32_t size, const char* name) :
    FATFileSystem(name) {
    memStart = addr;
    memSize = size - (size % SECTOR_SIZE);
    status = 1; //1: disk not initialized
}

int RAMFileSystem::disk_initialize() {
    RAMFS_DBG("init RAM fs\n");
    status = 0; //OK
    return status;
}

int RAMFileSystem::disk_write(const uint8_t *buffer, uint64_t sector, uint8_t count) {
    RAMFS_DBG("write to sector(s) %llu..%llu\n", sector, sector+count);
    if ((sector+count-1) >= disk_sectors()) {
        return 1;
    }
    
    memcpy((uint8_t*)(memStart + SECTOR_SIZE*((uint32_t)sector)), buffer, SECTOR_SIZE*count);
    return 0;
}

int RAMFileSystem::disk_read(uint8_t *buffer, uint64_t sector, uint8_t count) {
    RAMFS_DBG("read from sector(s) %llu..%llu\n", sector, sector+count);
    if ((sector+count-1) >= disk_sectors()) {
        return 1;
    }
    
    memcpy(buffer, (uint8_t*)(memStart + SECTOR_SIZE*((uint32_t)sector)), SECTOR_SIZE*count);
    return 0;
}

int RAMFileSystem::disk_status() { 
    RAMFS_DBG("disk status %d\n", status);
    return status; 
}
int RAMFileSystem::disk_sync() { 
    return 0; 
}
uint64_t RAMFileSystem::disk_sectors() { 
    RAMFS_DBG("returning fs has %u sectors\n", memSize/SECTOR_SIZE);
    return memSize/SECTOR_SIZE; 
}

uint64_t RAMFileSystem::disk_size() {
    return memSize;
}
