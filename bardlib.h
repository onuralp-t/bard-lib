#ifndef BARDLIB_H
#define BARDLIB_H

#define WIN32_LEAN_AND_MEAN
#include "windows.h" // IWYU pragma: keep

#include <stdint.h> // IWYU pragma: keep
#include <stddef.h> // IWYU pragma: keep

typedef struct Window_State {
    union {
        struct { // win32
        HINSTANCE instance;
        WNDCLASS window_class;
        WNDPROC window_proc;
        HWND window;
        BITMAPINFO bitmap_info;
        } win32;
    };
    int window_should_close;
    uint64_t keyboard; // indices 0 - 26 is ASCII english alphabet, set bit means key is being pressed
    uint32_t* pixels;
    uint64_t window_width;
    uint64_t window_height;
} Window_State;

void bard_create_window_win32(uint64_t window_width, uint64_t window_height, const char* window_title);
void bard_begin_drawing_win32();
void bard_end_drawing_win32();

#endif // BARDLIB_H
#ifdef BARDLIB_IMPLEMENTATION

// GLOBAL
Window_State global_state;

// MACRO

#define BARD_BITSET(val, index) val = (uint64_t)((val) | (uint64_t)((uint64_t)1 << (index)))
#define BARD_BITUNSET(val, index) val = ((val) & ~((uint64_t)1 << (index))) 
#define BARD_BITTEST(val, index) (uint64_t)((val) & (uint64_t)((uint64_t)1 << (index)))

#define BARD_LAST_ASCII ('Z' - 65)
#define BARD_ESC (BARD_LAST_ASCII + 1)

#define BARD_RGB(r, g, b) (uint32_t)((0x00 << 24) | (((uint8_t)(r)) << 16) | (((uint8_t)(g)) << 8) | ((uint8_t)(b)))

#define R_RGB(rgb) (uint8_t)(((rgb) >> 16) & 0xFF)
#define G_RGB(rgb) (uint8_t)(((rgb) >> 8) & 0xFF)
#define B_RGB(rgb) (uint8_t)((rgb) & 0xFF)

#define BARD_MIN(x_, y_) ((x_) < (y_) ? (x_) : (y_))
#define BARD_MAX(x_, y_) ((x_) > (y_) ? (x_) : (y_))
#define BARD_CLAMP(value_, minimum_, maximum_) BARD_MAX(minimum_, BARD_MIN(value_, maximum_))

