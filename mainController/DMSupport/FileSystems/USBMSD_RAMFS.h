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

#ifndef USBMSDRAMFS_H
#define USBMSDRAMFS_H

#include "mbed.h"
#include "USBMSD.h"
#include "RAMFileSystem.h"
#include <stdint.h>

/**
 * USBMSD_RAMFS class: Allows the mbed board to expose a FAT file system in SDRAM as a USB memory stick
 */
class USBMSD_RAMFS : public USBMSD {
public:

    /**
    * Constructor
    *
    * @param ramfs The RAM file system
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your preoduct_release
    */
    USBMSD_RAMFS(RAMFileSystem* ramfs, uint16_t vendor_id = 0x0703, uint16_t product_id = 0x0104, uint16_t product_release = 0x0001);

protected:

    /*
    * read one or more blocks on a storage chip
    *
    * @param data pointer where will be stored read data
    * @param block starting block number
    * @param count number of blocks to read
    * @returns 0 if successful
    */
    virtual int disk_read(uint8_t* data, uint64_t block, uint8_t count);

    /*
    * write one or more blocks on a storage chip
    *
    * @param data data to write
    * @param block starting block number
    * @param count number of blocks to write
    * @returns 0 if successful
    */
    virtual int disk_write(const uint8_t* data, uint64_t block, uint8_t count);

    /*
    * Disk initilization
    */
    virtual int disk_initialize();

    /*
    * Return the number of blocks
    *
    * @returns number of blocks
    */
    virtual uint64_t disk_sectors();

    /*
    * Return memory size
    *
    * @returns memory size
    */
    virtual uint64_t disk_size();


    /*
    * To check the status of the storage chip
    *
    * @returns status: 0: OK, 1: disk not initialized, 2: no medium in the drive, 4: write protected
    */
    virtual int disk_status();

protected:

    RAMFileSystem* ramfs;
};

#endif
