#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>
#include "renderer.h"
#include "frame.h"

struct frame frame = {0};

static bool running = true;
const int window_width = 1200;
const int window_height = 800;


LRESULT CALLBACK WindowProc(HWND window_handle, UINT msg, WPARAM wParam, LPARAM lParam);

static BITMAPINFO frame_bitmap_info;
static HBITMAP frame_bitmap = 0;
static HDC frame_device_context = 0;


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE pInstance, 
                   PSTR cmd,
                   int showCmd) 
{
    /* INITIALIZE  */
    WNDCLASS wnd_class = {0};
    const char CLASS_NAME[] = "EnethWindow";
    wnd_class.lpfnWndProc = WindowProc;
    wnd_class.hInstance = hInstance;
    wnd_class.lpszClassName = CLASS_NAME;
    wnd_class.hCursor = LoadCursor(NULL, IDC_CROSS);
    RegisterClass(&wnd_class);
    

    frame_bitmap_info.bmiHeader.biSize = sizeof(frame_bitmap_info.bmiHeader);
    frame_bitmap_info.bmiHeader.biPlanes = 1;
    frame_bitmap_info.bmiHeader.biBitCount = 32;
    frame_bitmap_info.bmiHeader.biCompression = BI_RGB;
    frame_device_context = CreateCompatibleDC(0);


    /* CREATE WINDOW  */
    HWND window_handle = CreateWindowEx(0, CLASS_NAME, "Doom",
                                WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                window_width, window_height,
                                0, 0, hInstance, 0);
    
    if (window_handle == NULL) {
        return 1;
    }



    
    /* GAME LOOP  */
    ShowWindow(window_handle, showCmd);
    MSG msg = {0};

    print_pointer(&frame);
    int moveX = 0;

    int x1 = 500;
    int y1 = 500; 
    int x2 = 700;
    int y2 = 300;
    float deltaTime = 0;


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

        static unsigned int p = 0;
        // frame.pixels[(p++)%(frame.width*frame.height)] = rand();
        // frame.pixels[rand()%(frame.width*frame.height)] = 0;



        //draw_rectangle(&frame);

        int x, y = 0;

        background(0xFF00FF, &frame);
        // for (int y=0;y<frame.height;y++) {
        //     for (int x=0;x<frame.width;x++) {
        //         frame.pixels[(x+frame.width*y)%(frame.height*frame.width)] = 0x444444;
        //     }
        // }

        //draw_line(&frame, frame.width/2, 300, frame.width/2+10, 700);
        //draw_line(&frame, 700, 700, 500, 500);

        draw_center_circle(500, 300, 50, 0x00000F, &frame);

        draw_line(x1, y1, x2, y2, 5, 0xFFFFFF, &frame);
        draw_line(x1, y2, x2, y1, 5, 0xFFFFFF, &frame);
        draw_line(x1, x1, x2, y1, 5, 0xFFFFFF, &frame);
        draw_line(x1, y2, x2, y2, 5, 0xFFFFFF, &frame);
        draw_line(x1, y1, x1, y2, 5, 0xFFFFFF, &frame);
        draw_line(x2, y1, x2, y2, 5, 0xFFFFFF, &frame);


        IntSpinner(x1, y1, &x2, &y2, 200, deltaTime);
        deltaTime += 0.01f;


        update_screen(&window_handle);
        /*
        InvalidateRect(window_handle, NULL, false);
        UpdateWindow(window_handle);
        */   
    }

    /* UPDATE RENDER */

    return 0;
}

LRESULT CALLBACK WindowProc(HWND window_handle, 
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
            device_conext = BeginPaint(window_handle, &paint);
            BitBlt(device_conext,
                   paint.rcPaint.left, paint.rcPaint.top,
                   paint.rcPaint.right - paint.rcPaint.left, paint.rcPaint.bottom - paint.rcPaint.top,
                   frame_device_context,
                   paint.rcPaint.left, paint.rcPaint.top,
                   SRCCOPY);
            EndPaint(window_handle, &paint);
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
            return DefWindowProc(window_handle, message, wParam, lParam);
        } break;
    }
    return 0;
}