// FUNCTION DEFINITION

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE: { // user attempted to close the window
            // DestroyWindow(hwnd);
            PostQuitMessage(0);
        } break;

        case WM_DESTROY: { // the closed window's resources are getting released
            // thread is quiting (sends WM_QUIT, not handled by windproc, used to exit the inf loop in main)
            PostQuitMessage(0);
        } break;

        case WM_KEYDOWN: { // https://learn.microsoft.com/en-gb/windows/win32/inputdev/virtual-key-codes
            switch(wParam)
            {
                case 'A': BARD_BITSET(global_state.keyboard, 'A' - 65); break;
                case 'B': BARD_BITSET(global_state.keyboard, 'B' - 65); break;
                case 'C': BARD_BITSET(global_state.keyboard, 'C' - 65); break;
                case 'D': BARD_BITSET(global_state.keyboard, 'D' - 65); break;
                case 'E': BARD_BITSET(global_state.keyboard, 'E' - 65); break;
                case 'F': BARD_BITSET(global_state.keyboard, 'F' - 65); break;
                case 'G': BARD_BITSET(global_state.keyboard, 'G' - 65); break;
                case 'H': BARD_BITSET(global_state.keyboard, 'H' - 65); break;
                case 'I': BARD_BITSET(global_state.keyboard, 'I' - 65); break;
                case 'J': BARD_BITSET(global_state.keyboard, 'J' - 65); break;
                case 'K': BARD_BITSET(global_state.keyboard, 'K' - 65); break;
                case 'L': BARD_BITSET(global_state.keyboard, 'L' - 65); break;
                case 'M': BARD_BITSET(global_state.keyboard, 'M' - 65); break;
                case 'N': BARD_BITSET(global_state.keyboard, 'N' - 65); break;
                case 'O': BARD_BITSET(global_state.keyboard, 'O' - 65); break;
                case 'P': BARD_BITSET(global_state.keyboard, 'P' - 65); break;
                case 'Q': BARD_BITSET(global_state.keyboard, 'Q' - 65); break;
                case 'R': BARD_BITSET(global_state.keyboard, 'R' - 65); break;
                case 'S': BARD_BITSET(global_state.keyboard, 'S' - 65); break;
                case 'T': BARD_BITSET(global_state.keyboard, 'T' - 65); break;
                case 'U': BARD_BITSET(global_state.keyboard, 'U' - 65); break;
                case 'V': BARD_BITSET(global_state.keyboard, 'V' - 65); break;
                case 'W': BARD_BITSET(global_state.keyboard, 'W' - 65); break;
                case 'X': BARD_BITSET(global_state.keyboard, 'X' - 65); break;
                case 'Y': BARD_BITSET(global_state.keyboard, 'Y' - 65); break;
                case 'Z': BARD_BITSET(global_state.keyboard, 'Z' - 65); break;
                case VK_ESCAPE: BARD_BITSET(global_state.keyboard, BARD_ESC); DestroyWindow(hwnd); break;
            }
        } break;

        case WM_KEYUP: {
            switch(wParam)
            {
                case 'A': BARD_BITUNSET(global_state.keyboard, 'A' - 65); break;
                case 'B': BARD_BITUNSET(global_state.keyboard, 'B' - 65); break;
                case 'C': BARD_BITUNSET(global_state.keyboard, 'C' - 65); break;
                case 'D': BARD_BITUNSET(global_state.keyboard, 'D' - 65); break;
                case 'E': BARD_BITUNSET(global_state.keyboard, 'E' - 65); break;
                case 'F': BARD_BITUNSET(global_state.keyboard, 'F' - 65); break;
                case 'G': BARD_BITUNSET(global_state.keyboard, 'G' - 65); break;
                case 'H': BARD_BITUNSET(global_state.keyboard, 'H' - 65); break;
                case 'I': BARD_BITUNSET(global_state.keyboard, 'I' - 65); break;
                case 'J': BARD_BITUNSET(global_state.keyboard, 'J' - 65); break;
                case 'K': BARD_BITUNSET(global_state.keyboard, 'K' - 65); break;
                case 'L': BARD_BITUNSET(global_state.keyboard, 'L' - 65); break;
                case 'M': BARD_BITUNSET(global_state.keyboard, 'M' - 65); break;
                case 'N': BARD_BITUNSET(global_state.keyboard, 'N' - 65); break;
                case 'O': BARD_BITUNSET(global_state.keyboard, 'O' - 65); break;
                case 'P': BARD_BITUNSET(global_state.keyboard, 'P' - 65); break;
                case 'Q': BARD_BITUNSET(global_state.keyboard, 'Q' - 65); break;
                case 'R': BARD_BITUNSET(global_state.keyboard, 'R' - 65); break;
                case 'S': BARD_BITUNSET(global_state.keyboard, 'S' - 65); break;
                case 'T': BARD_BITUNSET(global_state.keyboard, 'T' - 65); break;
                case 'U': BARD_BITUNSET(global_state.keyboard, 'U' - 65); break;
                case 'V': BARD_BITUNSET(global_state.keyboard, 'V' - 65); break;
                case 'W': BARD_BITUNSET(global_state.keyboard, 'W' - 65); break;
                case 'X': BARD_BITUNSET(global_state.keyboard, 'X' - 65); break;
                case 'Y': BARD_BITUNSET(global_state.keyboard, 'Y' - 65); break;
                case 'Z': BARD_BITUNSET(global_state.keyboard, 'Z' - 65); break;
                case VK_ESCAPE: BARD_BITUNSET(global_state.keyboard, BARD_ESC); break;
            }
        } break;

        case WM_KILLFOCUS: global_state.keyboard = 0; break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            int windowWidth = clientRect.right - clientRect.left;
            int windowHeight = clientRect.bottom - clientRect.top;

            StretchDIBits(
                hdc,
                0, 0, windowWidth, windowHeight,
                0, 0, 800, 600,
                global_state.pixels,
                &global_state.win32.bitmap_info,
                DIB_RGB_COLORS,
                SRCCOPY
            );

            EndPaint(hwnd, &ps);
            return 0;
        } break;

        default: {
            return DefWindowProcA(hwnd, uMsg, wParam, lParam);
        } break;
    }
    return 0;
}

void bard_create_window_win32(uint64_t window_width, uint64_t window_height, const char* window_title)
{
    HINSTANCE hinstance = GetModuleHandleA(NULL);

    const char* CLASS_NAME = window_title; // this doesn't determine title name

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hinstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursorA(NULL, IDC_ARROW); // first param is the location of the cursor resource

    global_state.win32.window_proc = WindowProc;

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        window_title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hinstance, NULL
    );

    global_state.win32.instance = hinstance;
    global_state.win32.window_class = wc;
    global_state.win32.window = hwnd;
    global_state.win32.window_proc = WindowProc;

    global_state.pixels = (uint32_t*)VirtualAlloc(NULL, window_width * window_height * sizeof(uint32_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    
    global_state.win32.bitmap_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    global_state.win32.bitmap_info.bmiHeader.biWidth = window_width;
    global_state.win32.bitmap_info.bmiHeader.biHeight = -window_height; // neg -> making (0,0) top left
    global_state.win32.bitmap_info.bmiHeader.biPlanes = 1;
    global_state.win32.bitmap_info.bmiHeader.biBitCount = 32;
    global_state.win32.bitmap_info.bmiHeader.biCompression = BI_RGB;

    global_state.window_width = window_width;
    global_state.window_height = window_height;

    ShowWindow(global_state.win32.window, SW_NORMAL);
}

void bard_begin_drawing_win32()
{
    MSG msg = {0};

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT) {
            global_state.window_should_close = true;
        }

        TranslateMessage(&msg);
        DispatchMessageA(&msg); // calls WindowProc
    }
}

void bard_end_drawing_win32()
{
    RedrawWindow(global_state.win32.window, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
    // InvalidateRect(global_state.win32.window, NULL, FALSE);
}

#endif // BARDLIB_IMPLEMENTATION