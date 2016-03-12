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
#include "EwHAL.h"
#include "GUI.h"
#include "GUIDRV_Lin.h"
#include "rtos.h"


/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

/* This value is not important but the same value must be used by both the
   GUI_X_WaitEvent() and the GUI_X_SignalEvent() functions. */
#define EMWIN_TASK_SIGNAL   (0x2)

/******************************************************************************
 * Local variables
 *****************************************************************************/

static EwHAL* _hal = NULL;
static int _gettimeoffset = 0;
static Semaphore* _emSem;

static Timer _ewTimer;
static osThreadId _emTask = 0;

/******************************************************************************
 * Private Functions
 *****************************************************************************/

void EwHAL::handleTouchEvent() {
	GUI_PID_STATE ewCoord = {0};

    if (_touch->read(_coord) == TouchPanel::TouchError_Ok) {
		ewCoord.x = _coord.x;
		ewCoord.y = _coord.y;
		ewCoord.Pressed = (_coord.z == 0) ? 0 : 1;
		GUI_PID_StoreState(&ewCoord);
    }
}

/******************************************************************************
 * Public Functions
 *****************************************************************************/

EwHAL::EwHAL(int numFB, uint32_t extraMem) {

    RtosLog* log = DMBoard::instance().logger();
    _display = DMBoard::instance().display();
    _touch = DMBoard::instance().touchPanel();
    
    _width = _display->width();
    _height = _display->height();
    _fbSz = _display->fbSize();
	_numFB = numFB;
	
    do {
		if (numFB < 1) {
            log->printf("Failed. Must have at least one frame buffer, wanted %d\n", numFB);
            break;
		}

        _fb = (uint32_t)_display->allocateFramebuffers(numFB);
        if (_fb == 0) {
            log->printf("Failed to allocate memory for %d framebuffer(s)\n", numFB);
            break;
        }
        memset((void*)_fb, 0x0, _fbSz * numFB);
        
        _display->setFramebuffer((void*)_fb);
    
        _memSz = extraMem;
        _mem = (uint32_t)malloc(_memSz);
        if (_mem == 0) {
            log->printf("Failed to allocate memory block for emwin (wanted %u bytes)\n", _memSz);
            break;
        }
        memset((void*)_mem, 0x0, _memSz);
        
        _fp = new FunctionPointer(this, &EwHAL::handleTouchEvent);
		_touch->setListener(_fp);
		
		_hal = this;
    } while(false);	
}

EwHAL::~EwHAL() {
	_hal = NULL;
	if (_mem != 0) {
		free((void*)_mem);
		_mem = NULL;
	}
	if (_fb != 0) {
		free((void*)_fb);
		_fb = NULL;
	}
	if (_fp != NULL) {
		_touch->setListener(NULL);
		delete(_fp);
	}
}

void EwHAL::showFrameBuffer(int id) {
    if (id >= 0 && id < getNumFrameBuffers()) {
        _display->setFramebuffer((void*)(_fb + id * _fbSz));
    }
}

/******************************************************************************
 * emWin HAL functions
 *****************************************************************************/

/*********************************************************************
 *
 *       GUI_X_Config
 *
 * Purpose:
 *   Called during the initialization process in order to set up the
 *   available memory for the GUI.
 */
void GUI_X_Config(void) {

    if (_hal == NULL) return;

    //
    // Assign memory to emWin
    //

    GUI_ALLOC_AssignMemory(_hal->getMemoryBlockAddress(), _hal->getMemoryBlockSize());
    GUI_ALLOC_SetAvBlockSize(0x80);
    
    _emTask = Thread::gettid();

    GUI_SetSignalEventFunc(GUI_X_SignalEvent);
    GUI_SetWaitEventFunc(GUI_X_WaitEvent);
    GUI_SetWaitEventTimedFunc(GUI_X_WaitEventTimed);
}

/*********************************************************************
 *
 *       LCD_X_Config
 *
 * Purpose:
 *   Called during the initialization process in order to set up the
 *   display driver configuration.
 */
