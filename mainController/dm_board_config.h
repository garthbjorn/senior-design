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

#ifndef DM_BOARD_CONFIG_H
#define DM_BOARD_CONFIG_H

// Template to use for the project-specific settings. Copy this file to your project,
// rename it to dm_board_config.h and uncomment the wanted features below:

// #define DM_BOARD_USE_USB_DEVICE
// #define DM_BOARD_USE_USB_HOST
// #define DM_BOARD_USE_MCI_FS
// #define DM_BOARD_USE_QSPI_FS
// #define DM_BOARD_USE_QSPI
#define DM_BOARD_USE_DISPLAY
#define DM_BOARD_USE_TOUCH
// #define DM_BOARD_USE_ETHERNET
#define DM_BOARD_USE_FAST_UART
// #define DM_BOARD_USE_USBSERIAL_IN_RTOSLOG
// #define DM_BOARD_DISABLE_STANDARD_PRINTF
// #define DM_BOARD_ENABLE_MEASSURING_PINS
// #define DM_BOARD_USE_REGISTRY

#endif
