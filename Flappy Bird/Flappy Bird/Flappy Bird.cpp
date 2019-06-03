// Flappy Bird.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Flappy Bird.hpp"
#include "showHelp.hpp"

#define sndPlaySoundW(X, Y)

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
		Game::hWnd = Game::createEXWindow(Game::WNDWIDTH, Game::WNDHEIGHT, cmdLineCfg::isDebugMode);

		// Disabling minimize button
		LONG oldstyle = GetWindowLongW(Game::hWnd, GWL_STYLE);
		SetWindowLongW(Game::hWnd, GWL_STYLE, oldstyle & ~WS_MINIMIZEBOX);
		
		// Always on top
		SetWindowPos(Game::hWnd, HWND_TOPMOST, 0, 0, Game::WNDWIDTH, Game::WNDHEIGHT, 0);
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


// Class Hint

Game::Hint::Hint() {}

Game::Hint::Hint
(
	const LPCWSTR pResName1, const LPCWSTR pResName1_m,
	const LPCWSTR pResType
) throw(...)
{
	init
	(
		pResName1, pResName1_m,
		pResType
	);
}

void Game::Hint::init
(
	const LPCWSTR pResName1, const LPCWSTR pResName1_m,
	const LPCWSTR pResType
) throw(...)
{
	loadimage(imgHint, pResType, pResName1_m);
	loadimage(imgHint, pResType, pResName1_m, INT(imgHint->getwidth() * hintSZMultiplier), INT(imgHint->getheight() * hintSZMultiplier), true);
	if (!(imgHint[0].getwidth() && imgHint[0].getheight()))
	{
		throw stdWCexception(L"Hint1 mask 资源加载失败！");
	}

	loadimage(imgHint + 1, pResType, pResName1);
	loadimage(imgHint + 1, pResType, pResName1, INT(imgHint[1].getwidth() * hintSZMultiplier), INT(imgHint[1].getheight() * hintSZMultiplier), true);
	if (!(imgHint[1].getwidth() && imgHint[1].getheight()))
	{
		throw stdWCexception(L"Hint1 资源加载失败！");
	}
	posxHint = (WNDWIDTH - imgHint[0].getwidth()) / 2;
}

void Game::Hint::draw()
{
	if (isVisible)
	{
		putimage(posxHint, posyHint, imgHint, SRCAND);
		putimage(posxHint, posyHint, imgHint + 1, SRCPAINT);
	}
}

void Game::Hint::changeVisibility()
{
	isVisible = !isVisible;
}

bool Game::Hint::getVisibility()
{
	return isVisible;
}

INT Game::Hint::getX()
{
	return posxHint;
}

INT Game::Hint::getY()
{
	return posyHint;
}

IMAGE &Game::Hint::operator[](INT index)
{
	return imgHint[index];
}

// Class Bird

Game::Bird::Bird() {}

Game::Bird::Bird
(
	const LPCWSTR pResName1, const LPCWSTR pResName1_m,
	const LPCWSTR pResName2, const LPCWSTR pResName2_m,
	const LPCWSTR pResName3, const LPCWSTR pResName3_m,
	const LPCWSTR pResType
) throw(...)
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
	const LPCWSTR pResName1, const LPCWSTR pResName1_m,
	const LPCWSTR pResName2, const LPCWSTR pResName2_m,
	const LPCWSTR pResName3, const LPCWSTR pResName3_m,
	const LPCWSTR pResType
) throw(...)
{
	loadimage(imgBird, pResType, pResName1_m);
	if (!(imgBird[0].getwidth() && imgBird[0].getheight()))
	{
		throw stdWCexception(L"Bird1 mask 资源加载失败！");
	}

	loadimage(imgBird + 1, pResType, pResName1);
	if (!(imgBird[1].getwidth() && imgBird[1].getheight()))
	{
		throw stdWCexception(L"Bird1 资源加载失败！");
	}

	loadimage(imgBird + 2, pResType, pResName2_m);
	if (!(imgBird[2].getwidth() && imgBird[2].getheight()))
	{
		throw stdWCexception(L"Bird2 mask 资源加载失败！");
	}
	loadimage(imgBird + 3, pResType, pResName2);
	if (!(imgBird[3].getwidth() && imgBird[3].getheight()))
	{
		throw stdWCexception(L"Bird2 资源加载失败！");
	}

	loadimage(imgBird + 4, pResType, pResName3_m);
	if (!(imgBird[4].getwidth() && imgBird[4].getheight()))
	{
		throw stdWCexception(L"Bird3 mask 资源加载失败！");
	}
	loadimage(imgBird + 5, pResType, pResName3);
	if (!(imgBird[5].getwidth() && imgBird[5].getheight()))
	{
		throw stdWCexception(L"Bird3 资源加载失败！");
	}

	posxBird = (WNDWIDTH - imgBird[0].getwidth()) / 2;
}

