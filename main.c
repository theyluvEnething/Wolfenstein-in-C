#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>
#include "renderer.h"
#include "frame.h"
#include "level.h"

struct frame frame = {0};
struct frame debug = {0};
struct level level = {0};


const int game_window_width = 1280;
const int game_window_height = 720;

const int debug_window_width = 500;
const int debug_window_height = 500;


static bool running = true;


LRESULT CALLBACK GameWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GameWindowProc2(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

static BITMAPINFO frame_bitmap_info;
static HBITMAP frame_bitmap = 0;
static HDC frame_device_context = 0;

static BITMAPINFO debug_bitmap_info;
static HBITMAP debug_bitmap = 0;
static HDC debug_device_context = 0;


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE pInstance, 
                   PSTR cmd,
                   int showCmd) 
{
    /* INITIALIZE  */
    WNDCLASS game_wnd_class = {0};
    const char CLASS_NAME[] = "EnethWindow";
    game_wnd_class.lpfnWndProc = GameWindowProc;
    game_wnd_class.hInstance = hInstance;
    game_wnd_class.lpszClassName = CLASS_NAME;
    game_wnd_class.hCursor = LoadCursor(NULL, IDC_CROSS);
    
    WNDCLASS debug_wnd_class = {0};
    const char CLASS_NAME_d[] = "EnethWindow";
    debug_wnd_class.lpfnWndProc = GameWindowProc2;
    debug_wnd_class.hInstance = hInstance;
    debug_wnd_class.lpszClassName = CLASS_NAME_d;
    debug_wnd_class.hCursor = LoadCursor(NULL, IDC_CROSS);


    int reg0 = RegisterClass(&game_wnd_class);
    int reg1 = RegisterClass(&debug_wnd_class);

    printf("GameWndClass: %i, DebugWndClass: %i\n", reg0, reg1);

    // if (reg0 == 0) {
    //     // Registration failed
    //     DWORD error = GetLastError();
    //     printf("game winddow class failed %d\n", error);
    //     // You can add additional error handling or exit the program.
    //     return 1;
    // }

    // if (reg1 == 0) {
    //     // Registration failed
    //     DWORD error = GetLastError();
    //     printf("debug winddow class failed %d\n", error);
    //     // You can add additional error handling or exit the program.
    //     return 1;
    // }

    frame_bitmap_info.bmiHeader.biSize = sizeof(frame_bitmap_info.bmiHeader);
    frame_bitmap_info.bmiHeader.biPlanes = 1;
    frame_bitmap_info.bmiHeader.biBitCount = 32;
    frame_bitmap_info.bmiHeader.biCompression = BI_RGB;
    frame_device_context = CreateCompatibleDC(0);

    debug_bitmap_info.bmiHeader.biSize = sizeof(debug_bitmap_info.bmiHeader);
    debug_bitmap_info.bmiHeader.biPlanes = 1;
    debug_bitmap_info.bmiHeader.biBitCount = 32;
    debug_bitmap_info.bmiHeader.biCompression = BI_RGB;
    debug_device_context = CreateCompatibleDC(0);


    /* CREATE WINDOW  */
    HWND game_hwnd = CreateWindowEx(0, CLASS_NAME, "Doom",
                                WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                game_window_width, game_window_height,
                                0, 0, hInstance, 0);

    printf("Initializing Game Window. (%i | %i)\n", frame.width, frame.height);

    HWND debug_hwnd = CreateWindowEx(0, CLASS_NAME, "Debug",
                                WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                debug_window_width, debug_window_height,
                                0, 0, hInstance, 0);

    printf("Initializing Debug Window. (%i | %i)\n", debug.width, debug.height);
    
    if (game_hwnd == NULL || debug_hwnd == NULL) {
        printf("FAILED!\n");
        return 1;
    }

    /* GAME INIT */
    
    // ADD ONE TO ARRAY FOR NULL TERMINATOR
    char* mapArr[5] = {"########",
                       "#--#---#",
                       "#--#---#",
                       "########"};
                     
    char** map = mapArr;
                  
    init_Map(map, &level);

    
    // char initialMap[9][9] = {
    //     {'#','#','#','#','#','#','#','#','#'},
    //     {'#','_','_','_','_','_','_','_','#'},
    //     {'#','_','_','_','_','_','_','_','#'},
    //     {'#','_','_','_','_','_','_','_','#'},
    //     {'#','_','_','_','_','_','_','_','#'},
    //     {'#','_','_','_','_','_','_','_','#'},
    //     {'#','_','_','_','_','_','_','_','#'},
    //     {'#','_','_','_','_','_','_','_','#'},
    //     {'#','#','#','#','#','#','#','#','#'}
    // };


    printf("Widht: %i, Height: %i", frame.width, frame.height);
    printf("Widht: %i, Height: %i", debug.width, debug.height);

    /* GAME LOOP  */
    ShowWindow(game_hwnd, showCmd);
    ShowWindow(debug_hwnd, showCmd);

    MSG msg = {0};

    /* frame */
    print_frame(&frame);
    int moveX = 0;
    int x1 = 500;
    int y1 = 500; 
    int x2 = 700;
    int y2 = 300;
    float deltaTime = 0;

    /* level */
    while (running) 
    {
        while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) 
        {
            if (msg.message == WM_QUIT) 
            {
                running = 0;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // frame.pixels[(p++)%(frame.width*frame.height)] = rand();
        // frame.pixels[rand()%(frame.width*frame.height)] = 0;


        //draw_rectangle(&frame);

        int x, y = 0;

        background(0xFF00FF, &frame);
        background(0x000000, &debug);
        // for (int y=0;y<frame.height;y++) {
        //     for (int x=0;x<frame.width;x++) {
        //         frame.pixels[(x+frame.width*y)%(frame.height*frame.width)] = 0x444444;
        //     }
        // }

        //draw_line(&frame, frame.width/2, 300, frame.width/2+10, 700);
        //draw_line(&frame, 700, 700, 500, 500);

        draw_center_circle(500, 300, 50, 0x00000F, &frame);

        //draw_pi_circle(100, 100, 50, 0x00000F, &frame);

        draw_line(x1, y1, x2, y2, 5, 0xFFFFFF, &frame);
        draw_line(x1, y2, x2, y1, 5, 0xFFFFFF, &frame);
        draw_line(x1, x1, x2, y1, 5, 0xFFFFFF, &frame);
        draw_line(x1, y2, x2, y2, 5, 0xFFFFFF, &frame);
        draw_line(x1, y1, x1, y2, 5, 0xFFFFFF, &frame);
        draw_line(x2, y1, x2, y2, 5, 0xFFFFFF, &frame);

        draw_cube(300, 500, 100, 100, 2, 0xFFFFF, &frame);

        wash_machine(x1, y1, &x2, &y2, 200, deltaTime);
        deltaTime += 0.01f;


        update_screen(&game_hwnd);
        update_screen(&debug_hwnd);
        /*
        InvalidateRect(window_handle, NULL, false);
        UpdateWindow(window_handle);
        */   
    }

    /* UPDATE RENDER */

    return 0;
}

LRESULT CALLBACK GameWindowProc(HWND hwnd, 
                            UINT message,
                            WPARAM wParam,
                            LPARAM lParam) 
{
    switch (message)
    {
        /* PAINT */
        case WM_PAINT: {
            static PAINTSTRUCT paint;
            static HDC device_conext;
            device_conext = BeginPaint(hwnd, &paint);
            BitBlt(device_conext,
                   paint.rcPaint.left, paint.rcPaint.top,
                   paint.rcPaint.right - paint.rcPaint.left, paint.rcPaint.bottom - paint.rcPaint.top,
                   frame_device_context,
                   paint.rcPaint.left, paint.rcPaint.top,
                   SRCCOPY);
            EndPaint(hwnd, &paint);
        } break;

        /* INIT PIXELS */
        case WM_SIZE: {
            frame_bitmap_info.bmiHeader.biWidth = LOWORD(lParam);
            frame_bitmap_info.bmiHeader.biHeight = HIWORD(lParam);

            if (frame_bitmap) DeleteObject(frame_bitmap);
            frame_bitmap = CreateDIBSection(NULL, &frame_bitmap_info, DIB_RGB_COLORS, (void**)&frame.pixels, 0, 0);
            SelectObject(frame_device_context, frame_bitmap);

            frame.width = LOWORD(lParam);
            frame.height = HIWORD(lParam);
        } break;


         /* IDK (Close..?) */
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        } break;     


         /* DEFAULT */
        default: {
            return DefWindowProc(hwnd, message, wParam, lParam);
        } break;
    }
    return 0;
}

