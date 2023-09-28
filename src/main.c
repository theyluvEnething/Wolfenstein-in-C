#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>

#include "headers/renderer.h"
#include "headers/frame.h"
#include "headers/level.h"
#include "headers/func.h"
#include "headers/vector2.h"
#include "headers/player.h"
#include "headers/input.h"
#include "headers/const.h"

/* FRAME */
struct frame frame = {0};
struct frame debug = {0};
/*========*/

/* LEVEL */
struct level level = {0};
/*========*/

/* PLAYER */
struct player player = {0};
/*========*/

/* INPUT */
bool keyboard[256] = {0};
struct {
    int x, y;
    uint_least8_t buttons;
} mouse;
enum 
{ 
    MOUSE_LEFT = 0b1, 
    MOUSE_MIDDLE = 0b10, 
    MOUSE_RIGHT = 0b100, 
    MOUSE_X1 = 0b1000, 
    MOUSE_X2 = 0b10000 
};
/*========*/

/* WINAPI32 */
LRESULT CALLBACK GameWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DebugWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

static BITMAPINFO frame_bitmap_info;
static HBITMAP frame_bitmap = 0;
static HDC frame_device_context = 0;

static BITMAPINFO debug_bitmap_info;
static HBITMAP debug_bitmap = 0;
static HDC debug_device_context = 0;


/* WINDOW */
const int game_window_width = 1080; const int game_window_height = 720;
const int debug_window_width = 800; const int debug_window_height = 500;


static bool mn_wnd_running = true; static bool mn_wnd_minimized = false;
static bool db_wnd_running = true; static bool db_wnd_minimized = false;


void Initialize() {
    player.pos.x = 0.25; 
    player.pos.y = 0.25;
    player.lookangle = 0.0;
    player.health = 100;
    player.fov = 80;
    player.raycount = game_window_width/4;
    int sensitivity = 5;
    player.turnspeed = sensitivity * (PI/180);
    print_info("Player: %p\n", &player);
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE pInstance, 
                   PSTR cmd,
                   int showCmd) 
{
    printf("\n=========================================\n");

    Initialize();

    float mn_ratio = game_window_width / (float)game_window_height;
    float db_ratio = debug_window_width / (float)debug_window_height;
    if (mn_ratio != db_ratio) {
        print_warning("Incompatible ratio between main window and debug window.\n");
        //printf(ANSI_COLOR_YELLOW "[WARNING]" ANSI_COLOR_RESET " - Incompatible ratio between main window and debug window.\n");
    }

    /* INITIALIZE  */
    WNDCLASS game_wnd_class = {0};
    const char CLASS_NAME[] = "GameWindow";
    game_wnd_class.lpfnWndProc = GameWindowProc;
    game_wnd_class.hInstance = hInstance;
    game_wnd_class.lpszClassName = CLASS_NAME;
    game_wnd_class.hCursor = LoadCursor(NULL, IDC_CROSS);
    
    WNDCLASS debug_wnd_class = {0};
    const char CLASS_NAME_d[] = "DebugWindow";
    debug_wnd_class.lpfnWndProc = DebugWindowProc;
    debug_wnd_class.hInstance = hInstance;
    debug_wnd_class.lpszClassName = CLASS_NAME_d;
    debug_wnd_class.hCursor = LoadCursor(NULL, IDC_CROSS);


    int reg0 = RegisterClass(&game_wnd_class);
    int reg1 = RegisterClass(&debug_wnd_class);

    printf("GameWndClass: %i, DebugWndClass: %i\n", reg0, reg1);

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

    HWND debug_hwnd = CreateWindowEx(0, CLASS_NAME_d, "Debug",
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
    char* mapArr[7] = {"###########",
                       "#----++---#",
                       "#---------#",
                       "#---###---#",
                       "#---#-----#",
                       "###########"};

    init_map((char**)mapArr, &level);

    print_map(&level);

    /* GAME LOOP  */
    ShowWindow(game_hwnd, showCmd);
    ShowWindow(debug_hwnd, showCmd);

    /* frame */
    print_pointer(&frame);
    static unsigned int x1 = 100;
    static unsigned int y1 = 100; 
    static unsigned int x2 = 700;
    static unsigned int y2 = 300;
    static float deltaTime = 0;

    /* level */
    while (mn_wnd_running || db_wnd_running) 
    {
		static MSG message = { 0 };
		while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) { DispatchMessage(&message); }        

        static int keyboard_x = 0, keyboard_y = 0;
        if(keyboard[VK_RIGHT] || keyboard['D']) ++keyboard_x;
        if(keyboard[VK_LEFT]  || keyboard['A']) --keyboard_x;
        if(keyboard[VK_UP]    || keyboard['W']) ++keyboard_y;
        if(keyboard[VK_DOWN]  || keyboard['S']) --keyboard_y;

        if(keyboard_x < 0)			keyboard_x = 0;
		if(keyboard_x > frame.width-1)	keyboard_x = frame.width-1;
		if(keyboard_y < 0)			keyboard_y = 0;
		if(keyboard_y > frame.height-1)	keyboard_y = frame.height-1;


        x1 = mouse.x;
        y1 = mouse.y;

        /* MAIN WINDOW */
        if (!is_minimized(&frame)) {
            background(0xFF0000, &frame);

            draw_center_circle(vector2(500, 300), 100, 0xFFFFFF, &frame);
            draw_rectangle_wireframe(vector2(300, 300), vector2(100, 100), 5, 0xFFFFF, &frame);

            struct vector2 temp = vector2(x2, y2);

            draw_line(vector2(x1, y1), vector2(temp.x, temp.y), 5, 0xFFFFFF, &frame);
            draw_line(vector2(x1, y2), vector2(temp.x, y1), 5, 0xFFFFFF, &frame);
            draw_line(vector2(x1, y1), vector2(x1, temp.y), 5, 0xFFFFFF, &frame);
            draw_line(vector2(x2, y1), vector2(temp.x, temp.y), 5, 0xFFFFFF, &frame);
            draw_line(vector2(x1, y2), vector2(temp.x, temp.y), 5, 0xFFFFFF, &frame);
            draw_line(vector2(x1, y1), vector2(temp.x, y1), 5, 0xFFFFFF, &frame);
            wash_machine(vector2(x1, y1), &temp, 200, deltaTime);
            deltaTime += 0.02f;

            draw_cube(vector2(300, 500), vector2(100, 100), 2, 0xFFFFF, &frame);

            update_screen(&game_hwnd);
        }
        /* DEBUG WINDOW */
        if (!is_minimized(&debug)) {
            background(0x000000, &debug);
            db_map_level(&level, &debug);

            draw_player(&player, &level, &debug);


            update_screen(&debug_hwnd);
        }
    }

    /* UPDATE RENDER */

    printf("=========================================\n");
    PostQuitMessage(0);
    return 0;
}

