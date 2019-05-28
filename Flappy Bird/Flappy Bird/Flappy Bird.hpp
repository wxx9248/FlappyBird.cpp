#pragma once
#include "resource.h"
#include "log.hpp"

extern class stdWCexception;

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
	bool parseCmdLine(const _In_ int argc, _In_ char *argv[]);
}


namespace Game
{
	// Constants
	const LPCWSTR lpFontName = L"04b_19";
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
	const INT ScoreboardScorePosY = ScoreboardPosY + 32 * ScoreboardSZMultiplier;
	const INT ScoreboardHScorePosY = ScoreboardScorePosY + 42 * ScoreboardSZMultiplier;
	const INT RestartButtonPosY = 700;
	const INT dPipeVertical = 200;
	const INT dPipeHorizontal = 128;
	const INT birdDefPosY = 450;
	const INT birdGain = 20;
	const INT birdStaticWingPeriod = 15;
	const INT birdStaticFlucPeriod = 15;
	const DOUBLE birdStaticFlucAngFreq = 0.05;
	const DOUBLE birdStaticFlucAmplitude = 2;
	const DOUBLE defDownSpeed = 1.414213562373095;
	const DOUBLE downSpeedGain = 0.05;
	const DOUBLE downSpeedQRatio = 0.2;
	const INT startftsz = 32;
	const INT digitftsz = 72;
	const INT Scoreboarddigitftsz = 48;
	const INT lBoundGroundImg = -36;
	const INT lBoundPipeDn = 300;
	const INT uBoundPipeDn = WNDWIDTH - 20;

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
			const LPCWSTR pResName1, const LPCWSTR pResName1_m,
			const LPCWSTR pResType
		) throw();

		void init
		(
			const LPCWSTR pResName1, const LPCWSTR pResName1_m,
			const LPCWSTR pResType
		) throw();

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
			const LPCWSTR pResName1, const LPCWSTR pResName1_m,
			const LPCWSTR pResName2, const LPCWSTR pResName2_m,
			const LPCWSTR pResName3, const LPCWSTR pResName3_m,
			const LPCWSTR pResType
		) throw();

		void init
		(
			const LPCWSTR pResName1, const LPCWSTR pResName1_m,
			const LPCWSTR pResName2, const LPCWSTR pResName2_m,
			const LPCWSTR pResName3, const LPCWSTR pResName3_m,
			const LPCWSTR pResType
		) throw();
		
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
			const LPCWSTR pResName1, const LPCWSTR pResName1_m,
			const LPCWSTR pResName2, const LPCWSTR pResName2_m,
			const LPCWSTR pResType
		) throw();

		void init
		(
			const LPCWSTR pResName1, const LPCWSTR pResName1_m,
			const LPCWSTR pResName2, const LPCWSTR pResName2_m,
			const LPCWSTR pResType
		) throw();

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
			const LPCWSTR pResName1, const LPCWSTR pResName2,
			const LPCWSTR pResName3, const LPCWSTR pResName4,
			const LPCWSTR pResType, const OBJCIMG &ScoreBoard,		// Automatic positioning
			const HWND hWnd
		) throw();

		void init
		(
			const LPCWSTR pResName1, const LPCWSTR pResName2,
			const LPCWSTR pResName3, const LPCWSTR pResName4,
			const LPCWSTR pResType, const OBJCIMG &ScoreBoard,
			const HWND hWnd
		) throw();

		void changeState(INT state);
		INT getState();
		void changeWindowHandle(HWND hWnd) throw();
		void setX(INT pos);
		INT getX();
		INT getY();

		void draw() throw();

		CImage &operator[](INT index);

	private:
		HWND hWnd = NULL;
		CImage imgMedal[4] = {};
		INT posxMedal = 0;
		INT posyMedal = ScoreboardPosY + 42 * ScoreboardSZMultiplier;
		INT MedalState = 0;		// 0, 1, 2, 3, 4
	} *pMedal = NULL;

	// Types
	typedef std::vector<OBJIMG *> LAYER;
	typedef std::vector<LAYER> SCENE;
	typedef void (*fxpDrawing)();
	typedef std::vector<fxpDrawing> FXLAYERS;
	typedef std::queue<CHAR> KBEMSGQUEUE;
	
	// Variables
	WCHAR cntdwnChar = L'3';
	INT score = 0;
	INT highscore = 0;
	DOUBLE downSpeed;
	volatile bool lockPipe = true;
	volatile bool lockBird = true;
	volatile bool canIgetonepoint = false;
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
	HANDLE hMutRef = NULL;
	HANDLE hMutGNDAni = NULL;
	HANDLE hMutBirdAni = NULL;
	HWND hWnd = NULL;

	// Raw entities pointers
	LPVOID lpWAVWing = NULL;
	LPVOID lpWAVHit = NULL;
	LPVOID lpWAVDie = NULL;
	LPVOID lpWAVPoint = NULL;

	// Functions
	void subGame() throw();
	HWND createEXWindow(const int width, const int height, const bool isWindowShow);

	// - Static drawing functions
	void printBG();
	void printGameStartHint();
	void printCountdown();
	void printScore();
	void printEndScore();
	void printEndHighScore();

	// - Behavior functions
	void stimulate();

	// - Event handler functions
	void postKBEvent(CHAR event);
	CHAR asyncGetKBEvent();
	CHAR waitKBEvent();
	DWORD WINAPI KBELoop(LPVOID lpParam);
	DWORD WINAPI MSELoop(LPVOID lpParam);

	// - Dynamic drawing functions
	DWORD WINAPI refreshLoop(LPVOID lpParam);
	DWORD WINAPI GNDAnimationLoop(LPVOID lpParam);
	DWORD WINAPI BirdAnimationLoop(LPVOID lpParam);

	// - Tool functions
	HANDLE GetFontHandleW(const LPCWSTR lpResID, const LPCWSTR lpResType) throw();
	LPSTREAM GetPNGStreamW(const LPCWSTR lpResID, const LPCWSTR lpResType) throw();
	LPVOID GetRawWAVBufferW(const LPCWSTR lpResID, const LPCWSTR lpResType) throw();
	template<class T>
	std::queue<T> &clearQueue(std::queue<T> &q);
}

// Global variables
std::wstring wsLogPath;
Log *logger = NULL;
