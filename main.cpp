#define NOMINMAX
#include <windows.h>
#include <vector>
#include <iostream>
#include "globals.h"
#include "draw.h"
#include "pengu.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        ReleaseDC(hwnd, hdc);
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
        keys[wParam] = true;
        return 0;

    case WM_KEYUP:
        keys[wParam] = false;
        return 0;

    case WM_SIZE:
        int newWidth = LOWORD(lParam);
        int newHeight = HIWORD(lParam);
        ResizeFramebuffer(newWidth, newHeight);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

Vector2d rotate(Vector2d d, float theta) {
    return {
        d.x * cos(theta) - d.y * sin(theta),
        d.x * sin(theta) + d.y * cos(theta)
    };
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    setupGlobals();
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);

    // Register window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SoftRenderWindow";
    RegisterClass(&wc);

    // Create window
    HWND hwnd = CreateWindowEx(
        0,
        L"SoftRenderWindow",
        L"Software Renderer",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        screenWidth, screenHeight,
        0, 0, hInstance, 0
    );

    hdc = GetDC(hwnd);

	bool running = true;
    MSG msg = {};


    // Clear framebuffer
    for (int i = 0; i < screenWidth * screenHeight; i++) {
        frameBuffer[i] = 0xFF101010;

    }

    // Pre-Loop Initialization
	float zVal = 1.0f;

    float angle = 0.0f;

	std::vector<Vector3d> rotatedVertices = vertices;

    bool vRender = true;
	bool vToggle = false;
    bool eRender = true;
    bool eToggle = false;

    bool checkToggle = false;

    // Main loop
    while (running)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                running = false;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        for (int i = 0; i < screenWidth * screenHeight; i++) {
            frameBuffer[i] = 0xFF101010;

        }


		rotatedVertices = rotateMeshInPlace(vertices, angle);

        if (vRender)
        {
            for (const auto& v : rotatedVertices)
            {
                Vector3d tv = v;
                tv.z += zVal;
                drawPixel(screen(project(tv)));
            }

        }

		if (eRender)
        {
            // Check for invalid indices in edges
            if (!checkToggle) {
                bool caught = false;
                for (auto& e : edges) {
                    for (int idx : e) {
                        if (idx < 0 || idx >= vertices.size()) {
                            std::cerr << "Invalid index: " << idx << "\n";
                            caught = true;
                        }
                    }
                }
                if(caught) {
                    throw std::runtime_error("Invalid edge indices detected. Check logs for details.");
				}
                checkToggle = true;
            }
            for (const auto& e : edges)
            {
                for (int i = 0; i < e.size(); ++i) {
                    Vector3d a = rotatedVertices[e[i]];
                    Vector3d b = rotatedVertices[e[(i + 1) % e.size()]];
					a.z += zVal; b.z += zVal;
                    drawLine(screen(project(a)), screen(project(b)));
                }
               
            }
        }

        if ((GetAsyncKeyState('V') & 0x8000) && !vToggle) {
            vRender = !vRender;
            vToggle = true;
		}
        else if (!(GetAsyncKeyState('V') & 0x8000) && vToggle){
			vToggle = false;
        }

        if ((GetAsyncKeyState('E') & 0x8000) && !eToggle) {
            eRender = !eRender;
            eToggle = true;
        }
        else if(!(GetAsyncKeyState('E') & 0x8000) && eToggle) {
            eToggle = false;
        }

        angle += 0.05f;// 2.0f * 3.14159265358f * 1/60;

        // Actual displaying framebuffer to window
        StretchDIBits(
            hdc,
            0, 0, screenWidth, screenHeight,
            0, 0, screenWidth, screenHeight,
            frameBuffer,
            &bmi,
            DIB_RGB_COLORS,
            SRCCOPY
        );

        Sleep(4);
    }

    return 0;
}
