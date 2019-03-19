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



class invalidParameters : public stdWCexception
{
public:
	using stdWCexception::stdWCexception;
};

class createWindowFailed : public stdWCexception
{
public:
	using stdWCexception::stdWCexception;
};
