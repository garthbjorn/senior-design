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

#ifndef BIOSDISPLAY_H
#define BIOSDISPLAY_H

#include "mbed.h"
#include "TouchPanel.h"
#include "Display.h"
#include "bios.h"

/**
 * Glue between the BIOS and the Display interface.
 */
class BiosDisplay : public Display {
public:
    
    /** Get the only instance of the BiosDisplay
     *
     *  @returns The display
     */
    static BiosDisplay& instance()
    {
        static BiosDisplay singleton;
        return singleton;
    }
  

    /** Initializes the display but does not turn it on
     *
     *  @returns
     *       Ok on success
     *       An error code on failure
     */
    DisplayError init();
  
    // From the Display interface
    virtual DisplayError powerUp(void* framebuffer, Resolution wanted = Resolution_16bit_rgb565, FrameRate_t rate = FrameRate_Normal);
    virtual DisplayError powerDown();
    virtual DisplayError backlight(int percent);
    virtual uint16_t width();
    virtual uint16_t height();
    virtual uint16_t bytesPerPixel();
    virtual uint32_t fbSize();
    virtual bool landscape();
    virtual bool isSupported(Resolution res);
    virtual Resolution currentResolution();
    virtual void setFramebuffer(void* buff);
    virtual void* swapFramebuffer(void* buff);
    virtual void* allocateFramebuffer(Resolution res=Resolution_16bit_rgb565);
    virtual void* allocateFramebuffers(uint32_t num=1, Resolution res=Resolution_16bit_rgb565);

private:

    bool _initialized;
    bool _poweredOn;

    bios_header_t* _bios;
    void* _biosData;

    uint16_t _width;
    uint16_t _height;
    uint16_t _bpp;
    uint16_t _supportedRes;
    Resolution_t _activeRes;
    bool _landscape;

    explicit BiosDisplay();
    // hide copy constructor
    BiosDisplay(const BiosDisplay&);
    // hide assign operator
    BiosDisplay& operator=(const BiosDisplay&);
    ~BiosDisplay();

    uint32_t bpp(Resolution res) { return ((res == Resolution_16bit_rgb565) ? 2 : 4); }
};

#endif /* BIOSDISPLAY_H */
