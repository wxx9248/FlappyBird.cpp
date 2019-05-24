// Flappy Bird.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Flappy Bird.hpp"
#include "showHelp.hpp"

int main(_In_ int argc, _In_ char *argv[])
{
	std::ios::sync_with_stdio(false);
	
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
		Game::hWnd = Game::createEXWindow(WNDWIDTH, WNDHEIGHT, cmdLineCfg::isDebugMode);
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
	

	*logger << L"预初始化完成，正在启动游戏……" << logger->endl;
	
	try
	{
		Game::subGame();
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

	*logger << L"关闭日志系统……" << logger->endl;
	delete logger;

	std::locale::global(prevLocale);

	return EXIT_SUCCESS;
}

// Namespace: Game::

// Class Bird

Game::Bird::Bird
(
	LPCWSTR pResName1, LPCWSTR pResName1_m,
	LPCWSTR pResName2, LPCWSTR pResName2_m,
	LPCWSTR pResName3, LPCWSTR pResName3_m,
	LPCWSTR pResType
) throw()
{
	init
	(
			pResName1, pResName1_m,
			pResName2, pResName2_m,
			pResName3, pResName3_m,
			pResType
	);
}

void Game::Bird::init
(
	LPCWSTR pResName1, LPCWSTR pResName1_m,
	LPCWSTR pResName2, LPCWSTR pResName2_m,
	LPCWSTR pResName3, LPCWSTR pResName3_m,
	LPCWSTR pResType
)
{
	loadimage(imgBird, pResType, pResName1_m);
	if (!(imgBird[0].getwidth() && imgBird[0].getheight()))
	{
		throw stdWCexception(L"Bird1_mask 资源加载失败！");
	}

	loadimage(imgBird + 1, pResType, pResName1);
	if (!(imgBird[1].getwidth() && imgBird[1].getheight()))
	{
		throw stdWCexception(L"Bird1 资源加载失败！");
	}

	loadimage(imgBird + 2, pResType, pResName2_m);
	if (!(imgBird[2].getwidth() && imgBird[2].getheight()))
	{
		throw stdWCexception(L"Bird2_mask 资源加载失败！");
	}
	loadimage(imgBird + 3, pResType, pResName2);
	if (!(imgBird[3].getwidth() && imgBird[3].getheight()))
	{
		throw stdWCexception(L"Bird2 资源加载失败！");
	}

	loadimage(imgBird + 4, pResType, pResName3_m);
	if (!(imgBird[4].getwidth() && imgBird[4].getheight()))
	{
		throw stdWCexception(L"Bird3_mask 资源加载失败！");
	}
	loadimage(imgBird + 5, pResType, pResName3);
	if (!(imgBird[5].getwidth() && imgBird[5].getheight()))
	{
		throw stdWCexception(L"Bird3 资源加载失败！");
	}

	posxBird = (WNDWIDTH - imgBird[0].getwidth()) / 2;
	pBird = this;
}

void Game::Bird::draw()
{
	putimage(posxBird, posyBird, imgBird + birdState * 2, SRCAND);
	putimage(posxBird, posyBird, imgBird + birdState * 2 + 1, SRCPAINT);
}

void Game::Bird::changeState()
{
	if (++birdState > 2)
		birdState = 0;
}

void Game::Bird::changeVisibility()
{
	isVisible = !isVisible;
}


bool Game::Bird::getVisibility()
{
	return isVisible;
}

void Game::Bird::gain(INT val)
{

}

void Game::Bird::drop(INT val)
{

}

INT Game::Bird::getX()
{
	return posxBird;
}

INT Game::Bird::getY()
{
	return posyBird;
}

IMAGE &Game::Bird::operator[](INT index)
{
	return imgBird[index];
}



void Game::subGame()
{
	*logger << L"窗口句柄：0x" << hWnd << logger->endl;

	// Initialize background picture object
	*logger << L"初始化背景图层(BMP, IMAGE)……" << logger->endl;
	loadimage(&(BG.im), L"IMAGE", L"IDR_IMAGE_BG");
	BG.posx = BG.posy = 0;
	BG.dwRop = SRCCOPY;
	lBG.push_back(&BG);
	
	// Initlialize groud picture object
	// Total number: 22
	// Total width: 22 * 37 px = 814 px
	// Screen width: 768 px
	// Difference: 46 px (MAX_POSX_GND)
	loadimage(&(Ground.im), L"IMAGE", L"IDR_IMAGE_GND");
	Ground.posx = 0;
	Ground.posy = BG.im.getheight();
	Ground.dwRop = SRCCOPY;
	lBG.push_back(&Ground);

	mainScene.push_back(lBG);

	// Initlialize scoreboard layer
	*logger << L"初始化结算界面图层(PNG, CImage)……" << logger->endl;

	SB.cim.Load(GetPNGStreamW(L"IDR_PNG_SCOREBOARD", L"IMAGE"));
	SB.posx = 293;
	SB.posy = 240;

	bRestart.cim.Load(GetPNGStreamW(L"IDR_PNG_RESTART", L"IMAGE"));
	bRestart.posx = 275;
	bRestart.posy = 550;

	// Initialize the picture object of pipes
	*logger << L"初始化管道图层对象(BMP, IMAGE, Sealed)……" << logger->endl;

	// Initialize the picture object of the Bird
	*logger << L"初始化Bird对象(BMP, IMAGE, Sealed)……" << logger->endl;

	Bird bird
		(
			L"IDR_IMAGE_BIRD1", L"IDR_IMAGE_BIRD1_M",
			L"IDR_IMAGE_BIRD2", L"IDR_IMAGE_BIRD2_M",
			L"IDR_IMAGE_BIRD3", L"IDR_IMAGE_BIRD3_M",
			L"IMAGE"
		);

	// Initialize font resource
	*logger << L"初始化字体资源……" << logger->endl;
	*logger << L"TTF名称：" << lpFontName << logger->endl;
	HANDLE DefFont = GetFontHandleW(L"IDR_TTF_DEFAULT", L"TTF");
	*logger << L"TTF资源句柄：0x" << DefFont << logger->endl;
	if (NULL == DefFont)
		throw stdWCexception(L"TTF资源句柄无效！");


	// Initialize MUTEX
	*logger << L"正在创建互斥锁（异步刷新线程）……" << logger->endl;
	hMutRef = CreateMutexW(NULL, FALSE, L"MutexRefresh");
	*logger << L"互斥锁句柄：0x" << hMutRef << logger->endl;
	if (NULL == hMutRef)
		throw stdWCexception(L"无效互斥锁句柄！");

	*logger << L"正在创建互斥锁（动画计算线程）……" << logger->endl;
	hMutAni = CreateMutexW(NULL, FALSE, L"MutexAnimation");
	*logger << L"互斥锁句柄：0x" << hMutAni << logger->endl;
	if (NULL == hMutAni)
		throw stdWCexception(L"无效互斥锁句柄！");

	// Initialize refresh thread
	*logger << L"正在创建异步刷新线程……" << logger->endl;
	HANDLE hThRef = CreateThread(NULL, 0, refreshLoop, hMutRef, 0, NULL);
	*logger << L"线程句柄：0x" << hThRef << logger->endl;

	// Initialize animation thread
	*logger << L"正在创建动画计算线程……" << logger->endl;
	HANDLE hThAni = CreateThread(NULL, 0, animationLoop, hMutAni, 0, NULL);
	*logger << L"线程句柄：0x" << hThAni << logger->endl;

	// Initlialize keyboard event listening thread
	*logger << L"正在创建键盘事件处理线程……" << logger->endl;
	HANDLE hThKBEHandler = CreateThread(NULL, 0, KBELoop, NULL, 0, NULL);
	*logger << L"线程句柄：0x" << hThKBEHandler << logger->endl;

	// Initlialize keyboard event listening thread
	*logger << L"正在创建鼠标事件转换线程……" << logger->endl;
	HANDLE hThMSEHandler = CreateThread(NULL, 0, MSELoop, NULL, 0, NULL);
	*logger << L"线程句柄：0x" << hThMSEHandler << logger->endl;


	*logger << L"等待互斥锁空闲……" << logger->endl;
	WaitForSingleObject(hMutAni, INFINITE);

	*logger << L"尝试锁定互斥锁……" << logger->endl;
	OpenMutexW(SYNCHRONIZE, FALSE, L"MutexAnimation");

	*logger << L"等待互斥锁空闲……" << logger->endl;
	WaitForSingleObject(hMutRef, INFINITE);

	*logger << L"尝试锁定互斥锁……" << logger->endl;
	OpenMutexW(SYNCHRONIZE, FALSE, L"MutexRefresh");


	// Main loop start

	for (; ; )
	{
		static char c = '\0';

		printBG();
		printGameTitle();
		printGameStartHint();
		bird.changeVisibility();

		*logger << L"等待用户开始信号……" << logger->endl;
		c = waitKBEvent();
		if (c == 0x1b)
		{
			*logger << L"退出动作捕获，执行退出指令" << logger->endl;
			break;
		}

		// Clear texts
		*logger << L"释放异步刷新线程" << logger->endl;
		ReleaseMutex(hMutRef);

		*logger << L"释放动画计算线程" << logger->endl;
		ReleaseMutex(hMutAni);


		// Countdown
		WaitForSingleObject(hMutRef, INFINITE);
		OpenMutexW(SYNCHRONIZE, FALSE, L"MutexRefresh");
		fxLayers.push_back(printCountdown);
		ReleaseMutex(hMutRef);
		*logger << L"倒计时……" << logger->endl;
		for (cntdwnChar = L'3'; cntdwnChar > L'0'; --cntdwnChar)
		{
			*logger << cntdwnChar << logger->endl;
			Sleep(1000);
		}
		fxLayers.clear();

		// Battle control online :P
		*logger << L"清空键盘事件队列" << logger->endl;
		KBEMsgQueue.empty();
		fxLayers.clear();

		*logger << L"插入分数显示函数图层" << logger->endl;
		WaitForSingleObject(hMutRef, INFINITE);
		OpenMutexW(SYNCHRONIZE, FALSE, L"MutexRefresh");
		fxLayers.push_back(printScore);
		ReleaseMutex(hMutRef);


		// Game start

		for (; ; )
		{
			char c = waitKBEvent();
			if (c == 0x1b)
				break;
		}


		// Game Ends


		// Battle control terminated.
		*logger << L"删除 Bird 图层" << logger->endl;
		bird.changeVisibility();

		*logger << L"删除分数显示函数图层" << logger->endl;
		WaitForSingleObject(hMutRef, INFINITE);
		OpenMutexW(SYNCHRONIZE, FALSE, L"MutexRefresh");
		fxLayers.pop_back();
		ReleaseMutex(hMutRef);

		*logger << L"游戏结束" << logger->endl;
		*logger << L"锁定动画计算线程" << logger->endl;
		WaitForSingleObject(hMutAni, INFINITE);
		OpenMutexW(SYNCHRONIZE, FALSE, L"MutexAnimation");
		*logger << L"锁定异步刷新线程" << logger->endl;
		WaitForSingleObject(hMutRef, INFINITE);
		OpenMutexW(SYNCHRONIZE, FALSE, L"MutexRefresh");

		highscore = score > highscore ? score : highscore;

		*logger << L"显示分数结算界面" << logger->endl;

		printGameOver();
		SB.cim.Draw(GetDC(hWnd), SB.posx, SB.posy);;
		bRestart.cim.Draw(GetDC(hWnd), bRestart.posx, bRestart.posy);
		printEndScore();
		printEndHighScore();

		waitKBEvent();
	}

	
	// Main loop ends

	*logger << L"游戏退出中……" << logger->endl;

	*logger << L"中止键盘事件处理线程……" << logger->endl;
	TerminateThread(hThKBEHandler, 0);
	*logger << L"关闭键盘事件处理句柄……" << logger->endl;
	CloseHandle(hThKBEHandler);

	*logger << L"中止动画计算线程……" << logger->endl;
	TerminateThread(hThAni, 0);
	*logger << L"关闭动画计算线程句柄……" << logger->endl;
	CloseHandle(hThAni);

	*logger << L"中止异步刷新线程……" << logger->endl;
	TerminateThread(hThRef, 0);
	*logger << L"关闭异步刷新线程句柄……" << logger->endl;
	CloseHandle(hThRef);

	*logger << L"关闭图形界面……" << logger->endl;
	closegraph();
}

void Game::printBG()
{
	putimage(BG.posx, BG.posy, &BG.im, BG.dwRop);
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
	outtextxy(180, 200, L"Flappy Bird");
}


void Game::printGameStartHint()
{
	static bool firstrun = true;
	static LOGFONTW LogFontDef = { 0 };
	if (firstrun)
	{
		LogFontDef.lfHeight = 32;
		wcsncpy_s(LogFontDef.lfFaceName, Game::lpFontName, sizeof (LogFontDef.lfFaceName) / sizeof (WCHAR));
		firstrun = false;
	}

	settextstyle(&LogFontDef);
	setbkmode(TRANSPARENT);
	outtextxy(190, 600, L"Press any key to start");
}


void Game::printCountdown()
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
	outtextxy(365, 100, cntdwnChar);
}