void Game::Bird::draw()
{
	if (isVisible)
	{
		birdState %= 3;
		putimage(posxBird, posyBird, imgBird + birdState * 2, SRCAND);
		putimage(posxBird, posyBird, imgBird + birdState * 2 + 1, SRCPAINT);
	}
}

void Game::Bird::changeState()
{
	if (++birdState > 2)
		birdState = 0;
}

void Game::Bird::changeState(INT state)
{
	birdState = state;
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
	posyBird += val;
}

void Game::Bird::setX(INT pos)
{
	posxBird = pos;
}

void Game::Bird::setY(INT pos)
{
	posyBird = pos;
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

// Class Pipe

Game::Pipe::Pipe() {}
Game::Pipe::Pipe
(
	const LPCWSTR pResName1, const LPCWSTR pResName1_m,
	const LPCWSTR pResName2, const LPCWSTR pResName2_m,
	const LPCWSTR pResType
) throw(...)
{
	init
	(
		pResName1, pResName1_m,
		pResName2, pResName2_m,
		pResType
	);
}

void Game::Pipe::init
(
	const LPCWSTR pResName1, const LPCWSTR pResName1_m,
	const LPCWSTR pResName2, const LPCWSTR pResName2_m,
	const LPCWSTR pResType
) throw(...)
{
	loadimage(imgPipe, pResType, pResName1_m);
	if (!(imgPipe[0].getwidth() && imgPipe[0].getheight()))
	{
		throw stdWCexception(L"Pipe DN mask 资源加载失败！");
	}

	loadimage(imgPipe + 1, pResType, pResName1);
	if (!(imgPipe[1].getwidth() && imgPipe[1].getheight()))
	{
		throw stdWCexception(L"Pipe DN 资源加载失败！");
	}

	loadimage(imgPipe + 2, pResType, pResName2_m);
	if (!(imgPipe[2].getwidth() && imgPipe[2].getheight()))
	{
		throw stdWCexception(L"Pipe UP mask 资源加载失败！");
	}
	loadimage(imgPipe + 3, pResType, pResName2);
	if (!(imgPipe[3].getwidth() && imgPipe[3].getheight()))
	{
		throw stdWCexception(L"Pipe UP 资源加载失败！");
	}
}

void Game::Pipe::changeVisibility()
{
	isVisible = !isVisible;
}


bool Game::Pipe::getVisibility()
{
	return isVisible;
}

void Game::Pipe::gain(INT val)
{
	posxPipe += val;
}

void Game::Pipe::setX(INT pos)
{
	posxPipe = pos;
}

void Game::Pipe::setYDn(INT pos)
{
	posyPipeDn = pos;
}

INT Game::Pipe::getX()
{
	return posxPipe;
}

INT Game::Pipe::getYDn()
{
	return posyPipeDn;
}

void Game::Pipe::draw()
{
	if (isVisible)
	{
		putimage(posxPipe, posyPipeDn, imgPipe, SRCAND);
		putimage(posxPipe, posyPipeDn, imgPipe + 1, SRCPAINT);
		putimage(posxPipe, posyPipeDn - dPipeVertical - imgPipe[2].getheight(), imgPipe + 2, SRCAND);
		putimage(posxPipe, posyPipeDn - dPipeVertical - imgPipe[3].getheight(), imgPipe + 3, SRCPAINT);
	}
}

IMAGE &Game::Pipe::operator[](INT index)
{
	return imgPipe[index];
}


// Class Medal
Game::Medal::Medal() {}

Game::Medal::Medal
(
	const LPCWSTR pResName1, const LPCWSTR pResName2,
	const LPCWSTR pResName3, const LPCWSTR pResName4,
	const LPCWSTR pResType, const OBJCIMG &ScoreBoard,
	const HWND hWnd
) throw(...)
{
	init
	(
		pResName1, pResName2,
		pResName3, pResName4,
		pResType, ScoreBoard,
		hWnd
	);
}

void Game::Medal::init
(
	const LPCWSTR pResName1, const LPCWSTR pResName2,
	const LPCWSTR pResName3, const LPCWSTR pResName4,
	const LPCWSTR pResType, const OBJCIMG &ScoreBoard,
	const HWND hWnd
) throw(...)
{
	if (NULL == hWnd)
		throw stdWCexception(L"窗口句柄无效");
	else if (!(ScoreBoard.cim.GetHeight() && ScoreBoard.cim.GetWidth()))
		throw stdWCexception(L"计分板图层未正确初始化");
	else
	{
		imgMedal[0].Load(GetPNGStreamW(pResName1, pResType));
		if (imgMedal[0].IsNull())
			throw stdWCexception(L"Bronze 奖牌 资源无法加载");
		imgMedal[1].Load(GetPNGStreamW(pResName2, pResType));
		if (imgMedal[1].IsNull())
			throw stdWCexception(L"Silver 奖牌 资源无法加载");
		imgMedal[2].Load(GetPNGStreamW(pResName3, pResType));
		if (imgMedal[2].IsNull())
			throw stdWCexception(L"Golden 奖牌 资源无法加载");
		imgMedal[3].Load(GetPNGStreamW(pResName4, pResType));
		if (imgMedal[3].IsNull())
			throw stdWCexception(L"Platinum 奖牌 资源无法加载");
	}
	Medal::hWnd = hWnd;
}

void Game::Medal::draw() throw(...)
{
	if (NULL == hWnd)
		throw stdWCexception(L"窗口句柄无效");
	else if (MedalState %= 5)
		imgMedal[MedalState - 1].Draw
		(
			GetDC(hWnd),
			posxMedal, posyMedal,
			INT(imgMedal[MedalState - 1].GetWidth() * ScoreboardSZMultiplier),
			INT(imgMedal[MedalState - 1].GetHeight() * ScoreboardSZMultiplier)
		);
}

void Game::Medal::changeState(INT state)
{
	MedalState = state;
}

INT Game::Medal::getState()
{
	return MedalState;
}

void Game::Medal::changeWindowHandle(HWND hWnd) throw(...)
{
	if (NULL == hWnd)
		throw stdWCexception(L"窗口句柄无效");
	else
		Medal::hWnd = hWnd;
}

void Game::Medal::setX(INT pos)
{
	posxMedal = pos;
}

INT Game::Medal::getX()
{
	return posxMedal;
}

INT Game::Medal::getY()
{
	return posyMedal;
}

CImage &Game::Medal::operator[](INT index)
{
	return imgMedal[index];
}


void Game::subGame() throw(...)
{
	*logger << L"窗口句柄：0x" << hWnd << logger->endl;

	// Initlialize background layer
	*logger << L"初始化背景图层(BMP, IMAGE)……" << logger->endl;
	loadimage(&(BG.im), L"IMAGE", L"IDR_IMAGE_BG");
	BG.posx = BG.posy = 0;
	BG.dwRop = SRCCOPY;
	lBG.push_back(&BG);
	mainScene.push_back(lBG);

	// Initlialize groud picture object
	*logger << L"初始化地面图层(BMP, IMAGE)……" << logger->endl;
	loadimage(&(Ground.im), L"IMAGE", L"IDR_IMAGE_GND");
	Ground.posx = 0;
	Ground.posy = BG.im.getheight();
	Ground.dwRop = SRCCOPY;

	// Initlialize game logo
	*logger << L"初始化Logo图层(PNG, CImage)……" << logger->endl;
	Logo.cim.Load(GetPNGStreamW(L"IDR_PNG_LOGO", L"IMAGE"));
	Logo.posx = INT((WNDWIDTH - Logo.cim.GetWidth() * logoSZMultiplier)) / 2;
	Logo.posy = logoPosY;

	// Initialize hint layer
	*logger << L"初始化Hint图层(BMP, IMAGE, Sealed)……" << logger->endl;
	Hint hint
	(
		L"IDR_IMAGE_HINT", L"IDR_IMAGE_HINT_M",
		L"IMAGE"
	);
	pHint = &hint;

	// Initialize the picture object of pipes
	*logger << L"初始化管道图层对象(BMP, IMAGE, Sealed, " << PipeObjNum << L")……" << logger->endl;
	Pipe pipe[PipeObjNum];

	for (INT i = 0; i < PipeObjNum; ++i)
	{
		pipe[i].init
		(
			L"IDR_IMAGE_PIPE_DN", L"IDR_IMAGE_PIPE_DN_M",
			L"IDR_IMAGE_PIPE_UP", L"IDR_IMAGE_PIPE_UP_M",
			L"IMAGE"
		);
	}
	pPipe = pipe;

	// Initialize the picture object of the Bird
	*logger << L"初始化Bird对象(BMP, IMAGE, Sealed)……" << logger->endl;

	Bird bird
	(
		L"IDR_IMAGE_BIRD1", L"IDR_IMAGE_BIRD1_M",
		L"IDR_IMAGE_BIRD2", L"IDR_IMAGE_BIRD2_M",
		L"IDR_IMAGE_BIRD3", L"IDR_IMAGE_BIRD3_M",
		L"IMAGE"
	);
	pBird = &bird;

	// Inilialize game over mark layer
	*logger << L"初始化游戏结束标记图层(PNG, CImage)……" << logger->endl;
	GameOver.cim.Load(GetPNGStreamW(L"IDR_PNG_GAMEOVER", L"IMAGE"));
	GameOver.posx = INT((WNDWIDTH - GameOver.cim.GetWidth() * GameOverBannerSZMultiplier)) / 2;
	GameOver.posy = GameOverBannerPosY;

	// Initlialize scoreboard layer
	*logger << L"初始化计分板图层(PNG, CImage)……" << logger->endl;
	Scoreboard.cim.Load(GetPNGStreamW(L"IDR_PNG_SCOREBOARD", L"IMAGE"));
	Scoreboard.posx = INT((WNDWIDTH - Scoreboard.cim.GetWidth() * ScoreboardSZMultiplier)) / 2;
	Scoreboard.posy = ScoreboardPosY;

	// Initlialize medal layer
	*logger << L"初始化奖牌图层(PNG, CImage, Sealed)……" << logger->endl;
	Medal medal
	(
		L"IDR_PNG_BRONZE_MEDAL", L"IDR_PNG_SILVER_MEDAL",
		L"IDR_PNG_GOLDEN_MEDAL", L"IDR_PNG_PT_MEDAL",
		L"IMAGE", Scoreboard, hWnd
	);
	pMedal = &medal;
	medal.setX(INT(Scoreboard.posx + 27 * ScoreboardSZMultiplier));

	// Initlilize highscore mark layer
	*logger << L"初始化高分标记图层(PNG, CImage)……" << logger->endl;
	Highscore.cim.Load(GetPNGStreamW(L"IDR_PNG_HIGHSCORE", L"IMAGE"));
	Highscore.posx = INT(Scoreboard.posx + 140 * ScoreboardSZMultiplier);
	Highscore.posy = INT(ScoreboardPosY + 57 * ScoreboardSZMultiplier);

	// Initlialize replay button layer
	*logger << L"初始化重玩按钮图层(PNG, CImage)……" << logger->endl;
	bRestart.cim.Load(GetPNGStreamW(L"IDR_PNG_RESTART", L"IMAGE"));
	bRestart.posx = (WNDWIDTH - bRestart.cim.GetWidth()) / 2;
	bRestart.posy = RestartButtonPosY;

	// Initialize font resource
	*logger << L"初始化默认字体资源……" << logger->endl;
	*logger << L"TTF名称：" << lpFontName << logger->endl;
	HANDLE DefFont = GetFontHandleW(L"IDR_RFONT_DEFAULT", L"RFONT");
	*logger << L"TTF资源句柄：0x" << DefFont << logger->endl;
	if (NULL == DefFont)
		throw stdWCexception(L"TTF资源句柄无效！");

	// Initialize sound fx
	*logger << L"初始化音效资源（翅膀扇动）……" << logger->endl;
	lpWAVWing = GetRawWAVBufferW(L"IDR_AUDIO_WING", L"AUDIO");
	*logger << L"资源指针：0x" << lpWAVWing << logger->endl;

	*logger << L"初始化音效资源（撞击）……" << logger->endl;
	lpWAVHit = GetRawWAVBufferW(L"IDR_AUDIO_HIT", L"AUDIO");
	*logger << L"资源指针：0x" << lpWAVHit << logger->endl;

	*logger << L"初始化音效资源（死亡）……" << logger->endl;
	lpWAVDie = GetRawWAVBufferW(L"IDR_AUDIO_DIE", L"AUDIO");
	*logger << L"资源指针：0x" << lpWAVDie << logger->endl;

	*logger << L"初始化音效资源（得分）……" << logger->endl;
	lpWAVPoint = GetRawWAVBufferW(L"IDR_AUDIO_POINT", L"AUDIO");
	*logger << L"资源指针：0x" << lpWAVPoint << logger->endl;


	// Initialize MUTEX
	*logger << L"正在创建互斥锁（异步刷新线程）……" << logger->endl;
	HANDLE hMutRef = CreateMutexW(NULL, FALSE, CWCStrMutexRef);
	*logger << L"互斥锁句柄：0x" << hMutRef << logger->endl;
	if (NULL == hMutRef)
		throw stdWCexception(L"无效互斥锁句柄！");

	*logger << L"正在创建互斥锁（动画计算线程，地面，已冻结）……" << logger->endl;
	HANDLE hMutGNDAni = CreateMutexW(NULL, TRUE, CWCStrMutexGNDAni);
	*logger << L"互斥锁句柄：0x" << hMutGNDAni << logger->endl;
	if (NULL == hMutGNDAni)
		throw stdWCexception(L"无效互斥锁句柄！");

	*logger << L"正在创建互斥锁（动画计算线程，Bird，已冻结）……" << logger->endl;
	HANDLE hMutBirdAni = CreateMutexW(NULL, TRUE, CWCStrMutexBird);
	*logger << L"互斥锁句柄：0x" << hMutBirdAni << logger->endl;
	if (NULL == hMutBirdAni)
		throw stdWCexception(L"无效互斥锁句柄！");

	// Initialize refresh thread
	*logger << L"正在创建异步刷新线程……" << logger->endl;
	HANDLE hThRef = CreateThread(NULL, 0, refreshLoop, hMutRef, 0, NULL);
	*logger << L"线程句柄：0x" << hThRef << logger->endl;

	// Initialize animation thread
	*logger << L"正在创建动画计算线程（地面）……" << logger->endl;
	HANDLE hThGNDAni = CreateThread(NULL, 0, GNDAnimationLoop, hMutGNDAni, 0, NULL);
	*logger << L"线程句柄：0x" << hThGNDAni << logger->endl;

	*logger << L"正在创建动画计算线程（Bird）……" << logger->endl;
	HANDLE hThBirdAni = CreateThread(NULL, 0, BirdAnimationLoop, hMutBirdAni, 0, NULL);
	*logger << L"线程句柄：0x" << hThBirdAni << logger->endl;


	// Initialize keyboard event listening thread
	*logger << L"正在创建键盘事件处理线程……" << logger->endl;
	HANDLE hThKBEHandler = CreateThread(NULL, 0, KBELoop, NULL, 0, NULL);
	*logger << L"线程句柄：0x" << hThKBEHandler << logger->endl;

	// Initialize keyboard event listening thread
	*logger << L"正在创建鼠标事件转换线程……" << logger->endl;
	HANDLE hThMSEHandler = CreateThread(NULL, 0, MSELoop, NULL, 0, NULL);
	*logger << L"线程句柄：0x" << hThMSEHandler << logger->endl;

	*logger << L"等待互斥锁空闲……" << logger->endl;
	WaitForSingleObject(hMutRef, INFINITE);

	*logger << L"尝试锁定互斥锁……" << logger->endl;
	OpenMutexW(SYNCHRONIZE, FALSE, L"MutexRefresh");


	// Main loop start

	for (; ; )
	{
		static CHAR c = '\0';

		*logger << L"绘制背景……" << logger->endl;
		printBG();
		*logger << L"绘制Logo……" << logger->endl;
		Logo.cim.Draw(GetDC(hWnd), Logo.posx, Logo.posy, INT(Logo.cim.GetWidth() * logoSZMultiplier), INT(Logo.cim.GetHeight() * logoSZMultiplier));
		*logger << L"绘制游戏开始提示……" << logger->endl;
		printGameStartHint();

		*logger << L"等待用户开始信号……" << logger->endl;
		c = waitKBEvent();
		if (c == 0x1b)
		{
			*logger << L"退出动作捕获，执行退出指令……" << logger->endl;
			break;
		}

		*logger << L"设定初始数据……" << logger->endl;
		isGrounded = false;
		downSpeed = defDownSpeed;
		score = 0;

		*logger << L"显示Hint图层……" << logger->endl;
		hint.changeVisibility();

		*logger << L"设置显示管道初始位置及显示图层……" << logger->endl;
		for (INT i = 0; i < PipeObjNum; ++i)
		{
			pipe[i].setX(BG.im.getwidth() + i * dPipeHorizontal);
			pipe[i].setYDn(rangePipeDn(rand));
			pipe[i].changeVisibility();
		}
		*logger << L"锁定管道动作……" << logger->endl;
		lockPipe = true;

		*logger << L"设置显示Bird初始位置及显示图层……" << logger->endl;
		bird.setY(birdDefPosY);
		bird.changeVisibility();

		*logger << L"锁定Bird动作……" << logger->endl;
		lockBird = true;


		// Clear texts
		*logger << L"释放异步刷新线程……" << logger->endl;
		ReleaseMutex(hMutRef);

		*logger << L"释放动画计算线程……" << logger->endl;
		ReleaseMutex(hMutGNDAni);
		ReleaseMutex(hMutBirdAni);

		// Countdown
		*logger << L"插入倒计时图层……" << logger->endl;
		WaitForSingleObject(hMutRef, INFINITE);
		OpenMutexW(SYNCHRONIZE, FALSE, CWCStrMutexRef);
		fxLayers.push_back(printCountdown);
		ReleaseMutex(hMutRef);

		*logger << L"倒计时……" << logger->endl;
		for (cntdwnChar = L'3'; cntdwnChar > L'0'; --cntdwnChar)
		{
			*logger << cntdwnChar << logger->endl;
			Sleep(500);
		}

		*logger << L"隐藏Hint图层……" << logger->endl;
		hint.changeVisibility();
		*logger << L"删除倒计时图层……" << logger->endl;
		fxLayers.clear();

		*logger << L"插入分数显示函数图层……" << logger->endl;
		WaitForSingleObject(hMutRef, INFINITE);
		OpenMutexW(SYNCHRONIZE, FALSE, CWCStrMutexRef);
		fxLayers.push_back(printScore);
		ReleaseMutex(hMutRef);

		*logger << L"解锁Bird动作……" << logger->endl;
		lockBird = false;
		bird.changeState(2);
		*logger << L"清空键盘事件队列……" << logger->endl;
		clearQueue(KBEMsgQueue);

		// Battle control online :P
		*logger << L"用户操作限制已解除" << logger->endl;

		// Game start
		
		gameState = true;
		lockPipe = false;
		INT lastPassedPipeNum = -1;

		for (; ; )
		{
			const static INT birdLbound = bird.getX();
			const static INT birdRbound = bird.getX() + bird[0].getwidth();
			const static INT pipeWidth = pipe[0][0].getwidth();
			const static INT birdHeight = bird[0].getheight();

			static INT curPipeX[PipeObjNum] = { 0 };
			static INT curPipeYDn[PipeObjNum] = { 0 };
			static INT curBirdY = 0;

			if (bird.getY() + bird[0].getheight() >= BG.im.getheight() + BG.posy)	// Grounded
			{
				*logger << L"落地判定" << logger->endl;
				downSpeed = 0;
				bird.setY(BG.im.getheight() - bird[0].getheight());
				isGrounded = true;
				break;
			}

			for (INT i = 0; i < PipeObjNum; ++i)
			{
				WaitForSingleObject(hMutGNDAni, INFINITE);
				OpenMutexW(SYNCHRONIZE, FALSE, CWCStrMutexGNDAni);
				curPipeX[i] = (pPipe + i)->getX();
				curPipeYDn[i] = (pPipe + i)->getYDn();
				ReleaseMutex(hMutGNDAni);
			}
			curBirdY = bird.getY();
			
			for (INT i = 0; i < PipeObjNum; ++i)
			{
				if (birdRbound - 11 >= curPipeX[i] && birdLbound + 10 <= curPipeX[i] + pipeWidth)
				{
					if (curBirdY + 5 < curPipeYDn[i] - dPipeVertical || curBirdY + birdHeight - 5 > curPipeYDn[i])		// On no...
					{
						sndPlaySoundW((LPWSTR)lpWAVHit, SND_MEMORY | SND_ASYNC);
						*logger << L"撞击判定" << logger->endl;
						goto Gameover;
					}

					if (birdLbound == curPipeX[i])		// Got one score~
						if (i != lastPassedPipeNum)
						{
							sndPlaySoundW((LPWSTR)lpWAVPoint, SND_MEMORY | SND_ASYNC);
							*logger << L"得分" << logger->endl;
							++score;
							lastPassedPipeNum = i;
						}
				}
			}
		}

		// Game Ends
	Gameover:
		gameState = false;

		// Battle control terminated.
		*logger << L"游戏结束" << logger->endl;

		*logger << L"锁定动画计算线程（地面）……" << logger->endl;
		WaitForSingleObject(hMutGNDAni, INFINITE);
		OpenMutexW(SYNCHRONIZE, FALSE, CWCStrMutexGNDAni);

		if (!isGrounded)
		{
			downSpeed = defDownSpeed;
			sndPlaySoundW((LPWSTR)lpWAVDie, SND_MEMORY | SND_ASYNC);
			while (bird.getY() + bird[0].getheight() <= BG.im.getheight() + BG.posy);
			downSpeed = 0;
			bird.setY(BG.im.getheight() - bird[0].getheight());
		}

		*logger << L"锁定动画计算线程（Bird）……" << logger->endl;
		WaitForSingleObject(hMutBirdAni, INFINITE);
		OpenMutexW(SYNCHRONIZE, FALSE, CWCStrMutexBird);
		Sleep(1000);

		*logger << L"隐藏Bird图层……" << logger->endl;
		bird.changeVisibility();

		*logger << L"隐藏管道图层……" << logger->endl;
		for (INT i = 0; i < PipeObjNum; ++i)
			pipe[i].changeVisibility();

		*logger << L"删除分数显示函数图层……" << logger->endl;
		WaitForSingleObject(hMutRef, INFINITE);
		OpenMutexW(SYNCHRONIZE, FALSE, CWCStrMutexRef);
		fxLayers.pop_back();
		ReleaseMutex(hMutRef);

		*logger << L"锁定异步刷新线程……" << logger->endl;
		WaitForSingleObject(hMutRef, INFINITE);
		OpenMutexW(SYNCHRONIZE, FALSE, CWCStrMutexRef);


		*logger << L"清空键盘事件队列……" << logger->endl;
		clearQueue(KBEMsgQueue);;

		*logger << L"显示分数结算界面……" << logger->endl;
		*logger << L"显示Game Over标题图层……" << logger->endl;
		GameOver.cim.Draw
		(
			GetDC(hWnd),
			GameOver.posx, GameOver.posy,
			INT(GameOver.cim.GetWidth() * GameOverBannerSZMultiplier),
			INT(GameOver.cim.GetHeight() * GameOverBannerSZMultiplier)
		);

		*logger << L"显示计分板标题图层……" << logger->endl;
		Scoreboard.cim.Draw
		(
			GetDC(hWnd),
			Scoreboard.posx, Scoreboard.posy,
			INT(Scoreboard.cim.GetWidth() * ScoreboardSZMultiplier),
			INT(Scoreboard.cim.GetHeight() * ScoreboardSZMultiplier)
		);

		*logger << L"显示奖牌图层……" << logger->endl;
		medal.changeState(score / 10);
		medal.draw();

		*logger << L"显示重玩按钮图层……" << logger->endl;
		bRestart.cim.Draw(GetDC(hWnd), bRestart.posx, bRestart.posy);
		ScoreboardScorePosX = INT(Scoreboard.posx + 160 * ScoreboardSZMultiplier);
		ScoreboardHScorePosX = ScoreboardScorePosX;

		if (score > highscore)
		{
			*logger << L"显示新纪录标记图层……" << logger->endl;
			highscore = score;
			Highscore.cim.Draw(GetDC(hWnd), Highscore.posx, Highscore.posy);
		}

		*logger << L"显示分数……" << logger->endl;
		printEndScore();

		*logger << L"显示最高分……" << logger->endl;
		printEndHighScore();

		*logger << L"清空键盘事件队列" << logger->endl;
		clearQueue(KBEMsgQueue);
		waitKBEvent();
	}

	// Main loop ends

	*logger << L"游戏退出中……" << logger->endl;

	*logger << L"中止键盘事件处理线程……" << logger->endl;
	TerminateThread(hThKBEHandler, 0);
	*logger << L"关闭键盘事件处理句柄……" << logger->endl;
	CloseHandle(hThKBEHandler);

	*logger << L"中止动画计算线程（地面）……" << logger->endl;
	TerminateThread(hThGNDAni, 0);
	*logger << L"关闭动画计算线程句柄（地面）……" << logger->endl;
	CloseHandle(hThGNDAni);

	*logger << L"中止动画计算线程（Bird）……" << logger->endl;
	TerminateThread(hThBirdAni, 0);
	*logger << L"关闭动画计算线程句柄（Bird）……" << logger->endl;
	CloseHandle(hThBirdAni);

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

void Game::printGameStartHint()
{
	static bool firstrun = true;
	static LOGFONTW LogFontDef = { 0 };
	if (firstrun)
	{
		LogFontDef.lfHeight = startftsz;
		wcsncpy_s(LogFontDef.lfFaceName, Game::lpFontName, sizeof(LogFontDef.lfFaceName) / sizeof(WCHAR));
		firstrun = false;
	}

	settextstyle(&LogFontDef);
	setbkmode(TRANSPARENT);
	outtextxy(startHintPosX, startHintPosY, L"Press any key to start");
}


void Game::printCountdown()
{
	static bool firstrun = true;
	static LOGFONTW LogFontDef = { 0 };
	if (firstrun)
	{
		LogFontDef.lfHeight = digitftsz;
		wcsncpy_s(LogFontDef.lfFaceName, Game::lpFontName, sizeof(LogFontDef.lfFaceName) / sizeof(WCHAR));
		firstrun = false;
	}

	settextstyle(&LogFontDef);
	setbkmode(TRANSPARENT);
	outtextxy(digitPosX, digitPosY, cntdwnChar);
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
	outtextxy(digitPosX, digitPosY, wstrScore);
}

void Game::printEndScore()
{
	static bool firstrun = true;
	static LOGFONTW LogFontDef = { 0 };
	static WCHAR wstrScore[11];
	if (firstrun)
	{
		LogFontDef.lfHeight = Scoreboarddigitftsz;
		wcsncpy_s(LogFontDef.lfFaceName, Game::lpFontName, sizeof(LogFontDef.lfFaceName) / sizeof(WCHAR));
		firstrun = false;
	}

	settextstyle(&LogFontDef);
	setbkmode(TRANSPARENT);
	_itow_s(score, wstrScore, 10);
	outtextxy(ScoreboardScorePosX, ScoreboardScorePosY, wstrScore);
}

void Game::printEndHighScore()
{
	static bool firstrun = true;
	static LOGFONTW LogFontDef = { 0 };
	static WCHAR wstrHScore[11];
	if (firstrun)
	{
		LogFontDef.lfHeight = Scoreboarddigitftsz;
		wcsncpy_s(LogFontDef.lfFaceName, Game::lpFontName, sizeof(LogFontDef.lfFaceName) / sizeof(WCHAR));
		firstrun = false;
	}

	settextstyle(&LogFontDef);
	setbkmode(TRANSPARENT);
	_itow_s(highscore, wstrHScore, 10);
	outtextxy(ScoreboardHScorePosX, ScoreboardHScorePosY, wstrHScore);
}

void Game::postKBEvent(CHAR event)
{
	KBEMsgQueue.push(event);
}

CHAR Game::asyncGetKBEvent()
{
	CHAR event = '\0';

	if (!KBEMsgQueue.empty())
	{
		event = KBEMsgQueue.front();
		KBEMsgQueue.pop();
	}
	return event;
}

CHAR Game::waitKBEvent()
{
	static CHAR event = '\0';

	while (KBEMsgQueue.empty());

	event = KBEMsgQueue.front();
	KBEMsgQueue.pop();

	return event;
}

HWND Game::createEXWindow(const _In_ int width, const _In_ int height, const _In_ bool isWindowShow) throw(...)
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
	HDC hDC = NULL;

	for (; ; )
	{
		WaitForSingleObject((HANDLE *)lpParam, INFINITE);
		OpenMutexW(SYNCHRONIZE, FALSE, CWCStrMutexRef);

		BeginBatchDraw();

		// For regular IMAGE
		for (int i = 0; i < mainScene.size(); ++i)
			for (int j = 0; j < mainScene[i].size(); ++j)
				if (NULL != mainScene[i][j])
					putimage(mainScene[i][j]->posx, mainScene[i][j]->posy, &(mainScene[i][j]->im), mainScene[i][j]->dwRop);

		// For class Hint
		pHint->draw();

		// For class Pipe
		for (INT i = 0; i < PipeObjNum; ++i)
			(pPipe + i)->draw();

		// For Ground (Have to cover pipes up)
		putimage(Ground.posx, Ground.posy, &Ground.im, SRCCOPY);

		// For class Bird
		pBird->draw();

		// For Function layers
		for (int i = 0; i < fxLayers.size(); ++i)
			if (NULL != fxLayers[i])
				fxLayers[i]();

		EndBatchDraw();

		ReleaseMutex((HANDLE *)lpParam);
		Sleep(2);
	}
	return 0;
}

DWORD WINAPI Game::KBELoop(LPVOID lpParam)
{
	static char c;
	static INT8 asc;

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
			postKBEvent('\n');
	}
}

