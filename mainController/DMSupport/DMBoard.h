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

#ifndef DMBOARD_H
#define DMBOARD_H

#include "mbed.h"
#include "dm_board_config.h"
#include "RtosLog.h"

#if defined(DM_BOARD_USE_MCI_FS)
  #include "MCIFileSystem.h"
#endif
#if defined(DM_BOARD_USE_QSPI_FS)
  #include "SPIFI.h"
  #include "QSPIFileSystem.h"
#elif defined(DM_BOARD_USE_QSPI)
  #include "SPIFI.h"
#endif
#include "Display.h"
#include "TouchPanel.h"
#if defined(DM_BOARD_USE_REGISTRY)
  #include "Registry.h"
#endif


/**
 * Example of using the Board class:
 *
 * @code
 * #include "mbed.h"
 * #include "DMBoard.h"
 *
 * int main(void) {
 *    DMBoard* board = &DMBoard::instance();
 *    board->init();
 *    ...
 *    board->setLed(1, true);
 * }
 * @endcode
 */
class DMBoard {
public:
    enum Leds {
        Led1,
        Led2,
        Led3,
        Led4,
    };
  
    enum BoardError {
        Ok                =       0,
        MemoryError,
        SpifiError,
        DisplayError,
        TouchError,
        BiosInvalidError,
        BiosVersionError,
        BiosStorageError,
        RegistryError,
    };
    
    /** Get the only instance of the DMBoard
     *
     *  @returns The DMBoard
     */
    static DMBoard& instance()
    {
        static DMBoard singleton;
        return singleton;
    }
  
    /** Initializes the wanted features
     *
     *  @returns
     *       Ok on success
     *       An error code on failure
     */
    BoardError init();
  
    /** Controls the four LEDs on the Display Module
     *
     * @param led One of Led1, Led2, Led3 or Led4
     * @param on true to turn the LED on regardless of its polarity
     */
    void setLED(Leds led, bool on);
    
    /** Controls the buzzer
     *
     * Examples:
     *   buzzer()        turns it off
     *   buzzer(440)     plays an A4 (440Hz) note forever
     *   buzzer(200, 25) plays a 200Hz tone for 25ms and then turns it off
     *
     * Note that if duration_ms is >0 this is a blocking call
     *
     * @param frequency   the frequency of the tone (in Hz) or 0 to turn it off
     * @param duration_ms the number of milliseconds to play or 0 for forever
     */
    void buzzer(int frequency=0, int duration_ms=0);
    
    /** Test if the USER button is pressed or not
     *
     *  @returns
     *       True if the button is pressed, false if not
     */
    bool buttonPressed();
    
    /** Returns the TouchPanel interface
     *
     *  @returns
     *       The touch panel
     */
    TouchPanel* touchPanel();

    /** Returns the Display interface
     *
     *  @returns
     *       The display
     */
    Display* display();

    /** Returns the logger interface
     *
     *  @returns
     *       The logger
     */
    RtosLog* logger() { return &_logger; }

#if defined(DM_BOARD_USE_REGISTRY)
    /** Returns the Registry interface
     *
     *  @returns
     *       The registry
     */
    Registry* registry() { return &Registry::instance(); }
#endif
    
#if defined(DM_BOARD_USE_MCI_FS)
    /** Returns the MCI File System instance.
     *  
     *  Can be used to call e.g. cardInserted().
     *
     *  @returns
     *       The file system instance
     */
    MCIFileSystem* getMciFS() { return &_mcifs; }
#else
    void* getMciFS() { return NULL; }
#endif
#if defined(DM_BOARD_USE_QSPI_FS)
    /** Returns the QSPI File System instance.
     *  
     *  Can be used to call e.g. isformatted() and format().
     *
     *  @returns
     *       The file system instance
     */
    QSPIFileSystem* getQspiFS() { return &_qspifs; }
#else
    void* getQspiFS() { return NULL; }
#endif
    

private:

    bool _initialized;

#if defined(DM_BOARD_USE_MCI_FS)
    MCIFileSystem _mcifs;
#endif
#if defined(DM_BOARD_USE_QSPI_FS)
    QSPIFileSystem _qspifs;
#endif

    PwmOut _buzzer;
    DigitalIn _button;
    DigitalOut _led1;
    DigitalOut _led2;
    DigitalOut _led3;
    DigitalOut _led4;

    RtosLog _logger;

    explicit DMBoard();
    // hide copy constructor
    DMBoard(const DMBoard&);
    // hide assign operator
    DMBoard& operator=(const DMBoard&);
    ~DMBoard();
};

#endif
