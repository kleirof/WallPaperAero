#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(WALLPAPERAERO_LIB)
#  define WALLPAPERAERO_EXPORT Q_DECL_EXPORT
# else
#  define WALLPAPERAERO_EXPORT Q_DECL_IMPORT
# endif
#else
# define WALLPAPERAERO_EXPORT
#endif

#include <QWidget>
#include <QMenu>
#include <QSystemTrayIcon>
#include "qt_windows.h"

class WALLPAPERAERO_EXPORT BackWidget : public QWidget
{
	Q_OBJECT

protected:

	static BOOL CALLBACK CBFindWorkerW1(HWND hWnd, LPARAM lp);

	//获取第一个WorkerW窗口
	HWND GetDesktopWorkerW1();

	//获取第二个WorkerW窗口
	HWND GetDesktopWorkerW2();

	virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result) = 0;
	/* {
		MSG* msg = (MSG*)message;

		switch (msg->message)
		{

		}

		return false;
	}*/

	void setBackImage();

	HWND desktop;
	HWND backWidgetId;
	HMODULE hDLL;
	HOOKPROC mouseProc;
	HHOOK mouseHook;
};
