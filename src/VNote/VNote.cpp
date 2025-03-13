#include <windows.h>
#include <vector>

#define ID_TRAY_EXIT 1001
#define HOTKEY_ID    1

const wchar_t g_szClassName[] = L"MyNoteClass";
const wchar_t g_szNoteClass[] = L"NoteWindowClass";
HINSTANCE g_hInst;
NOTIFYICONDATA g_nid = {};

// 前向声明
LRESULT CALLBACK NoteWndProc(HWND, UINT, WPARAM, LPARAM);

class NoteWindow {
public:
    static HWND Create(HINSTANCE hInstance) {
        return CreateWindowEx(
            WS_EX_TOOLWINDOW,
            g_szNoteClass,
            L"新便签",
            WS_POPUP | WS_THICKFRAME | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT,
            300, 200,
            NULL, NULL,
            hInstance,
            NULL);
    }
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        // 注册全局热键 Ctrl+Alt+N
        if (!RegisterHotKey(hwnd, HOTKEY_ID, MOD_CONTROL | MOD_ALT, 0x4E)) {
            MessageBox(NULL, L"热键注册失败!", L"错误", MB_ICONERROR);
        }

        // 初始化托盘图标
        g_nid.cbSize = sizeof(NOTIFYICONDATA);
        g_nid.hWnd = hwnd;
        g_nid.uID = 1;
        g_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        g_nid.uCallbackMessage = WM_APP + 1;
        g_nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wcscpy_s(g_nid.szTip, L"便签应用 (Ctrl+Alt+N)");
        Shell_NotifyIcon(NIM_ADD, &g_nid);
        break;
    }

    case WM_HOTKEY:
        if (wParam == HOTKEY_ID) {
            NoteWindow::Create(g_hInst);
        }
        break;

    case WM_APP + 1: {
        if (lParam == WM_RBUTTONDOWN) {
            POINT pt;
            GetCursorPos(&pt);

            HMENU hMenu = CreatePopupMenu();
            AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT, L"退出");

            SetForegroundWindow(hwnd);
            TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
            DestroyMenu(hMenu);
        }
        break;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_TRAY_EXIT) {
            DestroyWindow(hwnd);
        }
        break;

    case WM_DESTROY:
        Shell_NotifyIcon(NIM_DELETE, &g_nid);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK NoteWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hEdit;

    switch (msg) {
    case WM_CREATE: {
        // 创建编辑控件
        hEdit = CreateWindowEx(
            0, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
            0, 0, 100, 100,
            hwnd, NULL, g_hInst, NULL);

        // 设置字体
        HFONT hFont = CreateFont(18, 0, 0, 0, FW_NORMAL,
            FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"微软雅黑");
        SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

        // 设置背景色
        SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(RGB(255, 255, 200)));
        break;
    }

    case WM_SIZE: {
        // 调整编辑控件大小
        RECT rc;
        GetClientRect(hwnd, &rc);
        SetWindowPos(hEdit, NULL, 0, 0, rc.right, rc.bottom, SWP_NOZORDER);
        break;
    }

    case WM_NCHITTEST: {
        // 允许通过客户区拖动窗口
        LRESULT hit = DefWindowProc(hwnd, msg, wParam, lParam);
        if (hit == HTCLIENT) return HTCAPTION;
        return hit;
    }

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    g_hInst = hInstance;

    // 注册主窗口类
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = g_szClassName;
    RegisterClassEx(&wc);

    // 注册便签窗口类
    WNDCLASSEX noteWc = {};
    noteWc.cbSize = sizeof(WNDCLASSEX);
    noteWc.style = CS_HREDRAW | CS_VREDRAW;
    noteWc.lpfnWndProc = NoteWndProc;
    noteWc.hInstance = hInstance;
    noteWc.hCursor = LoadCursor(NULL, IDC_ARROW);
    noteWc.lpszClassName = g_szNoteClass;
    RegisterClassEx(&noteWc);

    // 创建隐藏主窗口
    HWND hMainWnd = CreateWindowEx(
        0, g_szClassName, NULL, 0,
        0, 0, 0, 0,
        HWND_MESSAGE, NULL, hInstance, NULL);

    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}