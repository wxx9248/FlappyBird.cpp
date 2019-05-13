// Flappy Bird.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Flappy Bird.hpp"
#include "showHelp.hpp"

int main(_In_ int argc, _In_ char *argv[])
{
	std::locale newLocale(std::locale(), "", std::locale::ctype);
	std::locale prevLocale = std::locale::global(newLocale);

	try
	{
		cmdLineCfg::parseCmdLine(argc, argv);
	}
	catch (stdWCexception e)
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

	// Initiate log object
	try
	{
		logger = new Log;
	}
	catch (std::bad_alloc e)
	{
		MessageBoxW(NULL, L"无法从系统处分配内存！", L"错误", MB_ICONERROR);
		throw;
	}
	catch (...)
	{
		MessageBoxW(NULL, L"未知的内部错误", L"错误", MB_ICONERROR);
		throw;
	}

	if (cmdLineCfg::fileLogged)
	{
		// Convert path to wstring.
		wchar_t *ws;
		ws = char2wchar(argv[cmdLineCfg::argvLogFilePathIndex]);
		wsLogPath = ws;
		delete[] ws;

		logger->init(wsLogPath);
	}
	else
	{
		logger->init();
	}

	try
	{
		hWnd = Game::createEXWindow(WNDWIDTH, WNDHEIGHT, cmdLineCfg::isDebugMode);
	}
	catch (stdWCexception e)
	{
		MessageBoxW(NULL, e.WCwhat(), L"错误", MB_ICONERROR);
		throw;
	}
	catch (...)
	{
		MessageBoxW(NULL, L"未知的内部错误", L"错误", MB_ICONERROR);
		throw;
	}
	

	*logger << L"预初始化完成，正在启动游戏……";
	
	Game::subGame();

	std::locale::global(prevLocale);

	return EXIT_SUCCESS;
}

// Namespace: Game::

void Game::subGame()
{
	*logger << L"正在创建窗口……";
	HWND hWnd = createEXWindow(WNDWIDTH, WNDHEIGHT, cmdLineCfg::isDebugMode);
	*logger << L"窗口句柄：" << hWnd;

	// Initialize background picture
	*logger << L"初始化背景图……";
	OBJIMG BG;
	LAYER lBG;
	INT ZERO = 0;
	loadimage(&(BG.im), L"IMAGE", L"IDR_IMAGE_BG");
	BG.posx = BG.posy = &ZERO;
	BG.dwRop = SRCCOPY;
	lBG.push_back(BG);
	

	// Initialize the picture of Bird
	// with UNDEFINED position.
	OBJIMG Bird1, Bird1_M;
	LAYER lBird;
	loadimage(&(Bird1.im), L"IMAGE", L"IDR_IMAGE_BIRD1");
	loadimage(&(Bird1_M.im), L"IMAGE", L"IDR_IMAGE_BIRD1_M");

	Bird1_M.dwRop = SRCAND;
	Bird1.dwRop = SRCPAINT;

	lBird.push_back(Bird1_M);
	lBird.push_back(Bird1);

	mainScene.push_back(lBG);


	// Initialize refresh thread
	*logger << L"正在创建异步刷新线程……";
	HANDLE hThRef = CreateThread(NULL, 0, refreshLoop, NULL, 0, NULL);
	*logger << L"线程句柄：" << hThRef;





	_getch();
	CloseHandle(hThRef);
	closegraph();
}

HWND Game::createEXWindow(_In_ int width, _In_ int height, _In_ bool isWindowShow)
{
	HWND hWnd;
	if (isWindowShow)
		hWnd = initgraph(width, height, SHOWCONSOLE);
	else
		hWnd = initgraph(width, height);

	if (NULL == hWnd)
	{
		*logger << L"无法创建窗口：窗口句柄无效";
		throw stdWCexception(L"无法创建窗口");
	}

	return hWnd;
}


DWORD WINAPI Game::refreshLoop(LPVOID lpParam)
{
	static iLAYER iLayer;
	static iSCENE iScene;

	while (true)
	{
		BeginBatchDraw();
		for (iScene = mainScene.begin(); iScene != mainScene.end(); ++iScene)
			for (iLayer = (*iScene).begin(); iLayer != (*iScene).end(); ++iLayer)
				putimage(*(*iLayer).posx, *(*iLayer).posy, &((*iLayer).im), (*iLayer).dwRop);
		EndBatchDraw();
	}
}


Game::LPRFONT Game::getRawFontW(LPCWSTR lpResID, LPCWSTR lpResType)
{
	HRSRC hResource = FindResourceW(NULL, lpResID, lpResType);
	if (NULL == hResource)
	{
		throw stdWCexception(L"无法获取字体资源！");
	}

	HGLOBAL hGlobal = LoadResource(NULL, hResource);
	if (NULL == hGlobal)
	{
		throw stdWCexception(L"无法装载字体资源！");
	}

	LPVOID lpRawFont = LockResource(hGlobal);
	if (NULL == lpRawFont)
	{
		throw stdWCexception(L"字体资源无效！");
	}

	return lpRawFont;
}


// Namespace cmdLineCfg::

bool cmdLineCfg::parseCmdLine(_In_ int argc, _In_ char *argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		if (!_stricmp(argv[i], "/?") || !_stricmp(argv[i], "/help"))
			showHelp = true;

		if (!_stricmp(argv[i], "/logfile"))
		{
			if (argv[i + 1] == NULL || strchrs(argv[i + 1], "\"/:*?<>|", true))
				throw stdWCexception(L"/logfile 开关参数解析失败：非法路径");
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

