# WallPaperAero

## 内容

backwidget.h

WallPaperAero.lib

WallPaperAero.dll

## 类声明

```c++
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
```

## 使用说明

使用一个类继承BackWidget类，并且在构造函数中添加

`setBackImage();`

如果需要响应桌面鼠标事件，可以重写保护成员函数

`virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result);`

需要自行添加Qt的运行库以及InjectionDll.dll和WallPaperAero.dll

自行配置编译环境
