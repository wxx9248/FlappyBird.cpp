#pragma once

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")

#include <vector>
#include <string>
#include <mmsystem.h>
#include <dsound.h>
#include <Windows.h>
#include "stdWCexception.hpp"

using std::vector;
using std::pair;
using std::wstring;

struct WAV
{
	WAVEFORMATEX wfmex = { 0 };
	LPVOID lpWAVBuffer = NULL;
};

class SFX
{
public:
	SFX();
	SFX(const SFX &sfx);
	SFX(const WAV _wav[], UINT n);
	SFX(const LPVOID lpWAVBuffer[], UINT n) throw(...);
	SFX(LPCWSTR wcResName[], LPCWSTR wcResType, UINT n) throw(...);
	void init(const WAV _wav[], const UINT n);
	void init(const LPVOID lpWAVBuffer[], const UINT n) throw(...);
	void init(LPCWSTR wcResName[], LPCWSTR wcResType, UINT n) throw(...);
	void init(const pair<LPCWSTR, LPCWSTR> ResNameTypePair[], UINT n) throw(...);
	void clear();
	const WAV &get(UINT n) throw(...);
	void erase(UINT n) throw(...);
	void play(UINT index) throw(...);
	const SFX operator+(const SFX &sfx);
	const SFX &operator+=(const SFX &sfx);
	const SFX &operator=(const SFX &sfx);
	const WAV &operator[](UINT index) throw(...);
	friend SFX &operator<<(SFX &sfx, const WAV &_wav);
	friend SFX &operator<<(SFX &sfx, const LPVOID lpWAVBuffer) throw(...);
	friend SFX &operator<<(SFX &sfx, const pair<LPCWSTR, LPCWSTR> &ResNameTypePair) throw(...);

private:
	vector<WAV> v;

	LPVOID GetRawWAVBufferW(LPCWSTR lpResID, LPCWSTR lpResType) throw(...);
	void initDS() throw(...);
};
