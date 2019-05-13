#pragma once

class WCexception : public std::exception
{
public:
	virtual LPCWSTR WCwhat() const noexcept;

private:
	virtual const char *what() const noexcept;		// Disable what()
};


class stdWCexception : public WCexception
{
public:
	stdWCexception(LPCWSTR imsg);
	stdWCexception(const std::wstring &imsg);
	virtual LPCWSTR WCwhat() const noexcept;

protected:
	std::wstring msg;
};
