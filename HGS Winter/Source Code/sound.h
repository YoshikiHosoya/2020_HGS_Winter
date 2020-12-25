//------------------------------------------------------------------------------
//
// サウンド処理 [sound.h]
// Author : Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _SOUND_H_
#define _SOUND_H_

#define _CRT_SECURE_NO_WARNINGS

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"
#include "xaudio2.h"


class CSound
{
public:

	// サウンドファイル
	typedef enum
	{
		LABEL_BGM_TITLE = 0,						//タイトル
		LABEL_BGM_GAME,								//ゲーム
		LABEL_BGM_RANKING,							//リザルト

		//エフェクト
		LABEL_SE_KILL,
		LABEL_SE_KILL_MANY,
		LABEL_SE_KILL_MANY_MANY,
		LABEL_SE_MITCONDORIA,
		LABEL_SE_RESPAWN,
		LABEL_SE_BOMB_EXOLOSION,

		//システム音
		LABEL_SE_DECISION,						//決定
		LABEL_SE_HIGH_SCORE,
		LABEL_SE_HUNDREDFOLD,
		LABEL_SE_RESULT_IN,
		LABEL_SE_DEATH,							//死
		LABEL_SE_GO,							//死


		LABEL_MAX,
	} LABEL;

	HRESULT Init(HWND hWnd);
	void Uninit(void);

	HRESULT Play(LABEL label);

	void Stop(LABEL label);
	void StopAll(void);

	void SetMute(bool bMute)	{ m_bMute = bMute; };
	bool GetMute()				{ return m_bMute; };

private:

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = NULL;										// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;					// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX] = {};				// ソースボイス
	BYTE *m_apDataAudio[LABEL_MAX] = {};								// オーディオデータ
	DWORD m_aSizeAudio[LABEL_MAX] = {};									// オーディオデータサイズ

	bool m_bMute;														//ミュート

	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;

	// 各音素材のパラメータ
	SOUNDPARAM m_aParam[LABEL_MAX] =
	{
		//BGM
		{ "data/BGM/title.wav", -1 },								//BGM
		{ "data/BGM/game.wav", -1 },								//BGM
		{ "data/BGM/result.wav", -1 },								//BGM

		//エフェクト
		{ "data/SE/Effect/kill.wav", 0 },						//SE 決定
		{ "data/SE/Effect/kill02.wav", 0 },						//SE 決定
		{ "data/SE/Effect/KillMany.wav", 0 },					//SE 決定
		{ "data/SE/Effect/Mitochondria.wav", 0 },				//SE 決定
		{ "data/SE/Effect/Respawn.wav", 0 },					//SE 決定
		{ "data/SE/Effect/BombExplosion.wav", 0 },				//SE 決定

		//システム音
		{ "data/SE/System/pressbutton.wav", 0 },				//SE 決定
		{ "data/SE/System/highscore.wav", 0 },					//SE 決定
		{ "data/SE/System/Hundredfold.wav", 0 },				//SE 決定
		{ "data/SE/System/result02.wav", 0 },					//SE 決定
		{ "data/SE/System/death.wav", 0 },						//SE 決定
		{ "data/SE/System/Go.wav", 0 },							//SE 決定

	};

};


#endif
