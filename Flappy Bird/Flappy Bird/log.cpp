#include "stdafx.h"
#include "log.hpp"

Log::Log()
{
	init();
}

Log::Log(std::wstring &logFilePath)
{
	init(logFilePath);
}

Log::~Log()
{
	if (logFilePath != NULL)
	{
		delete logFilePath;
		logFilePath = NULL;
	}
	if (logFile != NULL)
	{
		delete logFile;
		logFile = NULL;
	}
}

void Log::init()
{	
	isInited = true;
	isOpened = true;
	goodbit = true;
	Log::logFilePath = NULL;
	Log::logFile = NULL;
}

bool Log::init(std::wstring &logFilePath)
{
	if (isInited)
		reset();

	Log::logFilePath = new std::wstring;
	if (Log::logFilePath == NULL)
		throw std::bad_alloc();
	*Log::logFilePath = logFilePath;

	isInited = true;

	Log::logFile = new std::ofstream;
	if (Log::logFilePath == NULL)
		throw std::bad_alloc();

	logFile->open(logFilePath);

	if (!(*logFile))
	{
		throw openLogFileFailed(L"不能打开文件:" + logFilePath);
		return false;
	}
	else
	{
		isOpened = true;
		goodbit = true;
		return true;
	}
}

bool Log::close()
{
	if (isOpened)
	{
		if (logFile != NULL)
			logFile->close();
		else
			reset();
		return true;
	}
	else
		return false;
}

bool Log::clearbit()
{
	if (Log::logFile != NULL)
	{
		logFile->clear();
		badbit = false;
		return true;
	}
	else
		return false;
}

void Log::reset()
{
	isInited = isOpened = goodbit = badbit = false;
	Log::logFilePath = NULL;
	Log::logFile = NULL;
}

Log &operator<<(Log &log, std::wstring &msg)
{
	return log;
}