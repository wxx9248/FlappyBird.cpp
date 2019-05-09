#pragma once

LPCWSTR static hlpmsg = L"\
用法：\"Flappy Bird.exe\" [参数列表]\
\n\n\
当前Flappy Bird 命令行参数选项：\n\
/help /？		显示本帮助页面。\n\
/debug		调试模式，保留命令行窗口，输出调试信息。\n\
/logfile <路径>	指定日志输出文件路径\n\
		打开此开关同时会启动/debug开关\n\
\
";

void showHelp();
