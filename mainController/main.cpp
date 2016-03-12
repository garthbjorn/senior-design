#include "mbed.h"
#include "DMBoard.h"
#include "EwHAL.h"
#include "WM.h"
#include "GUI.h"
#include "DIALOG.h"


#include <Interface.h>

int main()
{
  DMBoard::BoardError err;
  DMBoard* board = &DMBoard::instance();
  RtosLog* log = board->logger();
  Display* disp = board->display();
  
  do {
    err = board->init();
    if (err != DMBoard::Ok) {
      log->printf("Failed to initialize the board, got error %d\r\n", err);
      break;
    }
    
    log->printf("\n\nHello World!\n\n");
    
    // Create the HAL for emWin
    // - Use 3 frame buffers for tripple-buffering
    // - Allow emWin to use 12MByte of external SDRAM
    EwHAL hal(3, 12*1024*1024);
    
    // Start display in default mode (16-bit)
    Display::DisplayError disperr = disp->powerUp(hal.getFrameBufferAddress());
    if (disperr != Display::DisplayError_Ok) {
      log->printf("Failed to initialize the display, got error %d\r\n", disperr);
      break;
    }
    
    // Add extra options here
    // - Set WM_CF_MEMDEV option to use "Memory Devices" to reduce flickering
    WM_SetCreateFlags(WM_CF_MEMDEV);
    
    // Execute the emWin example and never return...
    GUI_Init();
    WM_HWIN mainWin;
    mainWin=CreateWindow();
    GUI_ExecCreatedDialog(mainWin);

    while(1)
    {
    	GUI_Delay(10);
    }
    } while(false);
  
  mbed_die();
}
