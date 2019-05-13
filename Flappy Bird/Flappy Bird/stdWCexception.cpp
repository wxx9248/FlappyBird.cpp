#include "stdafx.h"
#include "stdWCexception.hpp"

LPCWSTR WCexception::WCwhat() const noexcept
{
	return L"Unknown WCException";
}

const char *WCexception::what() const noexcept
{
	// Since this class is based on wide characters, what() is disabled;
	return "Please overwrite WCwhat().";
}

stdWCexception::stdWCexception(LPCWSTR imsg)
{
	msg = imsg;
}

stdWCexception::stdWCexception(const std::wstring &imsg)
{
	msg = imsg;
}

LPCWSTR stdWCexception::WCwhat() const noexcept
{
	return msg.c_str();
}
