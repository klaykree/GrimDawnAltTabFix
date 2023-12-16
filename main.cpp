#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <WinUser.h>

void CALLBACK Wineventproc(
    HWINEVENTHOOK hWinEventHook,
    DWORD event,
    HWND hwnd,
    LONG idObject,
    LONG idChild,
    DWORD idEventThread,
    DWORD dwmsEventTime
)
{
    auto test = GetActiveWindow();
    HWND foreground = GetForegroundWindow();
    if(foreground)
    {
        std::string title(256, 0);
        int length = GetWindowTextA(foreground, &title[0], 256);    
        std::transform(title.begin(), title.end(), title.begin(),
            [] (unsigned char c) { return std::tolower(c); });

        title.resize(length);
        std::cout << title << std::endl;

        if(title == "grim dawn")
        {
            Sleep(5);

            INPUT inputs[2] = {};
            ZeroMemory(inputs, sizeof(inputs));

            inputs[0].type = INPUT_KEYBOARD;
            inputs[0].ki.wScan = 0x38;
            inputs[0].ki.dwFlags = KEYEVENTF_SCANCODE;

            inputs[1].type = INPUT_KEYBOARD;
            inputs[1].ki.wScan = 0x38;
            inputs[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

            UINT sent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

            
            std::cout << "sent left alt key " << std::endl;
        }
    }
}

int main()
{
    HWINEVENTHOOK hook = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL, Wineventproc, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}