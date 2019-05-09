// Flappy Bird.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Flappy Bird.hpp"
#include "showHelp.hpp"
#include "log.hpp"

const int WNDWIDTH					= 768;
const int WNDHEIGHT					= 896;


int main(_In_ int argc, _In_ char *argv[])
{
	std::locale newLocale(std::locale(), "", std::locale::ctype);
	std::locale prevLocale = std::locale::global(newLocale);

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
	Log *log = NULL;

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
	
	if (cmdLineCfg::isDebugMode)
	{
		log = new Log;

		if (cmdLineCfg::fileLogged)
		{
			// Convert path to wstring.
			int num = MultiByteToWideChar(CP_OEMCP, 0, argv[cmdLineCfg::argvLogFilePathIndex], -1, NULL, 0);
			wchar_t *ws = new wchar_t[num];
			MultiByteToWideChar(CP_OEMCP, 0, argv[cmdLineCfg::argvLogFilePathIndex], -1, ws, num);
			wsLogPath = ws;
			delete[] ws;

			log->init(wsLogPath);
		}
		else
		{
			log->init();
		}
	}

	_getch();
	closegraph();
	
	std::locale::global(prevLocale);

	return EXIT_SUCCESS;
}


bool cmdLineCfg::parseCmdLine(_In_ int argc, _In_ char *argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		if (!_stricmp(argv[i], "/?") || !_stricmp(argv[i], "/help"))
			showHelp = true;

		if (!_stricmp(argv[i], "/logfile"))
		{
			if (argv[i + 1] == NULL || strchrs(argv[i + 1], "\"/:*?<>|", true))
				throw invalidParameters(L"/logfile 开关参数解析失败：非法路径");
			else
			{
				fileLogged = true;
				argvLogFilePathIndex = i + 1;
				isDebugMode = true;
			}
		}
		else if (!_stricmp(argv[i], "/debug"))
			isDebugMode = true;


		// TODO: Add more cmdline options here.
	}
	return true;
}


HWND createEXWindow(int width, int height, bool isWindowShow)
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
