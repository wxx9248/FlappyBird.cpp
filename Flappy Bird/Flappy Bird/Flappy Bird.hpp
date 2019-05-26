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
	int argvLogFilePathIndex = 0;


	// Functions
	bool parseCmdLine(const _In_ int argc, _In_ char *argv[]);
}


namespace Game
{
	// Constants
	const LPCWSTR lpFontName = L"04b_19";
	const INT dPipeHeight = 60;
	const int WNDWIDTH = 768;
	const int WNDHEIGHT = 1024;
	const int birdDefPosY = 450;
	const int birdGain = 20;

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
	
	class Bird
	{
	public:
		Bird
		(
			LPCWSTR pResName1, LPCWSTR pResName1_m,
			LPCWSTR pResName2, LPCWSTR pResName2_m,
			LPCWSTR pResName3, LPCWSTR pResName3_m,
			LPCWSTR pResType
		) throw();

		void init
		(
			LPCWSTR pResName1, LPCWSTR pResName1_m,
			LPCWSTR pResName2, LPCWSTR pResName2_m,
			LPCWSTR pResName3, LPCWSTR pResName3_m,
			LPCWSTR pResType
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

	typedef CHAR KBE;

	typedef std::vector<OBJIMG *> LAYER;
	typedef std::vector<LAYER> SCENE;

	typedef void (*fxpDrawing)();
	typedef std::vector<fxpDrawing> FXLAYERS;

	typedef std::queue<KBE> KBEMSGQUEUE;
	
	// Variables
	WCHAR cntdwnChar = L'3';
	INT score = 0;
	INT highscore = 0;
	volatile bool lockPipe = true;
	volatile bool lockBird = true;
	DOUBLE downSpeed;

	// Instances
	SCENE mainScene;
	FXLAYERS fxLayers;
	KBEMSGQUEUE KBEMsgQueue;

	OBJIMG BG;
	OBJIMG Ground;

	LAYER lBG;

	OBJCIMG SB;
	OBJCIMG bRestart;

	// Handles
	HANDLE hMutRef = NULL;
	HANDLE hMutGNDAni = NULL;
	HANDLE hMutBirdAni = NULL;
	HWND hWnd = NULL;

	// Functions
	void subGame();
	HWND createEXWindow(const int width, const int height, const bool isWindowShow);

	// - Static drawing functions
	void printBG();
	void printGameTitle();
	void printGameStartHint();
	void printCountdown();
	void printScore();
	void printGameOver();
	void printEndScore();
	void printEndHighScore();

	// - Behavior functions
	void stimulate();
	DWORD WINAPI judgeLoop(LPVOID lpParam);

	// - Event handler functions
	void postKBEvent(KBE event);
	KBE asyncGetKBEvent();
	KBE waitKBEvent();
	DWORD WINAPI KBELoop(LPVOID lpParam);
	DWORD WINAPI MSELoop(LPVOID lpParam);

	// - Dynamic drawing functions
	DWORD WINAPI refreshLoop(LPVOID lpParam);
	DWORD WINAPI GNDAnimationLoop(LPVOID lpParam);
	DWORD WINAPI BirdAnimationLoop(LPVOID lpParam);

	// - Tool functions
	HANDLE GetFontHandleW(const LPCWSTR lpResID, const LPCWSTR lpResType);
	LPSTREAM GetPNGStreamW(const LPCWSTR lpResID, const LPCWSTR lpResType);
}

// Global variables
std::wstring wsLogPath;
Log *logger = NULL;