DWORD WINAPI Game::BirdAnimationLoop(LPVOID lpParam)
{
	downSpeed = defDownSpeed;
	for (UINT iSync = 0; ; ++iSync)
	{
		WaitForSingleObject((HANDLE *)lpParam, INFINITE);
		OpenMutexW(SYNCHRONIZE, FALSE, CWCStrMutexBird);

		if (lockBird)					// Bird animation (static)
		{
			downSpeed = defDownSpeed;
			if (!(iSync % birdStaticWingPeriod))
				pBird->changeState();

			if (!(iSync % birdStaticFlucPeriod))
				pBird->gain(INT(birdStaticFlucAmplitude * sin(birdStaticFlucAngFreq * iSync)));
		}
		else							// Bird animation (dynamic)		
		{
			if (!(iSync % birdDynamicWingPeriod))
				pBird->changeState();

			if (asyncGetKBEvent() && gameState)
			{
				downSpeed = defDownSpeedUp;
				// pBird->changeState(0);	// Ehhhh...Is there any human-being who can notice that instant change...?
			}
			
			pBird->gain(INT(speedQRatio * downSpeed * downSpeed * (downSpeed < 0 ? -1 : 1) + (downSpeed < 0 ? -1 : 2)));
			downSpeed < 0 ? downSpeed += upSpeedGain : downSpeed += downSpeedGain;
			// pBird->changeState(2);
			Sleep(2);
		}
		ReleaseMutex((HANDLE *)lpParam);
		Sleep(5);
	}
	return 0;

}

