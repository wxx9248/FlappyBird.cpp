#include "stdafx.h"
#include "WCexception.hpp"

LPCWSTR WCexception::WCwhat() const noexcept
{
	return L"Unknown WCException";
}

const char *WCexception::what() const noexcept
{
	// Since this class is based on wide characters, what() is disabled;
	return "Please overwrite WCwhat().";
}
