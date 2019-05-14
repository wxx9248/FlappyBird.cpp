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
	std::wcout << L"窗口句柄：0x" << hWnd << std::endl;

	// Initialize background picture
	*logger << L"初始化背景图层……";
	OBJIMG BG;
	INT ZERO = 0;
	LAYER lBG;
	loadimage(&(BG.im), L"IMAGE", L"IDR_IMAGE_BG");
	BG.posx = BG.posy = &ZERO;
	BG.dwRop = SRCCOPY;
	lBG.push_back(BG);
	mainScene.push_back(lBG);


	// Initialize the picture of Bird
	// with UNDEFINED position.
	*logger << L"初始化Bird图层（坐标未定）……";
	OBJIMG Bird1, Bird1_M;
	LAYER lBird;
	loadimage(&(Bird1.im), L"IMAGE", L"IDR_IMAGE_BIRD1");
	loadimage(&(Bird1_M.im), L"IMAGE", L"IDR_IMAGE_BIRD1_M");

	Bird1_M.dwRop = SRCAND;
	Bird1.dwRop = SRCPAINT;


	// Initialize font resource
	*logger << L"初始化字体资源……";
	std::wcout << L"TTF名称：" << lpFontName << std::endl;
	HANDLE DefFont = GetFontHandleW(L"IDR_TTF_DEFAULT", L"TTF");
	std::wcout << L"TTF资源句柄：0x" << DefFont << std::endl;
	if (NULL == DefFont)
		throw stdWCexception(L"TTF资源句柄无效！");


	// Initialize MUTEX
	*logger << L"正在创建互斥锁（异步刷新线程）……";
	HANDLE hMutRef = CreateMutexW(NULL, FALSE, L"MutexRefresh");
	std::wcout << L"互斥锁句柄：0x" << hMutRef << std::endl;
	if (NULL == hMutRef)
		throw stdWCexception(L"无效互斥锁句柄！");


	// Initialize refresh thread
	*logger << L"正在创建异步刷新线程……";
	HANDLE hThRef = CreateThread(NULL, 0, refreshLoop, hMutRef, 0, NULL);
	std::wcout << L"线程句柄：0x" << hThRef << std::endl;
	
	// Initlialize keyboard event listening thread
	*logger << L"正在创建键盘事件处理线程……";
	HANDLE hThKBEHandler = CreateThread(NULL, 0, KBELoop, hMutRef, 0, NULL);
	std::wcout << L"线程句柄：0x" << hThKBEHandler << std::endl;


	// Game start.

	WaitForSingleObject(hMutRef, INFINITE);
	OpenMutexW(SYNCHRONIZE, FALSE, L"MutexRefresh");

	fxLayers.push_back(printGameTitle);

	ReleaseMutex(hMutRef);

	for (; ; )
	{
		WaitForSingleObject(hMutRef, INFINITE);
		OpenMutexW(SYNCHRONIZE, FALSE, L"MutexRefresh");

		ReleaseMutex(hMutRef);
	}

	CloseHandle(hThRef);
	closegraph();
}

void Game::printGameTitle()
{
	static bool firstrun = true;
	static LOGFONTW LogFontDef = { 0 };
	if (firstrun)
	{	
		LogFontDef.lfHeight = 72;
		wcsncpy_s(LogFontDef.lfFaceName, Game::lpFontName, sizeof(LogFontDef.lfFaceName) / sizeof(WCHAR));
		firstrun = false;
	}

	settextstyle(&LogFontDef);
	setbkmode(TRANSPARENT);
	outtextxy(175, 200, L"Flappy Bird");
}


HWND Game::createEXWindow(const _In_ int width, const _In_ int height, const _In_ bool isWindowShow)
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
	for (; ; )
	{
		WaitForSingleObject((HANDLE *)lpParam, INFINITE);
		OpenMutexW(SYNCHRONIZE, FALSE, L"MutexRefresh");

		BeginBatchDraw();
		for (int i = 0; i < mainScene.size(); ++i)
			for (int j = 0; j < mainScene[i].size(); ++j)
			{
				if (NULL != mainScene[i][j].posx && NULL != mainScene[i][j].posy)
				{
					if (NULL != mainScene[i][j].rot)
					{
						// Rotate picture.
					}
					else
						putimage(*mainScene[i][j].posx, *mainScene[i][j].posy, &(mainScene[i][j].im), mainScene[i][j].dwRop);
				}
			}
		
		for (int i = 0; i < fxLayers.size(); ++i)
			if (NULL != fxLayers[i])
				fxLayers[i]();
		

		EndBatchDraw();

		ReleaseMutex((HANDLE *) lpParam);
	}
	return 0;
}

DWORD WINAPI Game::KBELoop(LPVOID lpParam)
{
	for (; ; )
	{
		char c = _getch();
		*logger << L"捕获到键盘事件：";
		std::wcout << L"16进制机内码为：" << L"0x" << std::hex << (int) c << std::endl;
		std::wcout << L"对应字符为：" << c << std::endl;
		switch (c)
		{
		default:
			break;
		}
	}
	return 0;
}

HANDLE Game::GetFontHandleW(const LPCWSTR lpResID, const LPCWSTR lpResType)
{
	HRSRC hResource = FindResourceW(NULL, lpResID, lpResType);
	if (NULL == hResource)
		throw stdWCexception(L"无法获取字体资源！");

	HGLOBAL hGlobal = LoadResource(NULL, hResource);
	if (NULL == hGlobal)
		throw stdWCexception(L"无法装载字体资源！");

	LPVOID lpRawFont = LockResource(hGlobal);
	if (NULL == lpRawFont)
		throw stdWCexception(L"字体资源无效！");

	size_t fontSize = SizeofResource(NULL, hResource);
	DWORD NumFonts;
	HANDLE hFont = AddFontMemResourceEx(lpRawFont, fontSize, NULL, &NumFonts);
	if (NULL == hFont)
		throw stdWCexception(L"无法获取字体句柄！");

	return hFont;
}


// Namespace cmdLineCfg::

bool cmdLineCfg::parseCmdLine(const _In_ int argc, _In_ char *argv[])
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

