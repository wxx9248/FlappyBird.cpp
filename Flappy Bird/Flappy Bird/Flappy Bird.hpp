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
	
	// Data structures
	struct OBJIMG
	{
		IMAGE im;
		INT *posx = NULL, *posy = NULL;	// Optimize: store pointers instead of variables
		DOUBLE *rot = NULL;				//			 to prevent intense alter of elements of vectors.
		DWORD dwRop;
	};

	typedef CHAR KBE;

	typedef std::vector<OBJIMG> LAYER;
	typedef std::vector<LAYER> SCENE;

	typedef void (*fxpDrawing)();
	typedef std::vector<fxpDrawing> FXLAYERS;
	typedef std::queue<KBE> KBEMSGQUEUE;
	
	// Instances
	SCENE mainScene;
	FXLAYERS fxLayers;
	KBEMSGQUEUE KBEMsgQueue;

	void subGame();
	void printGameTitle();
	void printGameStartHint();
	void postKBEvent(KBE event);
	KBE asyncGetKBEvent();
	KBE waitKBEvent();

	HWND createEXWindow(const int width, const int height, const bool isWindowShow);
	DWORD WINAPI refreshLoop(LPVOID lpParam);
	DWORD WINAPI KBELoop(LPVOID lpParam);
	HANDLE GetFontHandleW(const LPCWSTR lpResID, const LPCWSTR lpResType);
}


// Constants
const int WNDWIDTH = 768;
const int WNDHEIGHT = 896;

// Global variables
std::wstring wsLogPath;
Log *logger = NULL;
