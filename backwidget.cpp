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

		if (strcmp(str, "W\0o\0r\0k\0e\0r\0W\0") == 0)//////////�˴�str����\0
		{
			*(HWND*)lp = hWnd;
			return FALSE;
		}
	}

	return TRUE;
}

//��ȡ��һ��WorkerW����
HWND BackWidget::GetDesktopWorkerW1()
{
	HWND hWorkerW1 = NULL;
	EnumWindows(BackWidget::CBFindWorkerW1, (LPARAM)&hWorkerW1);
	return hWorkerW1;
}

//��ȡ�ڶ���WorkerW����
HWND BackWidget::GetDesktopWorkerW2()
{
	HWND hWorkerW1 = GetDesktopWorkerW1();
	if (!::IsWindow(hWorkerW1))
		return NULL;

	HWND hWorkerW2 = ::FindWindowEx(0, hWorkerW1, L"WorkerW", NULL);
	return hWorkerW2;
}

//��װ����
void BackWidget::setBackImage()
{
	HWND hProg = FindWindow(L"Progman", L"Program Manager");
	if (hProg == nullptr)
		return;

	//����0x52c��ϢʹProgman�������WorkerW�Ӵ���
	if (GetDesktopWorkerW2() == nullptr)
		SendMessageTimeout(hProg, 0x052c, 0, 0, SMTO_NORMAL, 0x3e8, nullptr);

	desktop = GetDesktopWorkerW2();
	if (desktop == nullptr)
		return;

	backWidgetId = (HWND)winId();
	//****��WorkerW��Ϊ������****//
	SetParent(backWidgetId, desktop);

	setWindowFlags(Qt::FramelessWindowHint);
	showFullScreen();

	///******�����������Ϊ��������******///
	//����InjectionDll.dll����������explorer.exe����Ϣ
	hDLL = LoadLibrary(L"InjectionDll.dll");

	typedef int (*func)(HWND);

	//��ȡdll�е�MouseProc�ص�����
	mouseProc = (HOOKPROC)GetProcAddress(hDLL, "MouseProc");

	//ע�빳��
	mouseHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)mouseProc, hDLL, GetWindowThreadProcessId(GetDesktopWorkerW1(), NULL));

	func SetDstHwnd = (func)GetProcAddress(hDLL, "SetDstHwnd");

	//���ͱ����ڵ�HWND�Ա����
	SetDstHwnd((HWND)winId());
}