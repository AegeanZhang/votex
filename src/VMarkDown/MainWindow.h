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
    HINSTANCE _hInst;                                // ��ǰʵ��
    WCHAR _szTitle[MAX_LOADSTRING];                  // �������ı�
    WCHAR _szWindowClass[MAX_LOADSTRING];            // ����������
    HWND _hWnd;                                      // �����ھ��
    int _nCmdShow;
};

#endif /* __MAINWINDOW_H__ */
