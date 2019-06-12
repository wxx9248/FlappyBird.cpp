#pragma once
#include "resource.h"
#include "log.hpp"
#include "sfx.hpp"

namespace cmdLineCfg
{
	// Varibles
#ifdef _DEBUG
	bool isDebugMode = true;
	bool fileLogged = false;
#else
	bool isDebugMode = false;
	bool fileLogged = false;
#endif


	bool showHelp = false;
	INT argvLogFilePathIndex = 0;


	// Functions
	bool parseCmdLine(_In_ int argc, _In_ char *argv[]);
}


namespace Game
{
	// Constants
	LPCWSTR lpFontName = L"04b_19";
	const INT WNDWIDTH = 768;
	const INT WNDHEIGHT = 1024;
	const INT logoPosY = 180;
	const DOUBLE logoSZMultiplier = 2.5;
	const INT startHintPosX = 190;
	const INT startHintPosY = 600;
	const DOUBLE hintSZMultiplier = 2;
	const INT hintPosY = 250;
	const INT digitPosX = 365;
	const INT digitPosY = 100;
	const DOUBLE GameOverBannerSZMultiplier = 2.5;
	const INT GameOverBannerPosY = 180;
	const DOUBLE ScoreboardSZMultiplier = 2;
	const INT ScoreboardPosY = 380;
	const INT ScoreboardScorePosY = int(ScoreboardPosY + 32 * ScoreboardSZMultiplier);
	const INT ScoreboardHScorePosY = int(ScoreboardScorePosY + 42 * ScoreboardSZMultiplier);
	const INT RestartButtonPosY = 700;
	const INT PipeObjNum = 3;
	const INT dPipeVertical = 220;
	const INT dPipeHorizontal = 384;
	const INT birdDefPosY = 450;
	const INT birdStaticWingPeriod = 15;
	const INT birdStaticFlucPeriod = 15;
	const DOUBLE birdStaticFlucAngFreq = 0.05;
	const DOUBLE birdStaticFlucAmplitude = 2;
	const INT birdDynamicWingPeriod = 10;
	const DOUBLE defDownSpeed = 0;
	const DOUBLE defDownSpeedUp = -10.5;
	const DOUBLE upSpeedGain = 0.42;
	const DOUBLE downSpeedGain = 0.22;
	const DOUBLE speedQRatio = 0.07;
	const INT startftsz = 32;
	const INT digitftsz = 72;
	const INT Scoreboarddigitftsz = 48;
	const INT lBoundGroundImg = -36;
	const INT lBoundPipeDn = 300;
	const INT uBoundPipeDn = WNDWIDTH - 20;

	LPCWSTR CWCStrMutexRef = L"MutexRefresh";
	LPCWSTR CWCStrMutexGNDAni = L"MutexGNDAnimation";
	LPCWSTR CWCStrMutexBird = L"MutexBirdAnimation";
	LPCWSTR CWCStrMutexKBE = L"MutexKeyboardEvent";
	LPCWSTR lpAudioResIDs[] =
	{
		L"IDR_AUDIO_WING", L"IDR_AUDIO_HIT",
		L"IDR_AUDIO_DIE", L"IDR_AUDIO_POINT",
		L"IDR_AUDIO_MUTE"
	};

	// Data structures
	struct OBJIMG
	{
		IMAGE im;
		INT posx = NULL, posy = NULL;
		DWORD dwRop;
	};

	struct OBJCIMG
	{
		CImage cim;
		INT posx = NULL, posy = NULL;
	};

	class Hint
	{
	public:
		Hint();
		Hint
		(
			LPCWSTR pResName1, LPCWSTR pResName1_m,
			LPCWSTR pResType
		) throw(...);

		void init
		(
			LPCWSTR pResName1, LPCWSTR pResName1_m,
			LPCWSTR pResType
		) throw(...);

		void changeVisibility();
		bool getVisibility();
		INT getX();
		INT getY();

		void draw();

		IMAGE &operator[](INT index);
	private:
		IMAGE imgHint[2] = { 0 };
		INT posxHint = 0;
		INT posyHint = hintPosY;
		bool isVisible = false;
	} *pHint = NULL;


	class Bird
	{
	public:
		Bird();
		Bird
		(
			LPCWSTR pResName1, LPCWSTR pResName1_m,
			LPCWSTR pResName2, LPCWSTR pResName2_m,
			LPCWSTR pResName3, LPCWSTR pResName3_m,
			LPCWSTR pResType
		) throw(...);

		void init
		(
			LPCWSTR pResName1, LPCWSTR pResName1_m,
			LPCWSTR pResName2, LPCWSTR pResName2_m,
			LPCWSTR pResName3, LPCWSTR pResName3_m,
			LPCWSTR pResType
		) throw(...);

		void changeState();
		void changeState(INT state);
		void changeVisibility();
		bool getVisibility();
		void gain(INT val);
		void setX(INT pos);
		void setY(INT pos);
		INT getX();
		INT getY();

		void draw();