LRESULT CALLBACK GameWindowProc2(HWND hwnd, 
                            UINT message,
                            WPARAM wParam,
                            LPARAM lParam) 
{
    printf("\nInitzai9fadadaffafafgasdjasf\n");
    switch (message)
    {
        /* PAINT */
        case WM_PAINT: {
            static PAINTSTRUCT paint_d;
            static HDC device_conext_d;
            device_conext_d = BeginPaint(hwnd, &paint_d);
            BitBlt(device_conext_d,
                   paint_d.rcPaint.left, paint_d.rcPaint.top,
                   paint_d.rcPaint.right - paint_d.rcPaint.left, paint_d.rcPaint.bottom - paint_d.rcPaint.top,
                   device_conext_d,
                   paint_d.rcPaint.left, paint_d.rcPaint.top,
                   SRCCOPY);
            EndPaint(hwnd, &paint_d);
        } break;

        /* INIT PIXELS */
        case WM_SIZE: {
            debug_bitmap_info.bmiHeader.biWidth = LOWORD(lParam);
            debug_bitmap_info.bmiHeader.biHeight = HIWORD(lParam);

            if (debug_bitmap) DeleteObject(debug_bitmap);
            debug_bitmap = CreateDIBSection(NULL, &debug_bitmap_info, DIB_RGB_COLORS, (void**)&debug.pixels, 0, 0);
            SelectObject(debug_device_context, debug_bitmap);

            debug.width = LOWORD(lParam);
            debug.height = HIWORD(lParam);
        } break;


         /* IDK (Close..?) */
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        } break;     


         /* DEFAULT */
        default: {
            return DefWindowProc(hwnd, message, wParam, lParam);
        } break;
    }
    return 0;
}