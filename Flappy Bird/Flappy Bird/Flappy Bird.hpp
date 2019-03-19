#pragma once
#include "resource.h"

namespace cmdLineCfg
{
	// Varibles
#ifdef _DEBUG
	bool isDebugMode = true;
	bool fileLogged = true;
#else
	bool isDebugMode = false;
	bool fileLogged = false;
#endif

	
	bool showHelp = false;

	// Functions
	bool parseCmdLine(_In_ int argc, _In_ char *argv[]);
}



HWND createEXWindow(INT width, INT height, bool isWindowShow);