void Game::printScore()
{
	static bool firstrun = true;
	static LOGFONTW LogFontDef = { 0 };
	static WCHAR wstrScore[11];
	if (firstrun)
	{
		LogFontDef.lfHeight = 72;
		wcsncpy_s(LogFontDef.lfFaceName, Game::lpFontName, sizeof(LogFontDef.lfFaceName) / sizeof(WCHAR));
		firstrun = false;
	}

	settextstyle(&LogFontDef);
	setbkmode(TRANSPARENT);
	_itow_s(score, wstrScore, 10);
	outtextxy(350, 100, wstrScore);
}

void Game::printGameOver()
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
	outtextxy(195, 100, L"Game Over");
}

void Game::printEndScore()
{
	static bool firstrun = true;
	static LOGFONTW LogFontDef = { 0 };
	static WCHAR wstrScore[11];
	if (firstrun)
	{
		LogFontDef.lfHeight = 48;
		wcsncpy_s(LogFontDef.lfFaceName, Game::lpFontName, sizeof(LogFontDef.lfFaceName) / sizeof(WCHAR));
		firstrun = false;
	}

	settextstyle(&LogFontDef);
	setbkmode(TRANSPARENT);
	_itow_s(score, wstrScore, 10);
	outtextxy(353, 305, wstrScore);
}