static void InputCollection(const char input) {
    printf("[%c | %i]\n", (uint8_t)input, (uint8_t)input);
    HandlePlayerInput(input, &player);
}

LRESULT CALLBACK GameWindowProc(HWND hwnd, 
                            UINT message,
                            WPARAM wParam,
                            LPARAM lParam) 
{
    static bool has_focus = true;
    switch (message)
    {
        /* PAINT */
        case WM_PAINT: {
            static PAINTSTRUCT paint;
            static HDC device_context;
            device_context = BeginPaint(hwnd, &paint);
            BitBlt(device_context,
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
            mn_wnd_running = false;
            db_wnd_running = false;
            //PostQuitMessage(0);
            return 0;
        } break;     

        case WM_SETFOCUS: has_focus = true; break;

        case WM_KILLFOCUS: {
            printf("Debug Window is now in focus.\n");
			has_focus = false;
			memset(keyboard, 0, 256 * sizeof(keyboard[0]));
			mouse.buttons = 0;
		} break;

        /* KEYBOARD */
        case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
        {
			if (!has_focus)
                break;

            static bool key_is_down, key_was_down;
            key_is_down  = ((lParam & (1 << 31)) == 0);
            key_was_down = ((lParam & (1 << 30)) != 0);
            keyboard[(uint8_t)wParam] = key_is_down;
            InputCollection((uint8_t)wParam);

            switch(wParam) 
            {
                case VK_ESCAPE: 
                    mn_wnd_running = false; 
                    db_wnd_running = false;
                    PostQuitMessage(0);
                break;  
            }
        } break;

        /* MOUSE */
        case WM_MOUSEMOVE: {
			mouse.x = LOWORD(lParam);
			mouse.y = frame.height - 1 - HIWORD(lParam);
		} break;

		case WM_LBUTTONDOWN: mouse.buttons |=  MOUSE_LEFT;   break;
		case WM_LBUTTONUP:   mouse.buttons &= ~MOUSE_LEFT;   break;
		case WM_MBUTTONDOWN: mouse.buttons |=  MOUSE_MIDDLE; break;
		case WM_MBUTTONUP:   mouse.buttons &= ~MOUSE_MIDDLE; break;
		case WM_RBUTTONDOWN: mouse.buttons |=  MOUSE_RIGHT;  break;
		case WM_RBUTTONUP:   mouse.buttons &= ~MOUSE_RIGHT;  break;

		case WM_XBUTTONDOWN: {
			if(LOWORD(wParam) == XBUTTON1) {
					 mouse.buttons |= MOUSE_X1;
			} else { mouse.buttons |= MOUSE_X2; }
		} break;
		case WM_XBUTTONUP: {
			if(LOWORD(wParam) == XBUTTON1) {
					 mouse.buttons &= ~MOUSE_X1;
			} else { mouse.buttons &= ~MOUSE_X2; }
		} break;

		case WM_MOUSEWHEEL: {
			printf("%s\n", wParam & 0b10000000000000000000000000000000 ? "Down" : "Up");
		} break;

         /* DEFAULT */
        default: {
            return DefWindowProc(hwnd, message, wParam, lParam);
        } break;
    }
    return 0;
}

LRESULT CALLBACK DebugWindowProc(HWND hwnd, 
                            UINT message,
                            WPARAM wParam,
                            LPARAM lParam) 
{
    static bool has_focus = true;
    switch (message)
    {
        /* PAINT */
        case WM_PAINT: {
            static PAINTSTRUCT paint;
            static HDC device_context;
            device_context = BeginPaint(hwnd, &paint);
            BitBlt(device_context,
                   paint.rcPaint.left, paint.rcPaint.top,
                   paint.rcPaint.right - paint.rcPaint.left, paint.rcPaint.bottom - paint.rcPaint.top,
                   debug_device_context,
                   paint.rcPaint.left, paint.rcPaint.top,
                   SRCCOPY);
            EndPaint(hwnd, &paint);
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
            db_wnd_running = false;
            //PostQuitMessage(0);
            return 0;
        } break;     

        case WM_SETFOCUS: has_focus = true; break;

        case WM_KILLFOCUS: {
            printf("Main Window is now in focus.\n");
			has_focus = false;
			memset(keyboard, 0, 256 * sizeof(keyboard[0]));
			mouse.buttons = 0;
		} break;

        

        /* KEYBOARD */
        case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
        {
			if (!has_focus)
                break;

            static bool key_is_down, key_was_down;
            key_is_down  = ((lParam & (1 << 31)) == 0);
            key_was_down = ((lParam & (1 << 30)) != 0);
            keyboard[(uint8_t)wParam] = key_is_down;
            InputCollection((uint8_t)wParam);

            switch(wParam) 
            {
                case VK_ESCAPE: 
                    mn_wnd_running = false; 
                    db_wnd_running = false;
                break;  
            }
        } break;


        /* MOUSE */
        /*
        case WM_MOUSEMOVE: {
			mouse.x = LOWORD(lParam);
			mouse.y = frame.height - 1 - HIWORD(lParam);
		} break;

		case WM_LBUTTONDOWN: mouse.buttons |=  MOUSE_LEFT;   break;
		case WM_LBUTTONUP:   mouse.buttons &= ~MOUSE_LEFT;   break;
		case WM_MBUTTONDOWN: mouse.buttons |=  MOUSE_MIDDLE; break;
		case WM_MBUTTONUP:   mouse.buttons &= ~MOUSE_MIDDLE; break;
		case WM_RBUTTONDOWN: mouse.buttons |=  MOUSE_RIGHT;  break;
		case WM_RBUTTONUP:   mouse.buttons &= ~MOUSE_RIGHT;  break;

		case WM_XBUTTONDOWN: {
			if(LOWORD(wParam) == XBUTTON1) {
					 mouse.buttons |= MOUSE_X1;
			} else { mouse.buttons |= MOUSE_X2; }
		} break;
		case WM_XBUTTONUP: {
			if(LOWORD(wParam) == XBUTTON1) {
					 mouse.buttons &= ~MOUSE_X1;
			} else { mouse.buttons &= ~MOUSE_X2; }
		} break;

		case WM_MOUSEWHEEL: {
			printf("%s\n", wParam & 0b10000000000000000000000000000000 ? "Down" : "Up");
		} break;

        */

         /* DEFAULT */
        default: {
            return DefWindowProc(hwnd, message, wParam, lParam);
        } break;
    }
    return 0;
}