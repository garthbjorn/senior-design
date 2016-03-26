#include "mbed.h"
#include "DMBoard.h"
#include "EwHAL.h"
#include "WM.h"
#include "GUI.h"
#include "DIALOG.h"
#include "rtos.h"
#include <Interface.h>
#define ID_EDIT_1    (GUI_ID_USER + 0x11)
#define ID_TEXT_0    (GUI_ID_USER + 0x0C)
static Serial slave(p12, p13);
static char tempC [] = "0000";
static char tempF [] = "0000";
DigitalOut led1(LED1);
static WM_HWIN mainWin;

char *intToDigits(int num){
	static char digitText[5] = {'0','0','0','0',0};
	int i = 3;
	while(i>=0){
		digitText[i--] = '0' + num%10;
		num/=10;
	}
	return digitText;
}

void uart_thread(void const *args){
	char value[8] = {0};
	uint8_t readChar = 0;
	int index = 0;
	WM_HWIN hItem;

	hItem = WM_GetDialogItem(mainWin,ID_EDIT_1);
	EDIT_SetText(hItem,intToDigits(1000));
    hItem = WM_GetDialogItem(mainWin, ID_TEXT_0);
    TEXT_SetFont(hItem, GUI_FONT_20F_ASCII);
    TEXT_SetText(hItem, intToDigits(index));
    slave.putc('f');
	while(1){
		wait_ms(100);

		if(slave.readable()){
			readChar = slave.getc();
			value[index++] = readChar;
			if(readChar == 'n'){
				int num = 0;
				int i = 1;
				while(value[i] != 'n'){
					num *= 10;
					num += value[i] - 0x30;
					i++;
				}
				hItem = WM_GetDialogItem(mainWin,ID_EDIT_1);
				EDIT_SetText(hItem,intToDigits(num));
				index = 0;
				slave.putc('f');
			}
		}
	}
}
int main()
{
  DMBoard::BoardError err;
  DMBoard* board = &DMBoard::instance();
  RtosLog* log = board->logger();
  Display* disp = board->display();
  led1 = 0;
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
    
    Thread uartthread(uart_thread);

    // Execute the emWin example and never return...
    GUI_Init();

    mainWin=CreateWindow();
    GUI_ExecCreatedDialog(mainWin);

    while(1)
    {
    	GUI_Delay(10);
    }
    } while(false);
  
  mbed_die();
}
