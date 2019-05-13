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
	bool parseCmdLine(_In_ int argc, _In_ char *argv[]);
}


namespace Game
{
	// Constants
	const UINT INSERT_BEFORE	= 0;
	const UINT INSERT_AFTER		= 1;

	
	// Data structures
	struct OBJIMG
	{
		IMAGE im;
		INT *posx = NULL, *posy = NULL;	// Optimize: store pointers instead of variables
		DOUBLE *rot = NULL;		//			 to prevent intense alter of elements of vectors.
		DWORD dwRop;
	};

	typedef std::vector<OBJIMG> LAYER;
	typedef std::vector<OBJIMG>::iterator iLAYER;
	typedef std::vector<LAYER> SCENE;
	typedef std::vector<LAYER>::iterator iSCENE;

	typedef LPVOID LPRFONT;

	// Instances
	SCENE mainScene;

	void subGame();
	HWND createEXWindow(int width, int height, bool isWindowShow);
	DWORD WINAPI refreshLoop(LPVOID lpParam);
	LPRFONT getRawFontW(LPCWSTR lpResID, LPCWSTR lpResType);
}


// Constants
const int WNDWIDTH = 768;
const int WNDHEIGHT = 896;

// Global variables
std::wstring wsLogPath;
Log *logger = NULL;
