#include "stdafx.hpp"
#include "sfx.hpp"

SFX::SFX() {}

SFX::SFX(const SFX &sfx)
{
	v = sfx.v;
}

SFX::SFX(const WAV _wav[], UINT n)
{
	init(_wav, n);
}

SFX::SFX(const LPVOID lpWAVBuffer[], UINT n) throw(...)
{
	init(lpWAVBuffer, n);
}

SFX::SFX(LPCWSTR wcResName[], LPCWSTR wcResType, UINT n) throw(...)
{
	init(wcResName, wcResType, n);
}

void SFX::init(const WAV _wav[], const UINT n)
{
	initDS();
	clear();

	for (UINT i = 0; i < n; ++i)
		v.push_back(_wav[i]);
}

void SFX::init(const LPVOID lpWAVBuffer[], const UINT n) throw(...)
{
	WAV wav;

	initDS();
	clear();

	if (NULL == lpWAVBuffer)
		throw stdWCexception(L"SFX：WAV内存资源指针数组基地址无效");

	for (UINT i = 0; i < n; ++i)
	{
		if (NULL == lpWAVBuffer[i])
		{
			wstring ws = L"SFX：WAV内存资源指针(";
			ws += i;
			ws += L")无效";
			throw stdWCexception(ws);
		}

		// TODO: Fill in struct WAV
		wav.lpWAVBuffer = lpWAVBuffer[i];

		v.push_back(wav);
	}
}

void SFX::init(LPCWSTR wcResName[], LPCWSTR wcResType, UINT n) throw(...)
{
	LPVOID *wavbuf = new LPVOID[n];

	initDS();
	clear();

	for (UINT i = 0; i < n; ++i)
		wavbuf[i] = GetRawWAVBufferW(wcResName[i], wcResType);

	init(wavbuf, n);
	delete[] wavbuf;
}

void SFX::init(const pair<LPCWSTR, LPCWSTR> ResNameTypePair[], UINT n) throw(...)
{
	LPVOID *wavbuf = new LPVOID[n];

	initDS();
	clear();

	for (UINT i = 0; i < n; ++i)
		wavbuf[i] = GetRawWAVBufferW(ResNameTypePair[i].first, ResNameTypePair[i].second);

	init(wavbuf, n);
	delete[] wavbuf;
}

void SFX::clear()
{
	v.clear();
}

const WAV &SFX::get(UINT n) throw(...)
{
	if (n >= v.size())
		throw stdWCexception(L"SFX：在调用声音资源结构向量时下标越界！");
	else
		return v[n];
}

void SFX::erase(UINT n) throw(...)
{
	if (n >= v.size())
		throw stdWCexception(L"SFX：在调用声音资源结构向量时下标越界！");
	else
		v.erase(v.begin() + n);
}

void SFX::play(UINT index) throw(...)
{
	// TODO: Play sound asyncly
}

const SFX SFX::operator+(const SFX &sfx)
{
	SFX new_sfx;

	new_sfx.v.insert(new_sfx.v.end(), v.begin(), v.end());
	new_sfx.v.insert(new_sfx.v.end(), sfx.v.begin(), sfx.v.end());

	return new_sfx;
}

const SFX &SFX::operator+=(const SFX &sfx)
{
	v.insert(v.end(), sfx.v.begin(), sfx.v.end());

	return *this;
}

const SFX &SFX::operator=(const SFX &sfx)
{
	v = sfx.v;

	return sfx;
}

const WAV &SFX::operator[](UINT index) throw(...)
{
	return get(index);
}

SFX &operator<<(SFX &sfx, const WAV &_wav)
{
	sfx.v.insert(sfx.v.end(), _wav);

	return sfx;
}

SFX &operator<<(SFX &sfx, const LPVOID lpWAVBuffer) throw(...)
{
	WAV wav;

	if (NULL == lpWAVBuffer)
		throw stdWCexception(L"SFX：WAV内存资源指针无效");

	// TODO: Fill in struct WAV
	wav.lpWAVBuffer = lpWAVBuffer;

	sfx.v.push_back(wav);

	return sfx;
}

SFX &operator<<(SFX &sfx, const pair<LPCWSTR, LPCWSTR> &ResNameTypePair) throw(...)
{
	LPVOID wavbuf = sfx.GetRawWAVBufferW(ResNameTypePair.first, ResNameTypePair.second);

	sfx << wavbuf;

	return sfx;
}

LPVOID SFX::GetRawWAVBufferW(LPCWSTR lpResID, LPCWSTR lpResType) throw(...)
{
	wstring ws;

	HRSRC hResource = FindResourceW(NULL, lpResID, lpResType);
	if (NULL == hResource)
	{
		ws = L"SFX：无法获取WAV声音资源：";
		ws += lpResType;
		ws += L"::";
		ws += lpResID;

		throw stdWCexception(ws);
	}

	HGLOBAL hGlobal = LoadResource(NULL, hResource);
	if (NULL == hGlobal)
	{
		ws = L"SFX：无法装载WAV声音资源：";
		ws += lpResType;
		ws += L"::";
		ws += lpResID;

		throw stdWCexception(ws);
	}

	LPVOID lpRawWAV = LockResource(hGlobal);
	if (NULL == lpRawWAV)
	{
		ws = L"SFX：声音资源 ";
		ws += lpResType;
		ws += L"::";
		ws += lpResID;
		ws += L" 无效";

		throw stdWCexception(ws);
	}

	return lpRawWAV;
}

void SFX::initDS() throw(...)
{
	// TODO: Initliaze Direct Sound
}
