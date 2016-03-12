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

#ifndef DISPLAY_H
#define DISPLAY_H

#include "mbed.h"
#include "bios.h"

/**
 * Display example
 *
 * @code
 * #include "mbed.h"
 * #include "DMBoard.h"
 *
 * int main(void) {
 *    // initialize the display
 *    DMBoard::instance().init();
 *
 *    // allocate one framebuffer
 *    Display* disp = DMBoard::instance().display();
 *    uint16_t* fb = (uint16_t*)disp->allocateFramebuffer();
 *    if (fb == NULL) {
 *      DMBoard::instance().logger()->printf("Failed to allocate memory for framebuffer\r\n");
 *      mbed_die();
 *    }
 *
 *    // draw something on the framebuffer
 *    ...
 *
 *    // turn on the display
 *    disperr = disp->powerUp(fb);
 *    if (disperr != Display::Ok) {
 *      DMBoard::instance().logger()->printf("Failed to initialize the display, got error %d\r\n", disperr);
 *      mbed_die();
 *    }
 *
 *    ...
 * }
 * @endcode
 */
class Display {
public:
    enum DisplayError {
        DisplayError_Ok                =   BiosError_Ok,
        DisplayError_ConfigError       =   BiosError_ConfigError,
        DisplayError_WrongBPP          =   BiosError_WrongBPP,
        DisplayError_InvalidParam      =   BiosError_InvalidParam,
        DisplayError_NoInit            =   BiosError_NoInit,
        DisplayError_CalibrationError  =   BiosError_Calibration,
        DisplayError_Timeout           =   BiosError_Timeout,
        DisplayError_TouchNotSupported =   BiosError_NotSupported,
        DisplayError_MemoryError,
    };
    
    enum Resolution {
        Resolution_16bit_rgb565 = Res_16bit_rgb565,
        Resolution_18bit_rgb666 = Res_18bit_rgb666,
        Resolution_24bit_rgb888 = Res_24bit_rgb888,
    };
    
    /** Turns the display on with the specified framebuffer showing
     *
     *  @param framebuffer  the data to show
     *  @param res          the resolution to use
     *  @param rate         the frame rate to use
     *
     *  @returns
     *       Ok on success
     *       An error code on failure
     */
    virtual DisplayError powerUp(void* framebuffer, Resolution res = Resolution_16bit_rgb565, FrameRate_t rate = FrameRate_Normal) = 0;
  
    /** Turns the display off
     *
     *  @returns
     *       Ok on success
     *       An error code on failure
     */
    virtual DisplayError powerDown() = 0;
  
    /** Sets the backlight level. 0% is off and 100% is fully on
     *
     *  @param percent   backlight in %
     *
     *  @returns
     *       Ok on success
     *       An error code on failure
     */
    virtual DisplayError backlight(int percent) = 0;
  
    /** Returns the width (in pixels) of the display
     *
     *  @returns the display width
     */
    virtual uint16_t width() = 0;

    /** Returns the height (in pixels) of the display
     *
     *  @returns the display height
     */
    virtual uint16_t height() = 0;

    /** Returns the number of bytes used by each pixel
     *
     *  @returns bytes per pixel
     */
    virtual uint16_t bytesPerPixel() = 0;

    /** Returns the number of bytes used for each frame buffer
     *
     *  @returns width*height*bytesPerPixel
     */
    virtual uint32_t fbSize() = 0;

    /** Returns the display orientation
     *
     *  @returns the display orientation
     */
    virtual bool landscape() = 0;

    /** Returns true if the specified resolution can be used
     *
     *  @returns true if supported, false if not
     */
    virtual bool isSupported(Resolution res) = 0;

    /** Returns the current resolution
     *
     *  @returns the current resolution
     */
    virtual Resolution currentResolution() = 0;
    
    /** Replaces the current framebuffer.
     *
     * Note that this requires the caller or someone else to have a
     * reference to the existing framebuffer, otherwise that memory
     * is lost.
     *
     *  @param buff the new framebuffer
     */
    virtual void setFramebuffer(void* buff) = 0;
    
    /** Replaces the current framebuffer with the specified one.
     *
     * This function as opposed to the setFramebuffer() one does return
     * the old framebuffer. This way the caller can save the old one and
     * then swap it back when done.
     *
     *  @param buff the new framebuffer
     *  @returns the old framebuffer
     */
    virtual void* swapFramebuffer(void* buff) = 0;
    
    /** Allocate enough memory for one framebuffer
     *
     * This function is a to make it easier to allocate memory for framebuffers
     * as the number of bytes needed depends on width, height and bytes per pixel.
     *
     * Free the allocated memory when done using the free() function.
     *
     *  @param res the resolution (default is the one that the display is using)
     *  @returns a new framebuffer or NULL if out of memory 
     */
    virtual void* allocateFramebuffer(Resolution res=Resolution_16bit_rgb565) = 0;
    
    /** Allocate enough memory for one or more consequtive framebuffers
     *
     * This function is a to make it easier to allocate memory for framebuffers
     * as the number of bytes needed depends on width, height and bytes per pixel.
     *
     * Free the allocated memory when done using the free() function.
     *
     * Use the default parameters to get one framebuffer for the display's current
     * resolution.
     *
     *  @param num the number of framebuffers, should be >= 1
     *  @param res the resolution (default is the one that the display is using)
     *  @returns new framebuffers or NULL if out of memory 
     */
    virtual void* allocateFramebuffers(uint32_t num=1, Resolution res=Resolution_16bit_rgb565) = 0;
};

#endif
