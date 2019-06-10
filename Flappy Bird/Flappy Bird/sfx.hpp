#pragma once

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")

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
	DSBUFFERDESC DSBufDesc = { 0 };
	LPDIRECTSOUNDBUFFER lpDSBuf;
	LPVOID lpWAVBuffer = NULL;
	DWORD dwSzRes = 0L;
};

struct WAV_HEADER
{
	char    riff_sig[4];
	long    waveform_chunk_size;
	char    wave_sig[4];
	char    format_sig[4];
	long    format_chunk_size;
	short   format_tag;
	short   channels;
	long    sample_rate;
	long    bytes_per_sec;
	short   block_align;
	short   bits_per_sample;
	char    data_sig[4];
	long    data_size;
};

class SFX
{
public:
	SFX(const HWND hWnd) throw(...);
	SFX(const SFX &sfx);
	SFX(const WAV _wav[], UINT n, const HWND hWnd) throw(...);
	SFX(LPCWSTR wcResName[], LPCWSTR wcResType, UINT n, const HWND hWnd) throw(...);
	void init(const WAV _wav[], const UINT n, const HWND hWnd) throw(...);
	void init(LPCWSTR wcResName[], LPCWSTR wcResType, UINT n, const HWND hWnd) throw(...);
	void clear();
	const WAV &get(UINT n) throw(...);
	void erase(UINT n) throw(...);
	void play(UINT index) throw(...);
	const SFX operator+(const SFX &sfx) throw(...);
	const SFX &operator+=(const SFX &sfx);
	const SFX &operator=(const SFX &sfx);
	const WAV &operator[](UINT index) throw(...);
	friend SFX &operator<<(SFX &sfx, const WAV &_wav);
	friend SFX &operator<<(SFX &sfx, const pair<LPCWSTR, LPCWSTR> &ResNameTypePair) throw(...);

private:
	vector<WAV> v;

	LPDIRECTSOUND lpDS;
	HWND hWnd;

	LPVOID GetRawWAVBufferW(LPCWSTR lpResID, LPCWSTR lpResType, WAV *wav) throw(...);
	WAV *GetWAVInfo(WAV *wav) throw(...);
	void initDS() throw(...);
};

const DWORD playOffsetSkip = 100;
