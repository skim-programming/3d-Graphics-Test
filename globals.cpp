#include "globals.h"


// initializing all global vars
int screenWidth = 800;
int screenHeight = 600;

// Allocate framebuffer
uint32_t* frameBuffer = new uint32_t[screenWidth * screenHeight];
uint32_t* hitboxBuffer = new uint32_t[screenWidth * screenHeight];
uint32_t* pixelBuffer = new uint32_t[screenWidth * screenHeight];

RenderPool pool = RenderPool(std::thread::hardware_concurrency());

HDC hdc;

BITMAPINFO bmi;

bool keys[256] = {}; // true if key is down

void setupGlobals() {
	// Setup BITMAPINFO (used by StretchDIBits)
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = screenWidth;
	bmi.bmiHeader.biHeight = -screenHeight; // top-down
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
}

void cleanupGlobals() { // Just for safety, OS does it anyways
	delete[] frameBuffer;
	delete[] hitboxBuffer;
    delete[] pixelBuffer;
}

void ResizeFramebuffer(int newWidth, int newHeight)
{
    if (newWidth <= 0 || newHeight <= 0)
        return;

    screenWidth = newWidth;
    screenHeight = newHeight;

    cleanupGlobals();
    frameBuffer = new uint32_t[screenWidth * screenHeight];
	hitboxBuffer = new uint32_t[screenWidth * screenHeight];
	pixelBuffer = new uint32_t[screenWidth * screenHeight];

    // Clear to background color (e.g. black)
    memset(frameBuffer, 0, screenWidth * screenHeight * sizeof(uint32_t));

    // Update BITMAPINFO
    bmi.bmiHeader.biWidth = screenWidth;
    bmi.bmiHeader.biHeight = -screenHeight; // top-down
}