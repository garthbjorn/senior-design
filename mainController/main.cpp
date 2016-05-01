#include "mbed.h"
#include "rtos.h"
#include "DMBoard.h"
#include "EwHAL.h"
#include "WM.h"
#include "GUI.h"
#include "DIALOG.h"
#include "pinmap.h"

// EMWIN DEFINES FOR TEMP FEILD
#define ID_EDIT_0    (GUI_ID_USER + 0x1A)
// EMWIN DEFINES FOR TIME RUNNING
#define ID_EDIT_4       (GUI_ID_USER + 0x24)

// Objects for our devices
static Serial slave(p12, p13);
DigitalOut led1(LED3);
PwmOut motor(p7);
InterruptIn hall(p4);
Timer speedTim;
static volatile bool running = 0;
volatile int seconds = 0;

// Global window for referencing the mainWindow
static WM_HWIN mainWin;

double speed = 0;

// Hold the defalut and changed values for the custom profile
int values[3][6] = {{1500,3000,0,2500,700,1000},{950,700,600,0,1800,500},{3,2,780,60,15,15}};

// Initialize to profile 1
static int profile = 1;

/*
 * Simple enumeration for profile selection
 */
enum profiles{
	CUST,
	PROF1,
	PROF2,
	PROF3,
};

/*
 * simple enumeration for indexing our array
 */
enum type{
	GAS,
	AIR,
	TIME,
};

/*
 * Create the mainWindow for execution
 *
 * @return handle for main window
 */
WM_HWIN CreateHome(void);

/*
 * Convert an Int to a String.
 *
 * @param Int to convert
 * @return string version of passed int
 */
char *convertIntToString(int i) {
  char *str = (char *) malloc(sizeof(int) * i/10 + 1);
  sprintf(str, "%d", i);
  return str;
}

/*
 * Convert an Int to a Double.
 * @param Double to convert
 * @return string version of passed double
 */
char *convertDoubleToString(double i) {
  char *str = (char *) malloc(sizeof(double) * i/10 + 1);
  sprintf(str, "%f", i);
  return str;
}

/*
 * Select profile 1.
 */
void setProfile1(){
	profile = PROF1;
}

/*
 * Select profile 2.
 */
void setProfile2(){
	profile = PROF2;
}

/*
 * Select profile 3.
 */
void setProfile3(){
	profile = PROF3;
}

/*
 * Select custom profile.
 */
void setProfileCust(){
	profile = CUST;
}

/*
 * Send over all of the values in the values array.
 */
void sendValues(){
	int i;
	for(i=0;i<6;i++){
		slave.printf("az%d%d",i+1,convertIntToString(values[0][i]));
		wait_ms(10);
	}
	for(i=0;i<6;i++){
		slave.printf("gz%d%d",i+1,convertIntToString(values[1][i]));
		wait_ms(10);
	}
	for(i=0;i<6;i++){
		slave.printf("tz%d%d",i+1,convertIntToString(values[2][i]));
		wait_ms(10);
	}
}

/*
 * Switches on the selected profile and sends the
 * correlating information to the slave.
 */
void start(){
	switch(profile){
	case CUST:
		sendValues();
		slave.puts("pz0\n");
		wait_ms(10);
		slave.printf("pz0\n");
		break;
	case PROF1:
		slave.puts("pz1\n");
		wait_ms(10);
		slave.printf("pz1\n");
		break;
	case PROF2:
		slave.puts("pz2\n");
		wait_ms(10);
		slave.printf("pz2\n");
		break;
	case PROF3:
		slave.puts("pz3\n");
		wait_ms(10);
		slave.printf("pz3\n");
		break;
	}
	wait_ms(10);
	slave.puts("sz\n");
	running = 1;
}

/*
 * Sends the stop command.
 * It sends multiple to ensure even if our slave is
 * busy it will get the command.
 */
void stop(){
	slave.puts("xz\n");
	wait_ms(10);
	slave.printf("xz\n");
	wait_ms(10);
	slave.printf("xz\n");

	running = 0;
	seconds = 0;
}

/*
 * Thread that will handle all uart communication.
 * It will send a c and handle a returned temperature.
 */
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
		Thread::yield();
	}
}

/*
 * This is experimental to read the frequency of the hall
 * sensor. Currently it is not working, but the concept is
 * to read a timer every rising edge of the input and
 * calculating a running speed.
 */
void rising()
{
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

/*
 * This thread will control the time running displayed on
 * the screen.
 */
void timing_thread(void const *args)
{
	while(1)
	{
		if(running){
			WM_HWIN hItem;
			hItem = WM_GetDialogItem(mainWin,ID_EDIT_4);
			Thread::wait(1000);
			seconds++;
			EDIT_SetText(hItem,convertIntToString(seconds));
		}
		Thread::yield();
	}
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
    // - Use 3 frame buffers for triple-buffering
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

    // Select gpio function for pin 4
    pin_function(p4,0);
	motor = 0;						// Disable motor pwm output

	// Spin off a thread for handling the uart
    Thread uartthread(uart_thread);

    // Spin off a thread for the timing
    Thread timingThread(timing_thread);
    motor = 0;

    // This assigns the rising edge interrupt
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