void Game::printEndHighScore()
{
	static bool firstrun = true;
	static LOGFONTW LogFontDef = { 0 };
	static WCHAR wstrHScore[11];
	if (firstrun)
	{
		LogFontDef.lfHeight = 48;
		wcsncpy_s(LogFontDef.lfFaceName, Game::lpFontName, sizeof(LogFontDef.lfFaceName) / sizeof(WCHAR));
		firstrun = false;
	}

	settextstyle(&LogFontDef);
	setbkmode(TRANSPARENT);
	_itow_s(highscore, wstrHScore, 10);
	outtextxy(353, 390, wstrHScore);
}

void Game::postKBEvent(KBE event)
{
	KBEMsgQueue.push(event);
}

void Game::stimulate()
{
	lockBird = true;

	pBird->gain(birdGain);

	lockBird = false;
}


Game::KBE Game::asyncGetKBEvent()
{
	static KBE event = '\0';

	if (KBEMsgQueue.size())
	{
		event = KBEMsgQueue.front();
		KBEMsgQueue.pop();
	}
	return event;
}

Game::KBE Game::waitKBEvent()
{
	static KBE event = '\0';

	while (!KBEMsgQueue.size());

	event = KBEMsgQueue.front();
	KBEMsgQueue.pop();

	return event;
}

