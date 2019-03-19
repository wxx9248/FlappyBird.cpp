// Flappy Bird.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Flappy Bird.hpp"
#include "showHelp.hpp"
#include "log.hpp"

const INT WNDWIDTH					= 768;
const INT WNDHEIGHT					= 896;


int main(_In_ int argc, _In_ char *argv[])
{
	try
	{
		cmdLineCfg::parseCmdLine(argc, argv);
	}
	catch (invalidParameters e)
	{
		MessageBoxW(NULL, e.WCwhat(), L"错误", MB_ICONERROR);
		throw;
	}
	catch (...)
	{
		MessageBoxW(NULL, L"未知的内部错误", L"错误", MB_ICONERROR);
		throw;
	}

	if (cmdLineCfg::showHelp)
	{
		showHelp();
		return EXIT_SUCCESS;
	}

	HWND hWnd = NULL;

	try
	{
		hWnd = createEXWindow(WNDWIDTH, WNDHEIGHT, cmdLineCfg::isDebugMode);
	}
	catch (createWindowFailed e)
	{
		MessageBoxW(NULL, e.WCwhat(), L"错误", MB_ICONERROR);
		throw;
	}
	catch (...)
	{
		MessageBoxW(NULL, L"未知的内部错误", L"错误", MB_ICONERROR);
		throw;
	}

	srand(static_cast<int>(time(static_cast<time_t>(0))));
	settextstyle(64, 0, L"宋体");
	for (; ; )
	{
		cleardevice();
		outtextxy(rand() % WNDWIDTH, rand() % WNDHEIGHT, L"咕");
		Sleep(200);
	}

	_getch();
	closegraph();
	
	return EXIT_SUCCESS;
}


bool cmdLineCfg::parseCmdLine(_In_ int argc, _In_ char *argv[])
{
	for (INT i = 1; i < argc; ++i)
	{
		if (!_stricmp(argv[i], "/?") || !_stricmp(argv[i], "/help"))
			showHelp = true;

		if (!_stricmp(argv[i], "/logfile"))
		{
			if (argv[i + 1] == NULL || (!strchrs(argv[i + 1], "\"/:*?<>|", true)))
				throw invalidParameters(L"/logfile 开关参数解析失败：非法路径");
			else
			{
				fileLogged = true;
				isDebugMode = true;
			}
		}
		else if (!_stricmp(argv[i], "/debug"))
			isDebugMode = true;


		// TODO: Add more cmdline options here.
	}
	return true;
}



HWND createEXWindow(INT width, INT height, bool isWindowShow)
{
	HWND hWnd;
	if (isWindowShow)
		hWnd = initgraph(width, height, SHOWCONSOLE);
	else
		hWnd = initgraph(width, height);

	if (NULL == hWnd)
		throw createWindowFailed(L"无法创建窗口");

	return hWnd;
}

