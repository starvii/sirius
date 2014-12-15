/************************************************************************/
/* @Author: starvii
/************************************************************************/
#include <windows.h>

HINSTANCE   m_hInstance = NULL;
HHOOK       m_hHook = NULL;

/************************************************************************/
/* 检测已启动实例，防止重复运行
/************************************************************************/
BOOL MultiInstanceExists()
{
	HANDLE hMutex = NULL;
	TCHAR *lpszName = "WIN8IME_SWICTH_TOOL_pDK9i6Ho04i3WpjJ";
	
	hMutex = CreateMutex(NULL, FALSE, lpszName);
	DWORD dwRet = GetLastError();

	if (hMutex)
	{
		if (ERROR_ALREADY_EXISTS == dwRet)
		{
			CloseHandle(hMutex);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		MessageBox(NULL, "Create Mutex Error.", "ERROR", MB_OK & MB_ICONERROR);
		return -1;
	}
}

/************************************************************************/
/* 模拟 win + space 按键，切换语言
/************************************************************************/
VOID SendWinSpaceDown()
{
	/* WIN + SPACE 键按下 */
	keybd_event(VK_LWIN, (BYTE)0, 0, 0);
	keybd_event(VK_SPACE, (BYTE)0, 0, 0);
}

VOID SendWinSpaceUp()
{
	/* WIN + SPACE 键释放 */
	keybd_event(VK_SPACE, (BYTE)0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_LWIN, (BYTE)0, KEYEVENTF_KEYUP, 0);
}

/************************************************************************/
/* 全局键盘钩子回调函数
/************************************************************************/
LRESULT CALLBACK KeyBoardProc(
	int nCode,
	WPARAM wParam,
	LPARAM lParam)
{
	KBDLLHOOKSTRUCT *tag = NULL;

	if (nCode == HC_ACTION)
	{
		tag = (KBDLLHOOKSTRUCT *)lParam;

		/* 拦截 ctrl + space 按下事件 */
		if ( WM_KEYDOWN == wParam )
		{
			if (GetKeyState(VK_CONTROL) < 0 && tag->vkCode == VK_SPACE)
			{
				SendWinSpaceDown();
				SendWinSpaceUp();
				return TRUE;
			}
		}
	}

	return CallNextHookEx(m_hHook, nCode, wParam, lParam);
}

/************************************************************************/
/* 主函数
/************************************************************************/
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	BOOL mie = TRUE;
	/* 检测运行实例 */
	mie = MultiInstanceExists();
	if (TRUE == mie)
	{
		return -1;
	}
	else if (-1 == mie)
	{
		return -2;
	}

	m_hInstance = (HINSTANCE)hInstance;

	/* 安装全局键盘钩子 */
	m_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyBoardProc, m_hInstance, 0);

	if (NULL == m_hHook)
	{
		MessageBox(NULL, "Create Keyboard Hook Error.", "ERROR", MB_OK & MB_ICONERROR);
		return -3;
	}

	/* 进入消息循环，保证全局键盘钩子生效 */
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{

	}

	return 0;
}