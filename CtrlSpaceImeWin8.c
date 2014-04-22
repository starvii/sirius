/************************************************************************/
/* @Author: starvii                                                     */
/************************************************************************/
#include <windows.h>


HINSTANCE   m_hInstance = NULL;
HHOOK       m_hHook = NULL;


/************************************************************************/
/* 模拟 ctrl + shift 按键，切换语言                                     */
/************************************************************************/
VOID sendCtrlShift()
{
	/* ctrl shift 键按下 */
	keybd_event(VK_CONTROL, (BYTE)0, 0, 0);
	keybd_event(VK_SHIFT, (BYTE)0, 0, 0);

	/* ctrl shift 键释放 */
	keybd_event(VK_SHIFT, (BYTE)0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, (BYTE)0, KEYEVENTF_KEYUP, 0);

	/* 恢复 ctrl 键按下状态 */
	keybd_event(VK_CONTROL, (BYTE)0, 0, 0);
}

/************************************************************************/
/* 全局键盘钩子回调函数                                                 */
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
		if (WM_KEYDOWN == wParam)
		{
			if (GetKeyState(VK_CONTROL) < 0 && tag->vkCode == VK_SPACE)
			{
				sendCtrlShift();
				return TRUE;
			}
		}

		/* 拦截 ctrl + space 抬起事件，并切换输入语言 */
		/* Win8之前输入法切换是由按键按下触发的，但win8由按键抬起触发 */
		/* 但经过测试，这种方法相应速度较慢，可能导致有时无法切换输入法 */

		/*
		if (WM_KEYUP == wParam || WM_SYSKEYUP == wParam)
		{
		if (GetKeyState(VK_CONTROL) < 0 && tag->vkCode == VK_SPACE)
		{
		sendCtrlShift();
		return TRUE;
		}
		}
		*/

	}

	return CallNextHookEx(m_hHook, nCode, wParam, lParam);
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	m_hInstance = (HINSTANCE)hInstance;

	/* 安装全局键盘钩子 */
	m_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyBoardProc, m_hInstance, 0);

	if (NULL == m_hHook)
	{
		MessageBox(NULL, "无法创建全局键盘钩子！", "错误", MB_OK & MB_ICONERROR);
		return -1;
	}

	/* 进入消息循环，保证全局键盘钩子生效 */
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{

	}

	return 0;
}