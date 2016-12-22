#include "stdafx.h"

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "Comctl32.lib")
#pragma comment (lib, "msimg32.lib")
#pragma comment (lib, "msacm32.lib")

BOOL ReadMP3File(LPTSTR lpszFileName, LPMPEGLAYER3WAVEFORMAT lpmf, LPBYTE *lplpData, LPDWORD lpdwSize)
{
    HMMIO    hmmio;
    MMRESULT mmr;
    MMCKINFO mmckRiff;
    MMCKINFO mmckFmt;
    MMCKINFO mmckData;

    hmmio = mmioOpen(lpszFileName, NULL, MMIO_READ);
    if (hmmio == NULL) {
        MessageBox(NULL, TEXT("ファイルのオープンに失敗しました。"), NULL, MB_ICONWARNING);
        return FALSE;
    }

    mmckRiff.fccType = mmioStringToFOURCC(TEXT("WAVE"), 0);
    mmr = mmioDescend(hmmio, &mmckRiff, NULL, MMIO_FINDRIFF);
    if (mmr != MMSYSERR_NOERROR) {
        mmioClose(hmmio, 0);
        return FALSE;
    }

    mmckFmt.ckid = mmioStringToFOURCC(TEXT("fmt "), 0);
    mmioDescend(hmmio, &mmckFmt, &mmckRiff, MMIO_FINDCHUNK);
    mmioRead(hmmio, (HPSTR)lpmf, mmckFmt.cksize);
    mmioAscend(hmmio, &mmckFmt, 0);
    if (lpmf->wfx.wFormatTag != WAVE_FORMAT_MPEGLAYER3) {
        MessageBox(NULL, TEXT("RIFF/WAVE形式のMP3ファイルではありません。"), NULL, MB_ICONWARNING);
        mmioClose(hmmio, 0);
        return FALSE;
    }

    mmckData.ckid = mmioStringToFOURCC(TEXT("data"), 0);
    mmioDescend(hmmio, &mmckData, &mmckRiff, MMIO_FINDCHUNK);
    *lplpData = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, mmckData.cksize);
    mmioRead(hmmio, (HPSTR)*lplpData, mmckData.cksize);
    mmioAscend(hmmio, &mmckData, 0);

    mmioAscend(hmmio, &mmckRiff, 0);
    mmioClose(hmmio, 0);

    *lpdwSize = mmckData.cksize;

    return TRUE;
}

BOOL DecodeToWave(LPWAVEFORMATEX lpwfSrc, LPBYTE lpSrcData, DWORD dwSrcSize, LPWAVEFORMATEX lpwfDest, LPBYTE *lplpDestData, LPDWORD lpdwDestSize)
{
    HACMSTREAM      has;
    ACMSTREAMHEADER ash;
    LPBYTE          lpDestData;
    DWORD           dwDestSize;
    BOOL            bResult;

    lpwfDest->wFormatTag = WAVE_FORMAT_PCM;
    acmFormatSuggest(NULL, lpwfSrc, lpwfDest, sizeof(WAVEFORMATEX), ACM_FORMATSUGGESTF_WFORMATTAG);

    if (acmStreamOpen(&has, NULL, lpwfSrc, lpwfDest, NULL, 0, 0, ACM_STREAMOPENF_NONREALTIME) != 0) {
        MessageBox(NULL, TEXT("変換ストリームのオープンに失敗しました。"), NULL, MB_ICONWARNING);
        return FALSE;
    }

    acmStreamSize(has, dwSrcSize, &dwDestSize, ACM_STREAMSIZEF_SOURCE);
    lpDestData = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, dwDestSize);

    ZeroMemory(&ash, sizeof(ACMSTREAMHEADER));
    ash.cbStruct = sizeof(ACMSTREAMHEADER);
    ash.pbSrc = lpSrcData;
    ash.cbSrcLength = dwSrcSize;
    ash.pbDst = lpDestData;
    ash.cbDstLength = dwDestSize;

    acmStreamPrepareHeader(has, &ash, 0);
    bResult = acmStreamConvert(has, &ash, 0) == 0;
    acmStreamUnprepareHeader(has, &ash, 0);

    acmStreamClose(has, 0);

    if (bResult) {
        *lplpDestData = lpDestData;
        *lpdwDestSize = ash.cbDstLengthUsed;
    }
    else {
        MessageBox(NULL, TEXT("変換に失敗しました。"), NULL, MB_ICONWARNING);
        *lplpDestData = NULL;
        *lpdwDestSize = 0;
        HeapFree(GetProcessHeap(), 0, lpDestData);
    }

    return bResult;
}


BOOL ReadWaveFile(LPTSTR lpszFileName, LPWAVEFORMATEX lpwf, LPBYTE *lplpData, LPDWORD lpdwDataSize)
{
    HMMIO    hmmio;
    MMCKINFO mmckRiff;
    MMCKINFO mmckFmt;
    MMCKINFO mmckData;
    LPBYTE   lpData;

    hmmio = mmioOpen(lpszFileName, NULL, MMIO_READ);
    if (hmmio == NULL) {
        MessageBox(NULL, TEXT("ファイルのオープンに失敗しました。"), NULL, MB_ICONWARNING);
        return FALSE;
    }

    mmckRiff.fccType = mmioStringToFOURCC(TEXT("WAVE"), 0);
    if (mmioDescend(hmmio, &mmckRiff, NULL, MMIO_FINDRIFF) != MMSYSERR_NOERROR) {
        MessageBox(NULL, TEXT("WAVEファイルではありません。"), NULL, MB_ICONWARNING);
        mmioClose(hmmio, 0);
        return FALSE;
    }

    mmckFmt.ckid = mmioStringToFOURCC(TEXT("fmt "), 0);
    if (mmioDescend(hmmio, &mmckFmt, NULL, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
        mmioClose(hmmio, 0);
        return FALSE;
    }
    mmioRead(hmmio, (HPSTR)lpwf, mmckFmt.cksize);
    mmioAscend(hmmio, &mmckFmt, 0);
    if (lpwf->wFormatTag != WAVE_FORMAT_PCM) {
        MessageBox(NULL, TEXT("PCMデータではありません。"), NULL, MB_ICONWARNING);
        mmioClose(hmmio, 0);
        return FALSE;
    }

    mmckData.ckid = mmioStringToFOURCC(TEXT("data"), 0);
    if (mmioDescend(hmmio, &mmckData, NULL, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
        mmioClose(hmmio, 0);
        return FALSE;
    }
    lpData = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, mmckData.cksize);
    mmioRead(hmmio, (HPSTR)lpData, mmckData.cksize);
    mmioAscend(hmmio, &mmckData, 0);

    mmioAscend(hmmio, &mmckRiff, 0);
    mmioClose(hmmio, 0);

    *lplpData = lpData;
    *lpdwDataSize = mmckData.cksize;

    return TRUE;
}