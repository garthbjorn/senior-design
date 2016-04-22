#include "mbed.h"
#include "DMBoard.h"
#include "EwHAL.h"
#include "WM.h"
#include "GUI.h"
#include "DIALOG.h"
#include "rtos.h"
#include "pinmap.h"

#define ID_EDIT_0    (GUI_ID_USER + 0x1A)
#define ID_TEXT_0    (GUI_ID_USER + 0x0C)
static Serial slave(p12, p13);
DigitalOut led1(LED3);
PwmOut motor(p7);
InterruptIn hall(p4);
Timer speedTim;
static WM_HWIN mainWin;
double speed = 0;
int values[2][6] = {{0,0,0,0,0,0},{0,0,0,0,0,0}};
int times = 0;
int id = 0;

WM_HWIN CreateHome(void);

void sendValues(){
	slave.puts("");
	slave.puts(convertIntToString(values[0][0]));
}

char *intToDigits(int num){
	static char digitText[5] = {'0','0','0','0',0};
	int i = 3;
	while(i>=0){
		digitText[i--] = '0' + num%10;
		num/=10;
	}
	return digitText;
}
//Convert an int to a string
char *convertIntToString(int i) {
  char *str = (char *) malloc(sizeof(int) * i/10 + 1);
  sprintf(str, "%d", i);
  return str;
}

char *convertDoubleToString(double i) {
  char *str = (char *) malloc(sizeof(double) * i/10 + 1);
  sprintf(str, "%f", i);
  return str;
}

void uart_thread(void const *args){
	char value[8] = {0};
	uint8_t readChar = 0;
	int index = 0;
	WM_HWIN hItem;

	hItem = WM_GetDialogItem(mainWin,ID_EDIT_0);
    TEXT_SetFont(hItem, GUI_FONT_20F_ASCII);
    TEXT_SetText(hItem, convertIntToString(554));
    slave.putc('c');
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
				hItem = WM_GetDialogItem(mainWin,ID_EDIT_0);
				EDIT_SetText(hItem,convertIntToString(num));
				index = 0;
				slave.putc('c');
			}
		}
		slave.puts(convertDoubleToString(speed));
		Thread::yield();
	}
}

void rising()
{
	led1 = !led1;
//	static int i = 0;
//	static int newTime = 0;
//	i++;
//	if(i == 1)
//	    speedTim.start();
//	if(i == 6){
//		newTime = speedTim.read_ms();
//		speedTim.stop();
//		speedTim.reset();
//		speed = (5./newTime)/1000;
//		i = 0;
//	}
}

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
    WM_MULTIBUF_Enable(0);
	pin_function(p4,0);
    Thread uartthread(uart_thread);
    led1 = 1;
    motor = 0;
//    hall.rise(&rising);
    // Execute the emWin example and never return...
    GUI_Init();

    mainWin = CreateHome();
    GUI_ExecCreatedDialog(mainWin);

    while(1)
    {
    	GUI_Delay(10);
    }
    } while(false);
  
  mbed_die();
}
