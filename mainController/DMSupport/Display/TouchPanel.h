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

#ifndef TOUCHPANEL_H
#define TOUCHPANEL_H

#include "bios.h"

/**
 * An abstract class that represents touch panels.
 */
class TouchPanel {
public:

    enum TouchError {
        TouchError_Ok                =   BiosError_Ok,
        TouchError_ConfigError       =   BiosError_ConfigError,
        TouchError_WrongBPP          =   BiosError_WrongBPP,
        TouchError_InvalidParam      =   BiosError_InvalidParam,
        TouchError_NoInit            =   BiosError_NoInit,
        TouchError_CalibrationError  =   BiosError_Calibration,
        TouchError_Timeout           =   BiosError_Timeout,
        TouchError_TouchNotSupported =   BiosError_NotSupported,
        TouchError_MemoryError,
    };

    /**
     * Read coordinates from the touch panel.
     *
     * In case of multitouch (capacitive touch screen) only the first touch
     * will be returned.
     *
     * @param coord pointer to coordinate object. The read coordinates will be
     * written to this object.
     *
     *  @returns
     *       Ok on success
     *       An error code on failure
     */
    virtual TouchError read(touch_coordinate_t &coord) = 0;

    /**
     * Read up to num coordinates from the touch panel.
     *
     * @param coords a list of at least num coordinates
     * @param num the number of coordinates to read
     *
     *  @returns
     *       Ok on success
     *       An error code on failure
     */
    virtual TouchError read(touch_coordinate_t* coord, int num) = 0;
    
    /**
     * Returns information about the touch panel
     *
     * @param resistive true for Resistive, false for Capacitive
     * @param maxPoints the maximum number of simultaneous touches
     * @param calibration true if the controller can be calibrated
     *
     *  @returns
     *       Ok on success
     *       An error code on failure
     */
    virtual TouchError info(bool* resistive, int* maxPoints, bool* calibrated) = 0;

    /**
     * Start to calibrate the display
     *
     *  @returns
     *       Ok on success
     *       An error code on failure
     */
    virtual TouchError calibrateStart() = 0;

    /**
     * Get the next calibration point. Draw an indicator on the screen
     * at the coordinates and ask the user to press/click on the indicator.
     * Please note that waitForCalibratePoint() must be called after this
     * method.
     *
     * @param x    the x coordinate is written to this argument
     * @param y    the y coordinate is written to this argument
     * @param last true if this is the last coordinate in the series
     *
     *  @returns
     *       Ok on success
     *       An error code on failure
     */
    virtual TouchError getNextCalibratePoint(uint16_t* x, uint16_t* y, bool* last=NULL) = 0;

    /**
     * Wait for a calibration point to have been pressed and recored.
     * This method must be called just after getNextCalibratePoint().
     *
     * @param morePoints true is written to this argument if there
     * are more calibrations points available; otherwise it will be false
     * @param timeout maximum number of milliseconds to wait for
     * a calibration point. Set this argument to 0 to wait indefinite.
     *
     *  @returns
     *       Ok on success
     *       An error code on failure
     */
    virtual TouchError waitForCalibratePoint(bool* morePoints, uint32_t timeout) = 0;
    
    virtual FunctionPointer* setListener(FunctionPointer* listener) = 0;
};

#endif