void LCD_X_Config(void) {

    if (_hal == NULL) return;
    
    //
    // Initialize MultiBuffering
    //
	if (_hal->getNumFrameBuffers() > 1) {
		GUI_MULTIBUF_Config(_hal->getNumFrameBuffers());
	}
     

    GUI_DEVICE_CreateAndLink(&GUIDRV_Lin_16_API, GUICC_M565, 0, 0);

    //
    // Display driver configuration, required for Lin-driver
    //
    LCD_SetPosEx(0, 0, 0);
    if (LCD_GetSwapXYEx(0)) {
        LCD_SetSizeEx  (0, _hal->getDisplayHeight(), _hal->getDisplayWidth());
        LCD_SetVSizeEx (0, _hal->getDisplayHeight(), _hal->getDisplayWidth());
    } else {
        LCD_SetSizeEx  (0, _hal->getDisplayWidth(), _hal->getDisplayHeight());
        LCD_SetVSizeEx (0, _hal->getDisplayWidth(), _hal->getDisplayHeight());
    }
    LCD_SetVRAMAddrEx(0, (void*)0);
    
    // TODO: Add Custom callback routine (DMA) for copying the buffers
    //       Explained in the emWin User Manual rev 5.26 section 21.4.1
}

/*********************************************************************
 *
 *       LCD_X_DisplayDriver
 *
 * Purpose:
 *   This function is called by the display driver for several purposes.
 *   To support the according task the routine needs to be adapted to
 *   the display controller. Please note that the commands marked with
 *   'optional' are not cogently required and should only be adapted if
 *   the display controller supports these features.
 *
 * Parameter:
 *   LayerIndex - Index of layer to be configured
 *   Cmd        - Please refer to the details in the switch statement below
 *   pData      - Pointer to a LCD_X_DATA structure
 *
 * Return Value:
 *   < -1 - Error
 *     -1 - Command not handled
 *      0 - Ok
 */
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
    //	LCD_X_SETORG_INFO * pSetOrg;
    int r;
    U32 TouchOrientation;

    if (_hal == NULL) return -1;

    switch (Cmd) {
    //
    // Required
    //
    case LCD_X_INITCONTROLLER:
        //
        // Called during the initialization process in order to set up the
        // display controller and put it into operation. If the display
        // controller is not initialized by any external routine this needs
        // to be adapted by the customer...
        //
        // ...

        //
        // Set display size and video-RAM address
        //
        //		LCD_SetSizeEx (800, 480, 0);
        //		LCD_SetVSizeEx(800, 480, 0);
        LCD_SetVRAMAddrEx(0, (void*)_hal->getFrameBufferAddress());

        TouchOrientation = (GUI_MIRROR_X * LCD_GetMirrorXEx(0)) |
							(GUI_MIRROR_Y * LCD_GetMirrorYEx(0)) |
							(GUI_SWAP_XY  * LCD_GetSwapXYEx (0)) ;
        GUI_TOUCH_SetOrientation(TouchOrientation);

        return 0;
    case LCD_X_SETORG:
        //
        // Required for setting the display origin which is passed in the 'xPos' and 'yPos' element of p
        //

        // pSetOrg = (LCD_X_SETORG_INFO *)pData;
        //        LPC_LCD->UPBASE = VRAM_ADDR_PHYS +
        //                (pSetOrg->yPos * YSIZE_PHYS * /*PIXEL_WIDTH*/ 2);  // Needs to be set, before LCDC is enabled

        return 0;
    case LCD_X_SHOWBUFFER:
        {
            LCD_X_SHOWBUFFER_INFO * p = (LCD_X_SHOWBUFFER_INFO *)pData;
            //
            // Calculate address of the given buffer
            //
            //unsigned long BufferSize = (XSIZE * YSIZE * BITSPERPIXEL) / 8;
            //unsigned long Addr = _VRamBaseAddr + ewGui->getFrameBufferSize() * pData->Index;
            //
            // Make the given buffer visible
            //
            _hal->showFrameBuffer(p->Index);
            //
            // Send a confirmation that the buffer is visible now
            //
            GUI_MULTIBUF_Confirm(p->Index);
        }
        return 0;
    default:
        r = -1;
    }

    return r;
}



/*********************************************************************
 *
 *      Timing:
 *                 GUI_X_GetTime()
 *                 GUI_X_Delay(int)
 *
 * Some timing dependent routines require a GetTime and delay function. 
 * Default time unit (tick), normally is 1 ms.
 */

int GUI_X_GetTime(void) {
    int t = _ewTimer.read_ms();
    if (t > 3600000) {
        // reset once per hour
        _ewTimer.reset();
        _gettimeoffset += t;
        t = 0;
    }
    return t + _gettimeoffset;
}

