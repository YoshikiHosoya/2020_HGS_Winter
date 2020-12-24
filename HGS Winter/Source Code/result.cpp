//------------------------------------------------------------------------------
//
//���U���g����  [result.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "fade.h"
#include "result.h"
#include "sound.h"
#include "scene2D.h"
#include "multinumber.h"
#include "game_2D.h"

//------------------------------------------------------------------------------
//�}�N��
//------------------------------------------------------------------------------
#define RESULT_SCORE_UI_SIZE		(D3DXVECTOR3(200.0f, 100.0f, 0.0f))					// ���U���g�X�R�A��UI�T�C�Y
#define SURVIVED_TIME_UI_SIZE		(D3DXVECTOR3(200.0f, 100.0f, 0.0f))					// �������Ԃ�UI�T�C�Y
#define KILL_UI_SIZE				(D3DXVECTOR3(200.0f, 100.0f, 0.0f))					// �L����UI�T�C�Y
#define ANY_BUTTON_UI_SIZE			(D3DXVECTOR3(500.0f, 100.0f, 0.0f))					// �G�j�[�{�^����UI�T�C�Y
#define SCORE_SIZE					(D3DXVECTOR3(50.0f, 70.0f, 0.0f))					// �X�R�A��UI�T�C�Y

#define RESULT_SCORE_DIGITS			(7)													// ���U���g�X�R�A�̌���
#define SURVIVED_TIME_DIGITS		(2)													// �������Ԃ̌���
#define NUM_KILL_DIGITS				(3)													// �L�����̌���

//------------------------------------------------------------------------------
//�ÓI�����o�ϐ��̏�����
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�R���X�g���N�^
//------------------------------------------------------------------------------
CResult::CResult()
{
	// ������
	m_nCntResult = 0;
	m_apScene2D.clear();					// ���U���gUI
	m_pResultScore = nullptr;					// ���U���g�X�R�A
	m_pSurvivedTime = nullptr;				// ��������
	m_pNumKill = nullptr;						// �L����
}

//------------------------------------------------------------------------------
//�f�X�g���N�^
//------------------------------------------------------------------------------
CResult::~CResult()
{
	// ������
	m_apScene2D.clear();					// ���U���gUI
	m_pResultScore = nullptr;					// ���U���g�X�R�A
	m_pSurvivedTime = nullptr;				// ��������
	m_pNumKill = nullptr;						// �L����
}

//------------------------------------------------------------------------------
//����������
//------------------------------------------------------------------------------
HRESULT CResult::Init(HWND hWnd)
{
	//�J�������W�ݒ�
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_FIXED);

	// ���U���gUI�̐���
	ResultUICreate();
	// ���U���g�X�R�A�̐���
	ResultScoreCreate();
	// �������Ԃ̐���
	SurvivedTimeCreate();
	// �L�����̐���
	NumKillCreate();

	return S_OK;
}

//------------------------------------------------------------------------------
//�X�V����
//------------------------------------------------------------------------------
void CResult::Update()
{
	//�J�E���^++
	m_nCntResult++;

	// �����L���OUI�̍X�V
	for (int nCnt = 0; nCnt < (int)RESULT_UI::RESULT_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt])
		{
			m_apScene2D[nCnt]->Update();
		}
	}

	//�t�F�[�h���ĂȂ���
	if (CManager::GetRenderer()->GetFade()->GetFade() == CFade::FADE_NONE)
	{
		//�����{�^���������Ƃ�
		if (CHossoLibrary::CheckAnyButton())
		{
				//�X�e�[�g�ύX
				CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RANKING);
				CManager::GetSound()->Play(CSound::LABEL_SE_DECISION);
		}
	}
}

//------------------------------------------------------------------------------
//�`�揈��
//------------------------------------------------------------------------------
void CResult::Draw()
{
	// �����L���OUI�̕`��
	for (int nCnt = 0; nCnt < (int)RESULT_UI::RESULT_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt])
		{
			m_apScene2D[nCnt]->Draw();
		}
	}
}

//------------------------------------------------------------------------------
// ���U���gUI�̐���
//------------------------------------------------------------------------------
void CResult::ResultUICreate()
{
	for (int nCnt = 0; nCnt < (int)RESULT_UI::RESULT_MAX; nCnt++)
	{
		// ���U���g�X�R�A
		if (nCnt == (int)RESULT_UI::RESULT_SCORE)
		{
			// �V�[��2D�̐���
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.25f), 120.0f, 0.0f), RESULT_SCORE_UI_SIZE, CScene::OBJTYPE_UI));
			// �e�N�X�`���̊��蓖��
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_SCORE));
		}
		// ��������
		else if (nCnt == (int)RESULT_UI::SURVIVED_TIME)
		{
			// �V�[��2D�̐���
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.75f), 120.0f, 0.0f), SURVIVED_TIME_UI_SIZE, CScene::OBJTYPE_UI));
			// �e�N�X�`���̊��蓖��
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_TIME));
		}
		// �L��
		else if (nCnt == (int)RESULT_UI::KILL)
		{
			// �V�[��2D�̐���
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), 370.0f, 0.0f), KILL_UI_SIZE, CScene::OBJTYPE_UI));
			// �e�N�X�`���̊��蓖��
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_RESULT_KILL));
		}
		// �G�j�[�{�^��
		else if (nCnt == (int)RESULT_UI::ANY_BUTTON)
		{
			// �V�[��2D�̐���
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), 650.0f, 0.0f), ANY_BUTTON_UI_SIZE, CScene::OBJTYPE_UI));
			// �e�N�X�`���̊��蓖��
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_ENTER));
		}
	}
}

//------------------------------------------------------------------------------
// ���U���g�X�R�A�̐���
//------------------------------------------------------------------------------
void CResult::ResultScoreCreate()
{
	// �X�R�A�̐���
	m_pResultScore = CMultiNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.25f), 220.0f, 0.0f),
		SCORE_SIZE,
		CGame_2D::GetScore(),
		RESULT_SCORE_DIGITS,
		CScene::OBJTYPE_UI);
}

//------------------------------------------------------------------------------
// �������Ԃ̐���
//------------------------------------------------------------------------------
void CResult::SurvivedTimeCreate()
{
	// �X�R�A�̐���
	m_pSurvivedTime = CMultiNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.75f), 220.0f, 0.0f),
		SCORE_SIZE,
		CGame_2D::GetTime(),
		SURVIVED_TIME_DIGITS,
		CScene::OBJTYPE_UI);
}

//------------------------------------------------------------------------------
// �L�����̐���
//------------------------------------------------------------------------------
void CResult::NumKillCreate()
{
	// �X�R�A�̐���
	m_pNumKill = CMultiNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), 470.0f, 0.0f),
		SCORE_SIZE,
		/*CGame_2D::GetScore()*/ 0,
		NUM_KILL_DIGITS,
		CScene::OBJTYPE_UI);
}