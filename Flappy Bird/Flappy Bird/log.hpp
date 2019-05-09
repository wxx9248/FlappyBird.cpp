#pragma once

// Exceptions
extern class openFileFailed;

class Outdev
{
public:
	Outdev();

	template<class T>
	Outdev(T &logFilePath);

	~Outdev();

	void init();

	template<class T>
	bool init(T &logFilePath);

	void close();
	bool clear();

	template<class T>
	friend Outdev &operator<<(Outdev &od, T &msg);

private:
	friend class Log;
	bool isInited = false, isOpened = false;
	bool goodbit = false, badbit = false, failbit = false, eofbit = false;
	std::wstring *logFilePath = NULL;
	std::wofstream *logFile = NULL;
};

class Log
{
public:
	Log();

	template<class T>
	Log(T &logFilePath);

	~Log();

	void init();

	template<class T>
	bool init(T &logFilePath);

	void close();
	bool clear();

	template<class T>
	friend Log &operator<<(Log &log, T &msg);
private:
	Outdev *od = NULL;
};



Outdev::Outdev()
{
	init();
}

template<class T>
Outdev::Outdev(T &logFilePath)
{
	init(logFilePath);
}

Outdev::~Outdev()
{
	close();
}

void Outdev::init()
{
	if (isInited)
		close();

	isInited = true;
	isOpened = true;
	goodbit = true;
	Outdev::logFilePath = NULL;
	Outdev::logFile = NULL;
}

template<class T>
bool Outdev::init(T &logFilePath)
{
	std::wstring excwstr = L"不能打开文件:";
	if (isInited)
		close();

	Outdev::logFilePath = new std::wstring;
	if (Outdev::logFilePath == NULL)
		throw std::bad_alloc();
	Outdev::logFilePath = &logFilePath;

	isInited = true;

	Outdev::logFile = new std::wofstream;
	if (Outdev::logFilePath == NULL)
		throw std::bad_alloc();

	logFile->open(logFilePath);

	if (!(*logFile))
	{
		throw openLogFileFailed(excwstr + logFilePath);
		return false;
	}
	else
	{
		isOpened = true;
		goodbit = true;
		return true;
	}
}

bool Outdev::clear()
{
	if (Outdev::isInited)
	{
		std::wcout.clear();

		badbit = failbit = eofbit = false;
		goodbit = true;

		if (Outdev::logFile != NULL)
		{
			logFile->clear();
		}

		return true;
	}
	else
		return false;
}

void Outdev::close()
{
	// Dumping all things!

	isInited = isOpened = goodbit = badbit = failbit = eofbit = false;

	try
	{
		delete Outdev::logFile;
	}
	catch (...) {}

	try
	{
		delete Outdev::logFilePath;
	}
	catch (...) {}

	Outdev::logFilePath = NULL;
	Outdev::logFile = NULL;
}

template<class T>
Outdev &operator<<(Outdev &od, T &msg)
{
	if (od.isInited && od.isOpened && od.goodbit)
	{
		std::wcout << msg;
		if (od.logFile != NULL)
		{
			*od.logFile << msg;

			od.badbit = std::wcout.bad() || od.logFile->bad();
			od.failbit = std::wcout.fail() || od.logFile->fail();
			od.eofbit = std::wcout.eof() || od.logFile->eof();
			od.goodbit = !(od.badbit || od.failbit || od.eofbit);
		}
		else
		{
			od.badbit = std::wcout.bad();
			od.failbit = std::wcout.fail();
			od.eofbit = std::wcout.eof();
			od.goodbit = !(od.badbit || od.failbit || od.eofbit);
		}
	}
	return od;
}

// Class Log

Log::Log()
{
	init();
}

template<class T>
Log::Log(T &logFilePath)
{
	init(logFilePath);
}

Log::~Log()
{
	close();
}

void Log::init()
{
	if (od != NULL)
		close();

	od = new Outdev;
	od->init();
}

template<class T>
bool Log::init(T &logFilePath)
{
	if (od != NULL)
		close();

	od = new Outdev;
	return od->init(logFilePath);
}

void Log::close()
{
	if (od != NULL)
	{
		od->close();
		delete od;
	}
	od = NULL;
}

bool Log::clear()
{
	if (od != NULL)
		return od->clear();
	else
		return false;
}

template<class T>
Log &operator<<(Log &log, T &msg)
{
	static time_t timer;
	static tm ts;
	static wchar_t wasctmbuf[32];
	static errno_t en;

	time(&timer);
	localtime_s(&ts, &timer);

	en = _wasctime_s(wasctmbuf, &ts);

	if (en)
		throw en;

	wasctmbuf[wcslen(wasctmbuf) - 1] = L'\0';

	(*log.od) << L"[" << wasctmbuf << L"] " << msg << L"\n";

	return log;
}