void GUI_X_Delay(int ms) {
    Thread::wait(ms);
}

/*********************************************************************
 *
 *       GUI_X_Init()
 *
 * Note:
 *     GUI_X_Init() is called from GUI_Init is a possibility to init
 *     some hardware which needs to be up and running before the GUI.
 *     If not required, leave this routine blank.
 */
void GUI_X_Init(void) {
    _ewTimer.start();
}


/*********************************************************************
 *
 *       GUI_X_ExecIdle
 *
 * Note:
 *  Called if WM is in idle state
 */

void GUI_X_ExecIdle(void) {}

/*********************************************************************
 *
 *      Multitasking:
 *
 *                 GUI_X_InitOS()
 *                 GUI_X_GetTaskId()
 *                 GUI_X_Lock()
 *                 GUI_X_Unlock()
 *
 * Note:
 *   The following routines are required only if emWin is used in a
 *   true multi task environment, which means you have more than one
 *   thread using the emWin API.
 *   In this case the
 *                       #define GUI_OS 1
 *  needs to be in GUIConf.h
 */

   
static void emTimeout(void const* args) {
	GUI_X_SignalEvent();
}

void GUI_X_InitOS(void) {
	_emSem = new Semaphore(1); 
}

void GUI_X_Unlock(void) {
	_emSem->release();
}

void GUI_X_Lock(void) {
	_emSem->wait();
}

U32  GUI_X_GetTaskId(void) {
	return (U32)Thread::gettid();
}

void GUI_X_WaitEvent(void) {
    Thread::signal_wait(EMWIN_TASK_SIGNAL); //wait forever
}

void GUI_X_SignalEvent(void) {
	if (_emTask) {
		osSignalSet(_emTask, EMWIN_TASK_SIGNAL);
	}
}

void GUI_X_WaitEventTimed(int Period) {
    static RtosTimer _emTim(emTimeout, osTimerOnce);
    if (Period > 0) {
        
        //wait no more than Period milliseconds
        _emTim.start(Period);
        Thread::signal_wait(EMWIN_TASK_SIGNAL);
        _emTim.stop();
    }
}

/*********************************************************************
 *
 *      Logging: OS dependent

Note:
  Logging is used in higher debug levels only. The typical target
  build does not use logging and does therefor not require any of
  the logging routines below. For a release build without logging
  the routines below may be eliminated to save some space.
  (If the linker is not function aware and eliminates unreferenced
  functions automatically)

 */

void GUI_X_Log     (const char *s) { DMBoard::instance().logger()->printf("emWin[Log] %s\n", s); }
void GUI_X_Warn    (const char *s) { DMBoard::instance().logger()->printf("emWin[Log] %s\n", s); }
void GUI_X_ErrorOut(const char *s) { DMBoard::instance().logger()->printf("emWin[Log] %s\n", s); }



/*********************************************************************
 *
 *       GUI_TOUCH_X_ActivateX()
 *
 * Function decription:
 *   Called from GUI, if touch support is enabled.
 *   Switches on voltage on X-axis,
 *   prepares measurement for Y-axis.
 *   Voltage on Y-axis is switched off.
 */
void GUI_TOUCH_X_ActivateX(void) {
}

/*********************************************************************
 *
 *       GUI_TOUCH_X_ActivateY()
 *
 * Function decription:
 *   Called from GUI, if touch support is enabled.
 *   Switches on voltage on Y-axis,
 *   prepares measurement for X-axis.
 *   Voltage on X-axis is switched off.
 */
void GUI_TOUCH_X_ActivateY(void) {
}

/*********************************************************************
 *
 *       GUI_TOUCH_X_MeasureX()
 *
 * Function decription:
 *   Called from GUI, if touch support is enabled.
 *   Measures voltage of X-axis.
 */
int  GUI_TOUCH_X_MeasureX(void) {
    if (_hal == NULL) return 0;

    return _hal->getTouchX();
}

/*********************************************************************
 *
 *       GUI_TOUCH_X_MeasureY()
 *
 * Function decription:
 *   Called from GUI, if touch support is enabled.
 *   Measures voltage of Y-axis.
 */
int  GUI_TOUCH_X_MeasureY(void) {
    if (_hal == NULL) return 0;

    return _hal->getTouchY();
}




