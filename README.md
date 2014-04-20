# Ctrl + Space IME switcher for WIN8

A windows keyboard hook program. It intercepts the CTRL + SPACE pressing action and sends the CTRL + SHIFT pressing action to Windows 8 system.

So you can use CTRL + SPACE to switch IME just like the other Windows systems.

# 任务计划配置

在“任务计划”添加关于 CtrlSpaceImeWin8.exe 的启动项，在启动时自动运行。

1. 常规 -> 安全选项 -> 运行任务时，请使用下列用户帐户：Administrators，只在用户登录时运行，使用最高权限运行，隐藏
2. 触发器 -> 当任何用户登录时
3. 启动程序 -> C:\Windows\CtrlSpaceImeWin8.exe（路径可自定义）
4. 条件 -> 所有选项均不选择
5. 设置 -> 勾选【允许按需运行任务】、【如果过了计划开始时间，立即启动任务】，【请勿启动新实例】
