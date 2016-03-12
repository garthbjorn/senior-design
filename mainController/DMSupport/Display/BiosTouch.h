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

#ifndef BIOSTOUCH_H
#define BIOSTOUCH_H

#include "mbed.h"
#include "TouchPanel.h"
#include "Display.h"
#include "bios.h"
#include "rtos.h"

class TouchHandler;

/**
 * Glue between the BIOS and the TouchPanel interface.
 */
class BiosTouch : public TouchPanel {
public:
    
    /** Get the only instance of the BiosTouch
     *
     *  @returns The display
     */
    static BiosTouch& instance()
    {
        static BiosTouch singleton;
        return singleton;
    }
  
    /**
     * Initialize the touch controller. This method must be called before
     * calibrating or reading data from the controller
     *
     *  @returns
     *       Ok on success
     *       An error code on failure
     */
    TouchError init();

    /**
     * Tests if a touch controller is available or not.
     *
     * Note that this function only returns a valid value
     * after the display has been intitialized.
     *
     * @return true if there is a touch controller
     */
    bool isTouchSupported();
    
    // From the TouchPanel interface
    virtual TouchError read(touch_coordinate_t &coord);
    virtual TouchError read(touch_coordinate_t* coord, int num);
    virtual TouchError info(bool* resistive, int* maxPoints, bool* calibrated);
    virtual TouchError calibrateStart();
    virtual TouchError getNextCalibratePoint(uint16_t* x, uint16_t* y, bool* last=NULL);
    virtual TouchError waitForCalibratePoint(bool* morePoints, uint32_t timeout);
    virtual FunctionPointer* setListener(FunctionPointer* listener);

private:

    bool _initialized;
    bool _haveInfo;
    bool _poweredOn;

    bios_header_t* _bios;
    void* _biosData;

    Thread* _handlerThread;
    TouchHandler* _handler;

    bool _supportsTouch;
    bool _supportsTouchCalibration;
    uint8_t _touchNumFingers;

    explicit BiosTouch();
    // hide copy constructor
    BiosTouch(const BiosTouch&);
    // hide assign operator
    BiosTouch& operator=(const BiosTouch&);
    ~BiosTouch();
};

#endif /* BIOSTOUCH_H */
