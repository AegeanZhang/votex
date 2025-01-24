#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <Windows.h>

#define MAX_LOADSTRING 100

class MainWindow
{
public:
	MainWindow(HINSTANCE hInstance, int nCmdShow);
	virtual ~MainWindow();

public:
    BOOL InitInstance();
    ATOM MyRegisterClass();
    void RunMessageLoop();
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

private:
    HINSTANCE _hInst;                                // 当前实例
    WCHAR _szTitle[MAX_LOADSTRING];                  // 标题栏文本
    WCHAR _szWindowClass[MAX_LOADSTRING];            // 主窗口类名
    HWND _hWnd;                                      // 主窗口句柄
    int _nCmdShow;
};

#endif /* __MAINWINDOW_H__ */