HWND Game::createEXWindow(const _In_ int width, const _In_ int height, const _In_ bool isWindowShow)
{
	HWND hWnd;
	if (isWindowShow)
		hWnd = initgraph(width, height, SHOWCONSOLE);
	else
		hWnd = initgraph(width, height);

	if (NULL == hWnd)
		throw stdWCexception(L"无法创建窗口");

	return hWnd;
}


DWORD WINAPI Game::refreshLoop(LPVOID lpParam)
{
	HDC hDC;

	for (; ; )
	{
		WaitForSingleObject((HANDLE *)lpParam, INFINITE);
		OpenMutexW(SYNCHRONIZE, FALSE, L"MutexRefresh");

		BeginBatchDraw();

		// For regular IMAGE
		for (int i = 0; i < mainScene.size(); ++i)
			for (int j = 0; j < mainScene[i].size(); ++j)
				if (NULL != mainScene[i][j])
					putimage(mainScene[i][j]->posx, mainScene[i][j]->posy, &(mainScene[i][j]->im), mainScene[i][j]->dwRop);
		

		// For class Bird
		if (pBird->getVisibility())
			pBird->draw();

		// For Function layers
		for (int i = 0; i < fxLayers.size(); ++i)
			if (NULL != fxLayers[i])
				fxLayers[i]();

		EndBatchDraw();

		ReleaseMutex((HANDLE *)lpParam);
	}
	return 0;
}