DWORD WINAPI Game::GNDAnimationLoop(LPVOID lpParam)
{

	for (; ; )
	{
		WaitForSingleObject((HANDLE *)lpParam, INFINITE);
		OpenMutexW(SYNCHRONIZE, FALSE, CWCStrMutexGNDAni);

		Ground.posx -= 1;			// Going-forward animation
		if (Ground.posx < lBoundGroundImg)
			Ground.posx = 0;


		if (!lockPipe)				// Pipe animation (in sync with ground)
			for (INT i = 0; i < PipeObjNum; ++i)
			{
				if ((pPipe + i)->getX() + (*(pPipe + i))[0].getwidth() < 0)
				{
					rand.seed((UINT)time(NULL));
					(pPipe + i)->setX(BG.im.getwidth() + dPipeHorizontal - (*pPipe)[0].getwidth());
					(pPipe + i)->setYDn(rangePipeDn(rand));
				}
				(pPipe + i)->gain(-1);
			}

		ReleaseMutex((HANDLE *)lpParam);
		Sleep(2);
	}
}

HANDLE Game::GetFontHandleW(const LPCWSTR lpResID, const LPCWSTR lpResType) throw(...)
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

	DWORD fontSize = SizeofResource(NULL, hResource);
	DWORD NumFonts;
	HANDLE hFont = AddFontMemResourceEx(lpRawFont, fontSize, NULL, &NumFonts);
	if (NULL == hFont)
		throw stdWCexception(L"无法获取字体句柄！");

	return hFont;
}

LPSTREAM Game::GetPNGStreamW(const LPCWSTR lpResID, const LPCWSTR lpResType) throw(...)
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

LPVOID Game::GetRawWAVBufferW(const LPCWSTR lpResID, const LPCWSTR lpResType) throw(...)
{
	HRSRC hResource = FindResourceW(NULL, lpResID, lpResType);
	if (NULL == hResource)
		throw stdWCexception(L"无法获取WAV声音资源！");

	HGLOBAL hGlobal = LoadResource(NULL, hResource);
	if (NULL == hGlobal)
		throw stdWCexception(L"无法装载WAV声音资源！");

	LPVOID lpRawWAV = LockResource(hGlobal);
	if (NULL == lpRawWAV)
		throw stdWCexception(L"WAV声音资源无效！");

	return lpRawWAV;
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
