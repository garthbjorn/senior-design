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

#include "USBMSD_RAMFS.h"

USBMSD_RAMFS::USBMSD_RAMFS(RAMFileSystem* ramfs, uint16_t vendor_id, uint16_t product_id, uint16_t product_release) :
    USBMSD(vendor_id, product_id, product_release)
{
  this->ramfs = ramfs;
}

int USBMSD_RAMFS::disk_read(uint8_t * data, uint64_t block, uint8_t count)
{
  return ramfs->disk_read(data, block, count);
}

int USBMSD_RAMFS::disk_write(const uint8_t * data, uint64_t block, uint8_t count)
{
  return ramfs->disk_write(data, block, count);
}

int USBMSD_RAMFS::disk_initialize() {
  return ramfs->disk_initialize();
}

uint64_t USBMSD_RAMFS::disk_sectors() {
  return ramfs->disk_sectors();
}

uint64_t USBMSD_RAMFS::disk_size() {
  return ramfs->disk_size();
}

int USBMSD_RAMFS::disk_status() {
  return ramfs->disk_status();
}
