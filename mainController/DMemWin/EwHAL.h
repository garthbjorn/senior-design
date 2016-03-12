
#ifndef EWHAL_H
#define EWHAL_H

#include "mbed.h"
#include "DMBoard.h"

/**
 * This is the main class to get Segger's emwin library up-and-running
 * with the mbed online tools.
 *
 * This class handles the porting/integration layer in emwin.
 */
class EwHAL {
public:

    EwHAL(int numFB = 1, uint32_t extraMem = 1*1024*1024);
	~EwHAL();

    /**
     * Returns the address to a memory block which may be used by emwin
     * to allocate objects. The memory block returned will be given to emwin
     * by calling GUI_ALLOC_AssignMemory.
     */
    void* getMemoryBlockAddress() { return (void*)_mem; }
	
    /**
     * Returns the size of the memory block returned by
     * getMemoryBlockAddress().
     */
    uint32_t getMemoryBlockSize() { return _memSz; }
    
    /**
     * Returns the width of the display.
     */
    uint32_t getDisplayWidth() { return _width; }
	
    /**
     * Returns the height of the display.
     */
    uint32_t getDisplayHeight() { return _height; }
    
    /**
     * Returns the address of the framebuffer (video RAM). This address will
     * be given to emwin by a call to LCD_SetVRAMAddrEx.
     */
    void* getFrameBufferAddress() { return (void*)_fb; }
	
    /**
     * Returns the size in bytes of the framebuffer (video RAM). 
     */
    uint32_t getFrameBufferSize() { return _fbSz; }
	
    /**
     * Returns the number of frame buffers to use, default is 1 meaning 
     * that drawing takes place on the same buffer that is being shown.
     * Double buffering is 2 and Tripple buffering is 3. No other values
     * should be used.
     */
    int getNumFrameBuffers() { return _numFB; }
	
    /**
     * Shows frame buffer number id. If getNumFrameBuffers() returns 1 then
     * this id will always be 0, if getNumFrameBuffers() returns N then this
     * function will be called with 0..(N-1). 
     */
    void showFrameBuffer(int id);
    
    /**
     * Returns the x coordinate of the latest touch event
     */
    int32_t getTouchX() {return _coord.x;}

    /**
     * Returns the y coordinate of the latest touch event
     */
    int32_t getTouchY() {return _coord.y;}
	

private:

    /**
     * Called when a new touch event is available. Reads the coordinates and
	 * forwards them to emWin
     */
	void handleTouchEvent();

	int _numFB;
	FunctionPointer* _fp;
    uint32_t _width;
    uint32_t _height;
    uint32_t _fb;
    uint32_t _fbSz;
    uint32_t _mem;
    uint32_t _memSz;
    Display*  _display;
    TouchPanel* _touch;
	touch_coordinate_t _coord;
};

#endif

