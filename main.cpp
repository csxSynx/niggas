#include "gui/gui.h"
#include "xor.hpp"


gui m_gui;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	m_gui.initialize();
}


bool __stdcall DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)wWinMain, hInstDLL, 0, NULL));
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
