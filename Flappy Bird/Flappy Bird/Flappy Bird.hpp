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
	const UINT INSERT_BEFORE	= 0;
	const UINT INSERT_AFTER		= 1;
	const LPCWSTR lpFontName = L"04b_19";
	
	// Data structures
	struct OBJIMG
	{
		IMAGE im;
		INT *posx = NULL, *posy = NULL;	// Optimize: store pointers instead of variables
		DOUBLE *rot = NULL;				//			 to prevent intense alter of elements of vectors.
		DWORD dwRop;
	};

	typedef std::vector<OBJIMG> LAYER;
	typedef std::vector<LAYER> SCENE;

	typedef void (*fxpDrawing)();
	typedef std::vector<fxpDrawing> FXLAYERS;

	// Instances
	SCENE mainScene;
	FXLAYERS fxLayers;

	void subGame();
	void printGameTitle();
	HWND createEXWindow(const int width, const int height, const bool isWindowShow);
	DWORD WINAPI refreshLoop(LPVOID lpParam);
	HANDLE GetFontHandleW(const LPCWSTR lpResID, const LPCWSTR lpResType);
}


// Constants
const int WNDWIDTH = 768;
const int WNDHEIGHT = 896;

// Global variables
std::wstring wsLogPath;
Log *logger = NULL;
