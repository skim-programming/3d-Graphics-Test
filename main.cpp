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

bool keyPressedOnce(int vk)
{
    static std::unordered_map<int, bool> wasDown;

    bool isDown = (GetAsyncKeyState(vk) & 0x8000) != 0;
    bool pressed = isDown && !wasDown[vk];

    wasDown[vk] = isDown;
    return pressed;
}

// Inspired by DPP library's run_once function
template<typename F>
void run_once(F&& fn)
{
    static bool ran = false;
    if (!ran)
    {
        ran = true;
        fn();
    }
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

	// Validate edge indices
    bool caught = false;
    for (auto& e : edges) {
        for (int idx : e) {
            if (idx < 0 || idx >= vertices.size()) {
                std::cerr << "Invalid index: " << idx << "\n";
                caught = true;
            }
        }
    }
    if (caught) {
        throw std::runtime_error("Invalid edge indices detected. Check logs for details.");
    }

    // Pre-Loop Initialization
	float zVal = 1.0f;
	float xVal = 0.0f;

    float angle = 0.0f;

	std::vector<Vector3d> rotatedVertices = vertices;

    // Face forward
    for(auto& v : rotatedVertices) {
        v.z = -v.z;
	}

    bool vRender = false;
    bool eRender = true;

    bool checkToggle = false;

    float closestV = 1000.0f;

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

        if (vRender)
        {
            for (const auto& v : rotatedVertices)
            {
                Vector3d tv = v;
                tv.z += zVal;
				tv.x += xVal;
                drawPixel(screen(project(tv)));
            }

        }

		if (eRender)
        {
            for (const auto& e : edges)
            {
                for (int i = 0; i < e.size(); ++i) {
                    Vector3d a = rotatedVertices[e[i]];
                    Vector3d b = rotatedVertices[e[(i + 1) % e.size()]];
					a.z += zVal; b.z += zVal;
					a.x += xVal; b.x += xVal;
                    drawLine(screen(project(a)), screen(project(b)));
                }
               
            }
        }

        for (auto& v : rotatedVertices) {
            if (v.z < closestV) {
                closestV = v.z;
            }
        }

        if (zVal < -closestV * 2) {
            zVal = -closestV * 2;
        }

        if (keyPressedOnce('V')) {
            vRender = !vRender;
		}

        if (keyPressedOnce('E')) {
            eRender = !eRender;
        }
        if (keyPressedOnce('R')) {
            rotatedVertices = vertices;
            for (auto& v : rotatedVertices) {
                v.z = -v.z;
            }
        }
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            rotatedVertices = rotateMeshInPlace(rotatedVertices, -angle, Z_AXIS);
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            rotatedVertices = rotateMeshInPlace(rotatedVertices, angle, Z_AXIS);
        }
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            rotatedVertices = rotateMeshInPlace(rotatedVertices, -angle, Y_AXIS);
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            rotatedVertices = rotateMeshInPlace(rotatedVertices, angle, Y_AXIS);
        }
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            rotatedVertices = rotateMeshInPlace(rotatedVertices, -angle, X_AXIS);
		}
        if (GetAsyncKeyState('W') & 0x8000) {
            zVal -= 0.1f;
        }
        if (GetAsyncKeyState('S') & 0x8000) {
            zVal += 0.1f;
        }
        if (GetAsyncKeyState('A') & 0x8000) {
            xVal += 0.1f;
        }
        if (GetAsyncKeyState('D') & 0x8000) {
            xVal -= 0.1f;
        }

        angle = 0.10f;// 2.0f * 3.14159265358f * 1/60;

        closestV = 1000.0f;

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
