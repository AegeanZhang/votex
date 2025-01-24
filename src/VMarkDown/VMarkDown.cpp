// VMarkDown.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "VMarkDown.h"

#include "MainWindow.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                        _In_opt_ HINSTANCE hPrevInstance,
                        _In_ LPWSTR    lpCmdLine,
                        _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MainWindow mainWindow(hInstance, nCmdShow);

	if(!mainWindow.InitInstance())
	{
		return FALSE;
	}

	mainWindow.RunMessageLoop();

    return 0;
}
