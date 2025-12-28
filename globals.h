#include <stdint.h>
#include <windows.h>
#include "thread_pool.h"
#include "vectors.h"

extern int screenHeight;
extern int screenWidth;
extern uint32_t* hitboxBuffer;
extern uint32_t* frameBuffer;
extern HDC hdc;
extern BITMAPINFO bmi;
extern bool keys[256];
extern uint32_t* pixelBuffer;	
extern RenderPool pool;

void setupGlobals();

void ResizeFramebuffer(int newW, int newH);