		IMAGE &operator[](INT index);
	private:
		IMAGE imgBird[6] = { 0 };
		INT posxBird = 0;
		INT posyBird = birdDefPosY;
		INT birdState = 0;		// 0, 1, 2
		bool isVisible = false;
	} *pBird = NULL;

	class Pipe
	{
	public:
		Pipe();
		Pipe
		(
			LPCWSTR pResName1, LPCWSTR pResName1_m,
			LPCWSTR pResName2, LPCWSTR pResName2_m,
			LPCWSTR pResType
		) throw(...);

		void init
		(
			LPCWSTR pResName1, LPCWSTR pResName1_m,
			LPCWSTR pResName2, LPCWSTR pResName2_m,
			LPCWSTR pResType
		) throw(...);

		void changeVisibility();
		bool getVisibility();
		void gain(INT val);
		void setX(INT pos);
		void setYDn(INT pos);
		INT getX();
		INT getYDn();

		void draw();

		IMAGE &operator[](INT index);

	private:
		IMAGE imgPipe[4] = { 0 };
		INT posxPipe = 0;
		INT posyPipeDn = 0;
		bool isVisible = false;
	} *pPipe = NULL;

	class Medal			// Platinum, Gold, Silver, Bronze
	{
	public:
		Medal();
		Medal
		(
			LPCWSTR pResName1, LPCWSTR pResName2,
			LPCWSTR pResName3, LPCWSTR pResName4,
			LPCWSTR pResType, const OBJCIMG &ScoreBoard,		// Automatic positioning
			const HWND _hWnd
		) throw(...);

		void init
		(
			LPCWSTR pResName1, LPCWSTR pResName2,
			LPCWSTR pResName3, LPCWSTR pResName4,
			LPCWSTR pResType, const OBJCIMG &ScoreBoard,
			const HWND _hWnd
		) throw(...);

		void changeState(INT state);
		INT getState();
		void changeWindowHandle(HWND _hWnd) throw(...);
		void setX(INT pos);
		INT getX();
		INT getY();

		void draw() throw(...);

		CImage &operator[](INT index);

	private:
		HWND hWnd = NULL;
		CImage imgMedal[4] = {};
		INT posxMedal = 0;
		INT posyMedal = int(ScoreboardPosY + 42 * ScoreboardSZMultiplier);
		INT MedalState = 0;		// 0, 1, 2, 3, 4
	} *pMedal = NULL;

	SFX *psfx = NULL;
	enum
	{
		Snd_Wing, Snd_Hit,
		Snd_Die, Snd_Point,
		Snd_Mute
	} SFXsnd;

	// Types
	typedef std::vector<OBJIMG *> LAYER;
	typedef std::vector<LAYER> SCENE;
	typedef void(*fxpDrawing)();
	typedef std::vector<fxpDrawing> FXLAYERS;
	typedef std::queue<CHAR> KBEMSGQUEUE;
	typedef std::vector<HANDLE> THREADQUEUE;

	// Variables
	WCHAR cntdwnChar = L'3';
	INT score = 0;
	INT highscore = 0;
	DOUBLE downSpeed;
	volatile bool lockPipe = true;
	volatile bool lockBird = true;
	bool gameState = false;
	bool isGrounded = false;
	INT ScoreboardScorePosX = 0;
	INT ScoreboardHScorePosX = 0;

	// Instances
	SCENE mainScene;
	FXLAYERS fxLayers;
	KBEMSGQUEUE KBEMsgQueue;

	std::default_random_engine rand;
	std::uniform_int_distribution<INT> rangePipeDn(lBoundPipeDn, uBoundPipeDn);

	OBJIMG BG;
	OBJIMG Ground;

	LAYER lBG;

	OBJCIMG Logo;
	OBJCIMG GameOver;
	OBJCIMG Scoreboard;
	OBJCIMG Highscore;
	OBJCIMG bRestart;

	// Handles
	HWND hWnd = NULL;

	// Raw entities pointers
	LPVOID lpWAVWing = NULL;
	LPVOID lpWAVHit = NULL;
	LPVOID lpWAVDie = NULL;
	LPVOID lpWAVPoint = NULL;

	// Functions
	void subGame() throw(...);
	HWND createEXWindow(const int width, const int height, const bool isWindowShow) throw(...);

	// - Static drawing functions
	void printBG();
	void printGameStartHint();
	void printCountdown();
	void printScore();
	void printEndScore();
	void printEndHighScore();

	// - Event handler functions
	void postKBEvent(CHAR event);
	CHAR asyncGetKBEvent();
	CHAR waitKBEvent();
	DWORD WINAPI KBELoop(LPVOID lpParam);

	// - Dynamic drawing functions
	DWORD WINAPI refreshLoop(LPVOID lpParam);
	DWORD WINAPI GNDAnimationLoop(LPVOID lpParam);
	DWORD WINAPI BirdAnimationLoop(LPVOID lpParam);

	// - Tool functions
	HANDLE GetFontHandleW(LPCWSTR lpResID, LPCWSTR lpResType) throw(...);
	LPSTREAM GetPNGStreamW(LPCWSTR lpResID, LPCWSTR lpResType) throw(...);
}

// Global variables
std::wstring wsLogPath;
Log *logger = NULL;
