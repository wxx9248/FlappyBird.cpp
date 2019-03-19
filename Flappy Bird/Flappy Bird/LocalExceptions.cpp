#include "stdafx.h"

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