DWORD WINAPI Game::KBELoop(LPVOID lpParam)
{
	static char c;
	static UCHAR asc;
	
	for (; ; )
	{
		c = _getch();
		asc = c;
		*logger << L"捕获到键盘事件：" << logger->endl;
		*logger << L"16进制机内码为：" << L"0x" << std::hex << asc << logger->endl;
		*logger << L"对应字符为：" << c << logger->endl;
		postKBEvent(c);
	}
	return 0;
}

DWORD WINAPI Game::MSELoop(LPVOID lpParam)
{
	static MOUSEMSG MouseMsg;
	for (; ; )
	{
		MouseMsg = GetMouseMsg();
		if (MouseMsg.uMsg == WM_LBUTTONDOWN)
		{
			while (GetMouseMsg().uMsg != WM_LBUTTONUP);
			postKBEvent('\n');
		}
	}
}


DWORD WINAPI Game::animationLoop(LPVOID lpParam)
{
	for (UINT iSync = 0; ; ++iSync)
	{
		WaitForSingleObject((HANDLE *)lpParam, INFINITE);
		OpenMutexW(SYNCHRONIZE, FALSE, L"MutexAnimation");
		{
			Ground.posx -= 1;			// Flying animation
			if (Ground.posx < -36)
				Ground.posx = 0;

			if (!(iSync % 30))
				pBird->changeState();

			if (!lockBird)
			{

			}

			if (!lockPipe)
			{

			}
		}
		ReleaseMutex((HANDLE *)lpParam);
		Sleep(5);
	}
}



DWORD WINAPI Game::judgeLoop(LPVOID lpParam)
{
	for (; ; )
	{

	}
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

LPSTREAM Game::GetPNGStreamW(const LPCWSTR lpResID, const LPCWSTR lpResType)
{
	HRSRC hResource = FindResourceW(NULL, lpResID, lpResType);
	if (NULL == hResource)
		throw stdWCexception(L"无法获取PNG图像资源！");

	HGLOBAL hGlobal = LoadResource(NULL, hResource);
	if (NULL == hGlobal)
		throw stdWCexception(L"无法装载PNG图像资源！");

	LPVOID lpRawPNG = LockResource(hGlobal);
	if (NULL == lpRawPNG)
		throw stdWCexception(L"PNG图像资源无效！");

	LPSTREAM lpStream = NULL;

	DWORD dwSize = SizeofResource(NULL, hResource);
	HGLOBAL hGlNew = GlobalAlloc(GHND, dwSize);
	if (NULL == hGlNew)
		throw stdWCexception(L"全局堆内存申请失败！");
	LPBYTE lpByte = (LPBYTE)GlobalLock(hGlNew);
	if (NULL == lpByte)
		throw stdWCexception(L"全局堆内存锁定失败！");
	memcpy(lpByte, lpRawPNG, dwSize);
	GlobalUnlock(hGlNew);

	HRESULT hResult = CreateStreamOnHGlobal(hGlNew, TRUE, &lpStream);
	if (hResult != S_OK && lpStream == NULL)
	{
		GlobalFree(hGlNew);
		throw stdWCexception(L"创建PNG流失败！");
	}
	else
		return lpStream;
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

