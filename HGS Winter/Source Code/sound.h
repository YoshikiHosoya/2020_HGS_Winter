//------------------------------------------------------------------------------
//
// �T�E���h���� [sound.h]
// Author : Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _SOUND_H_
#define _SOUND_H_

#define _CRT_SECURE_NO_WARNINGS

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "main.h"
#include "xaudio2.h"


class CSound
{
public:

	// �T�E���h�t�@�C��
	typedef enum
	{
		LABEL_BGM_TITLE = 0,						//�^�C�g��
		LABEL_BGM_GAME,								//�Q�[��
		LABEL_BGM_RANKING,							//���U���g

		//�G�t�F�N�g
		LABEL_SE_KILL,
		LABEL_SE_KILL_MANY,
		LABEL_SE_KILL_MANY_MANY,
		LABEL_SE_MITCONDORIA,
		LABEL_SE_RESPAWN,
		LABEL_SE_BOMB_EXOLOSION,

		//�V�X�e����
		LABEL_SE_DECISION,						//����
		LABEL_SE_HIGH_SCORE,
		LABEL_SE_HUNDREDFOLD,
		LABEL_SE_RESULT_IN,
		LABEL_SE_DEATH,							//��
		LABEL_SE_GO,							//��


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

	IXAudio2 *m_pXAudio2 = NULL;										// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;					// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX] = {};				// �\�[�X�{�C�X
	BYTE *m_apDataAudio[LABEL_MAX] = {};								// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[LABEL_MAX] = {};									// �I�[�f�B�I�f�[�^�T�C�Y

	bool m_bMute;														//�~���[�g

	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} SOUNDPARAM;

	// �e���f�ނ̃p�����[�^
	SOUNDPARAM m_aParam[LABEL_MAX] =
	{
		//BGM
		{ "data/BGM/title.wav", -1 },								//BGM
		{ "data/BGM/game.wav", -1 },								//BGM
		{ "data/BGM/result.wav", -1 },								//BGM

		//�G�t�F�N�g
		{ "data/SE/Effect/kill.wav", 0 },						//SE ����
		{ "data/SE/Effect/kill02.wav", 0 },						//SE ����
		{ "data/SE/Effect/KillMany.wav", 0 },					//SE ����
		{ "data/SE/Effect/Mitochondria.wav", 0 },				//SE ����
		{ "data/SE/Effect/Respawn.wav", 0 },					//SE ����
		{ "data/SE/Effect/BombExplosion.wav", 0 },				//SE ����

		//�V�X�e����
		{ "data/SE/System/pressbutton.wav", 0 },				//SE ����
		{ "data/SE/System/highscore.wav", 0 },					//SE ����
		{ "data/SE/System/Hundredfold.wav", 0 },				//SE ����
		{ "data/SE/System/result02.wav", 0 },					//SE ����
		{ "data/SE/System/death.wav", 0 },						//SE ����
		{ "data/SE/System/Go.wav", 0 },							//SE ����

	};

};


#endif
