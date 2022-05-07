#include "backwidget.h"
#include <windows.h>
#include <QDebug>
//#include "dllmain.h"

BOOL CALLBACK BackWidget::CBFindWorkerW1(HWND hWnd, LPARAM lp)
{
	if (lp == NULL)
		return FALSE;

	HWND hShl = NULL;
	hShl = FindWindowEx(hWnd, 0, L"SHELLDLL_DefView", 0);

	if (hShl)
	{
		char str[200] = { 0 };
		GetClassName(hWnd, (LPWSTR)str, 200);

		if (strcmp(str, "W\0o\0r\0k\0e\0r\0W\0") == 0)//////////此处str含有\0
		{
			*(HWND*)lp = hWnd;
			return FALSE;
		}
	}

	return TRUE;
}

//获取第一个WorkerW窗口
HWND BackWidget::GetDesktopWorkerW1()
{
	HWND hWorkerW1 = NULL;
	EnumWindows(BackWidget::CBFindWorkerW1, (LPARAM)&hWorkerW1);
	return hWorkerW1;
}

//获取第二个WorkerW窗口
HWND BackWidget::GetDesktopWorkerW2()
{
	HWND hWorkerW1 = GetDesktopWorkerW1();
	if (!::IsWindow(hWorkerW1))
		return NULL;

	HWND hWorkerW2 = ::FindWindowEx(0, hWorkerW1, L"WorkerW", NULL);
	return hWorkerW2;
}

//安装钩子
void BackWidget::setBackImage()
{
	HWND hProg = FindWindow(L"Progman", L"Program Manager");
	if (hProg == nullptr)
		return;

	//发送0x52c消息使Progman分离产生WorkerW子窗口
	if (GetDesktopWorkerW2() == nullptr)
		SendMessageTimeout(hProg, 0x052c, 0, 0, SMTO_NORMAL, 0x3e8, nullptr);

	desktop = GetDesktopWorkerW2();
	if (desktop == nullptr)
		return;

	backWidgetId = (HWND)winId();
	//****将WorkerW设为父窗口****//
	SetParent(backWidgetId, desktop);

	setWindowFlags(Qt::FramelessWindowHint);
	showFullScreen();

	///******以下两个语句为核心内容******///
	//导入InjectionDll.dll来监视桌面explorer.exe的消息
	hDLL = LoadLibrary(L"InjectionDll.dll");

	typedef int (*func)(HWND);

	//读取dll中的MouseProc回调函数
	mouseProc = (HOOKPROC)GetProcAddress(hDLL, "MouseProc");

	//注入钩子
	mouseHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)mouseProc, hDLL, GetWindowThreadProcessId(GetDesktopWorkerW1(), NULL));

	func SetDstHwnd = (func)GetProcAddress(hDLL, "SetDstHwnd");

	//发送本窗口的HWND以便接收
	SetDstHwnd((HWND)winId());
}