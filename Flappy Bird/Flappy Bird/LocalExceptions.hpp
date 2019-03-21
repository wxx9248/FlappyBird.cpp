#pragma once
#include "WCexception.hpp"

class stdWCexception: public WCexception
{
public:
	stdWCexception(LPCWSTR imsg);
	stdWCexception(const std::wstring &imsg);
	virtual LPCWSTR WCwhat() const noexcept;

protected:
	std::wstring msg;
};



extern class invalidParameters : public stdWCexception
{
public:
	using stdWCexception::stdWCexception;
};

extern class createWindowFailed : public stdWCexception
{
public:
	using stdWCexception::stdWCexception;
};

extern class openLogFileFailed : public stdWCexception
{
public:
	using stdWCexception::stdWCexception;
};
