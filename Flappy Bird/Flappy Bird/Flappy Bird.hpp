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


	// Data structures
	struct OBJIMG
	{
		IMAGE im;
		INT *posx = NULL, *posy = NULL;	// Optimize: store pointers instead of variables
										//			 to prevent intense alter of elements of vectors.
		DWORD dwRop;
	};

	struct OBJCIMG
	{
		CImage cim;
		INT *posx = NULL, *posy = NULL;

		DOUBLE *rot = NULL;
	};

	typedef CHAR KBE;

	typedef std::vector<OBJIMG> LAYER;
	typedef std::vector<LAYER> SCENE;

	typedef std::vector<OBJCIMG> CLayer;
	typedef std::vector<CLayer> CScene;

	typedef void (*fxpDrawing)();
	typedef std::vector<fxpDrawing> FXLAYERS;

	typedef std::queue<KBE> KBEMSGQUEUE;
	
	// Variables
	WCHAR cntdwnChar = L'3';
	INT score = 0;
	INT highscore = 0;
	INT posxGND = 0;
	INT posyBird = 0;
	DOUBLE rotBird = 0;
	INT posxPipe = 0;
	INT posyPipeDn = 0;
	INT posyPipeUp = 0;


	// Instances
	SCENE mainScene;
	CScene CmainScene;
	FXLAYERS fxLayers;
	KBEMSGQUEUE KBEMsgQueue;

	// Handles
	HANDLE hMutRef = NULL;
	HANDLE hMutAni = NULL;
	HWND hWnd = NULL;

	// Functions
	void subGame();
	void printGameTitle();
	void printGameStartHint();
	void printCountdown();
	void printScore();
	void printEndScore();
	void printEndHighScore();
	void postKBEvent(KBE event);
	KBE asyncGetKBEvent();
	KBE waitKBEvent();

	HWND createEXWindow(const int width, const int height, const bool isWindowShow);
	DWORD WINAPI refreshLoop(LPVOID lpParam);
	DWORD WINAPI KBELoop(LPVOID lpParam);
	DWORD WINAPI animationLoop(LPVOID lpParam);
	HANDLE GetFontHandleW(const LPCWSTR lpResID, const LPCWSTR lpResType);
	LPSTREAM GetPNGStreamW(const LPCWSTR lpResID, const LPCWSTR lpResType);
}


// Constants
const int WNDWIDTH = 768;
const int WNDHEIGHT = 1024;

// Global variables
std::wstring wsLogPath;
Log *logger = NULL;
