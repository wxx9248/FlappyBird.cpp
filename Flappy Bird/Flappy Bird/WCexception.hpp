#pragma once

class WCexception : public std::exception
{
public:
	virtual LPCWSTR WCwhat() const noexcept;

private:
	virtual const char *what() const noexcept;		// Disable what()
};
