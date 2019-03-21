#pragma once

// Exceptions
extern class openFileFailed;

class Log
{
public:
	Log();
	Log(std::wstring &logFilePath);
	~Log();

	void init();
	bool init(std::wstring &logFilePath);
	bool close();
	bool clearbit();
	void reset();
	Log &operator<<(std::wstring &msg);

private:
	bool isInited = false, isOpened = false, goodbit = false, badbit = false;
	std::wstring *logFilePath = NULL;
	std::ofstream *logFile = NULL;
	std::chrono::system_clock::time_point now;
};

