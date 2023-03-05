#include <windows.h>
#include <iostream>
#include <cstdio>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int startTime = 0;
    static int elapsedTime = 0;
    static bool isRunning = false;

    switch (msg)
    {
    case WM_CREATE:
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1:
            if (!isRunning)
            {
                startTime = GetTickCount() - elapsedTime;
                isRunning = true;
                SetTimer(hwnd, 4, 10, NULL); //update thời gian mỗi 10 mili
            }
            break;

        case 2:
            if (isRunning)
            {
                KillTimer(hwnd, 4);
                elapsedTime = GetTickCount() - startTime;
                isRunning = false;
            }
            break;

        case 3: // Nút Reset
            KillTimer(hwnd, 4); // Hủy timer
            isRunning = false;
            elapsedTime = 0;
            SetDlgItemText(hwnd, 4, TEXT("00:00:00.000"));
            break;
        }
        break;

    case WM_TIMER:
    {
        elapsedTime = GetTickCount() - startTime;
        int hours = elapsedTime / 3600000;
        int minutes = (elapsedTime % 3600000) / 60000;
        int seconds = (elapsedTime % 60000) / 1000;
        int milliseconds = elapsedTime % 1000;
        TCHAR timeStr[13];
        _stprintf_s(timeStr, _T("%02d:%02d:%02d.%03d"), hours, minutes, seconds, milliseconds); // Chuyển đổi thời gian thành chuỗi
        SetDlgItemText(hwnd, 4, timeStr); // Hiển thị chuỗi thời gian trên cửa sổ
    }
    break;

    case WM_DESTROY:
        KillTimer(hwnd, 1); // Hủy timer khi cửa sổ bị đóng
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("StopwatchClass");
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, TEXT("Cannot register class"), TEXT("Error"), MB_OK);
        return 0;
    }

    HWND hwnd = CreateWindow(TEXT("StopwatchClass"), TEXT("Stopwatch"), WS_OVERLAPPEDWINDOW, 100, 100, 320, 250, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, TEXT("Cannot create window"), TEXT("Error"), MB_OK);
        return 0;
    }

    // custom font
    HFONT hFont = CreateFont(50, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));

    HDC hdc = GetDC(hwnd);
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    // Tạo nút start,stop,reset
    CreateWindow(TEXT("button"), TEXT("Start"), WS_VISIBLE | WS_CHILD, 50, 120, 80, 30, hwnd, (HMENU)1, hInstance, NULL);
    CreateWindow(TEXT("button"), TEXT("Stop"), WS_VISIBLE | WS_CHILD, 135, 120, 80, 30, hwnd, (HMENU)2, hInstance, NULL);
    CreateWindow(TEXT("button"), TEXT("Reset"), WS_VISIBLE | WS_CHILD, 220, 120, 80, 30, hwnd, (HMENU)3, hInstance, NULL);

    //Nhãn thời gian
    CreateWindow(TEXT("static"), TEXT("00:00:00.000"), WS_VISIBLE | WS_CHILD | SS_CENTER, 50, 50, 250, 50, hwnd, (HMENU)4, hInstance, NULL);

    SelectObject(hdc, hOldFont);
    ReleaseDC(hwnd, hdc);

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}