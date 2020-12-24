//------------------------------------------------------------------------------
//
//�Q�[������  [game.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "game_2D.h"
#include "manager.h"
#include "renderer.h"
#include "player_2D.h"
#include "fade.h"
#include "keyboard.h"
#include "camera.h"
#include "ParticleManager.h"
#include "multinumber.h"
#include "bg.h"
#include "sound.h"
#include "particle.h"
#include "enemy_2D.h"
#include "bomb.h"
#include "scene2D.h"

//------------------------------------------------------------------------------
//�ÓI�����o�ϐ��̏�����
//------------------------------------------------------------------------------
int CGame_2D::m_nCntTime = 0;		// �J�E���g�^�C��
int CGame_2D::m_nScore = 0;			// �X�R�A

//------------------------------------------------------------------------------
//�}�N��
//------------------------------------------------------------------------------
#define DEFAULT_CREATE_POS (2000.0f)
#define WAY_SIZE (400.0f)
#define BENDING_TIME (4)
#define DEFAULT_TIME (20)
#define COUNTDOWN	(3)

#define SCORE_UI_SIZE				(D3DXVECTOR3(200.0f, 80.0f, 0.0f))					// �X�R�A��UI�T�C�Y
#define TIME_UI_SIZE				(D3DXVECTOR3(200.0f, 80.0f, 0.0f))					// �^�C����UI�T�C�Y
#define CROSS_UI_SIZE				(D3DXVECTOR3(80.0f, 80.0f, 0.0f))					// �o�c��UI�T�C�Y
#define HIGHSCORE_UI_SIZE			(D3DXVECTOR3(200.0f, 80.0f, 0.0f))					// �n�C�X�R�A��UI�T�C�Y
#define SCORE_SIZE					(D3DXVECTOR3(20.0f, 30.0f, 0.0f))					// �X�R�A��UI�T�C�Y
#define SCORE_DIGITS				(7)													// �X�R�A�̌���
#define TIME_DIGITS					(3)													// �^�C���̌���
#define MAGNIFICATION_DIGITS		(2)													// �{���̌���

//------------------------------------------------------------------------------
//�R���X�g���N�^
//------------------------------------------------------------------------------
CGame_2D::CGame_2D()
{
	m_nSpeed = 15;
	m_nScoreMag = 1;
	m_direction = DIRECTION::UP;
	m_nCnt = 0;
	m_nScoreDistance = 0;
	SetScore(0);
	m_bBendingFlag = false;
	m_nTime = 0;
	m_nCntTime = 0;
	m_nScore = 0;
	m_nBendingCountDown = COUNTDOWN;
	m_apScene2D.clear();						// �Q�[��UI
	m_pScoreNumber		= nullptr;				// �X�R�A
	m_pTimeNumber		= nullptr;				// �^�C��
	m_pMagnification	= nullptr;				// �{��
	m_pHighScoreNumber	= nullptr;				// �n�C�X�R�A
}
//------------------------------------------------------------------------------
//�f�X�g���N�^
//------------------------------------------------------------------------------
CGame_2D::~CGame_2D()
{
	m_apScene2D.clear();						// �Q�[��UI
	m_pScoreNumber		= nullptr;				// �X�R�A
	m_pTimeNumber		= nullptr;				// �^�C��
	m_pMagnification	= nullptr;				// �{��
	m_pHighScoreNumber	= nullptr;				// �n�C�X�R�A
}

//------------------------------------------------------------------------------
//����������
//------------------------------------------------------------------------------
HRESULT CGame_2D::Init(HWND hWnd)
{
	//�J�������W�ݒ�
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_DEBUG);

	CGame::SetPlayerPtr(CPlayer_2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 650.0f, 0.0f)));

	//�p�[�e�B�N���̃}�l�[�W��
	CParticleManager::Create();

	// �w�i�̐���
	CBg::Create(65);

	// �Q�[��UI�̐���
	GameUICreate();
	
	// �X�R�A�̐���
	m_pScoreNumber = CMultiNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.2f), 80.0f, 0.0f),
		SCORE_SIZE,
		0,
		SCORE_DIGITS,
		CScene::OBJTYPE_UI);

	// �^�C���̐���
	m_pTimeNumber = CMultiNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.4f), 80.0f, 0.0f),
		SCORE_SIZE,
		0,
		TIME_DIGITS,
		CScene::OBJTYPE_UI);

	// �{���̐���
	m_pMagnification = CMultiNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.63f), 55.0f, 0.0f),
		SCORE_SIZE,
		0,
		MAGNIFICATION_DIGITS,
		CScene::OBJTYPE_UI);

	// �n�C�X�R�A�̐���
	HighScoreCreate();

	//�Q�[���X�e�[�g������
	SetGamestate(CGame::STATE_READY);

	//���Đ�
	CManager::GetSound()->Play(CSound::LABEL_SE_READY);

	CEnemy_2D::Create(D3DXVECTOR3(100.0f, 100.0f, 0.0f), CEnemy_2D::BLUE);

	return S_OK;
}

//------------------------------------------------------------------------------
//�X�V����
//------------------------------------------------------------------------------
void CGame_2D::Update()
{
	// �G�̏o��
	EnemySpawn();

	// �J�E���g�A�b�v�^�C�}�[
	AddTimer();

	// �X�R�A�̒l�̐ݒ� ( �� )
	m_pScoreNumber->SetMultiNumber(m_nScore);

	// �n�C�X�R�A�̍X�V
	HighScoreUpdate();
}
//------------------------------------------------------------------------------
//�`�揈��
//------------------------------------------------------------------------------
void CGame_2D::Draw()
{

}
//------------------------------------------------------------------------------
//�f�o�b�O���\�L
//------------------------------------------------------------------------------
void CGame_2D::ShowDebugInfo()
{
#ifdef _DEBUG
	//�L�[�{�[�h�̃|�C���^
	CKeyboard *pKeyboard = CManager::GetKeyboard();


	CDebugProc::Print(CDebugProc::PLACE_LEFT, "m_nScoreDistance >> %d\n", m_nScoreDistance);
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "m_fNextBendingPoint >> %.2f\n", m_fNextBendingPoint);
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "NextDirection >> %d\n", m_NextBendingDirection);


#endif //_DEBUG
}

//------------------------------------------------------------------------------
//�G�̏o��
//------------------------------------------------------------------------------
void CGame_2D::EnemySpawn()
{
	m_nCnt++;

	CDebugProc::Print(CDebugProc::PLACE_LEFT, "EnemuNum >> %d\n", CEnemy_2D::GetEnemyNum());


	if (m_nCnt % 60 == 0)
	{
		CreateEnemyGroup(SCREEN_CENTER_POS + D3DXVECTOR3(CHossoLibrary::Random(500.0f), CHossoLibrary::Random(250.0f), 0.0f));

		if (m_nCnt % 120 == 0)
		{
			CBomb::Create(SCREEN_CENTER_POS + D3DXVECTOR3(CHossoLibrary::Random(500.0f), CHossoLibrary::Random(250.0f), 0.0f));
		}
	}

}

//------------------------------------------------------------------------------
//�G�̏W�c����
//------------------------------------------------------------------------------
void CGame_2D::CreateEnemyGroup(D3DXVECTOR3 posOrigin)
{
	int nLocalValue = m_nCnt / 120;

	CHossoLibrary::RangeLimit_Equal_Int(nLocalValue, 3, 10);

	CEnemy_2D::Create(posOrigin + D3DXVECTOR3(CHossoLibrary::Random(70.0f), CHossoLibrary::Random(70.0f), 0.0f), CEnemy_2D::RED);


	for (int nCnt = 0; nCnt < nLocalValue ; nCnt++)
	{
		//CEnemy_2D::Create(posOrigin + D3DXVECTOR3(CHossoLibrary::Random(70.0f), CHossoLibrary::Random(70.0f), 0.0f), CEnemy_2D::PURPLE);

		//CEnemy_2D::Create(posOrigin + D3DXVECTOR3(CHossoLibrary::Random(70.0f), CHossoLibrary::Random(70.0f), 0.0f), CEnemy_2D::BLUE);

		if (nLocalValue > 2000)
		{
			CEnemy_2D::Create(posOrigin + D3DXVECTOR3(CHossoLibrary::Random(70.0f), CHossoLibrary::Random(70.0f), 0.0f), CEnemy_2D::PURPLE);
		}
	}
}

//------------------------------------------------------------------------------
//�Q�[��UI�̐���
//------------------------------------------------------------------------------
void CGame_2D::GameUICreate()
{
	for (int nCnt = 0; nCnt < (int)GAME_UI::GAME_UI_MAX; nCnt++)
	{
		// �X�R�A
		if (nCnt == (int)GAME_UI::SCORE)
		{
			// �V�[��2D�̐���
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.2f), 50.0f, 0.0f), SCORE_UI_SIZE, CScene::OBJTYPE_UI));
			// �e�N�X�`���̊��蓖��
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_RANKING_NAME));
		}
		// �^�C��
		else if (nCnt == (int)GAME_UI::TIME)
		{
			// �V�[��2D�̐���
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.4f), 50.0f, 0.0f), TIME_UI_SIZE, CScene::OBJTYPE_UI));
			// �e�N�X�`���̊��蓖��
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_RANKING_NAME));
		}
		// �o�c ( ������ )
		else if (nCnt == (int)GAME_UI::CROSS)
		{
			// �V�[��2D�̐���
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.6f), 60.0f, 0.0f), CROSS_UI_SIZE, CScene::OBJTYPE_UI));
			// �e�N�X�`���̊��蓖��
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_RANKING_NAME));
		}
		// �n�C�X�R�A
		else if (nCnt == (int)GAME_UI::HIGHSCORE)
		{
			// �V�[��2D�̐���
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.8f), 50.0f, 0.0f), HIGHSCORE_UI_SIZE, CScene::OBJTYPE_UI));
			// �e�N�X�`���̊��蓖��
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_RANKING_NAME));
		}
	}
}

//------------------------------------------------------------------------------
//�Q�[���I��
//------------------------------------------------------------------------------
void CGame_2D::GameEnd()
{
	CGame::SetScore(m_nScoreDistance);
	CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RANKING);
	SetGamestate(CGame::STATE_NONE);

}
//------------------------------------------------------------------------------
//�^�C�}�[
//------------------------------------------------------------------------------
void CGame_2D::AddTimer()
{
	m_nTime ++;

	if (m_nTime >= 60)
	{
		m_nTime = 0;
		m_nCntTime++;
	}
	m_pTimeNumber->SetMultiNumber(m_nCntTime);
}

//------------------------------------------------------------------------------
// �n�C�X�R�A�̐���
//------------------------------------------------------------------------------
void CGame_2D::HighScoreCreate()
{
	// �t�@�C���|�C���g
	FILE *pFile;

	char cReadText[128];			// �����Ƃ��ēǂݎ��
	char cHeadText[128];			// ��r�p
	char cDie[128];					// �s�v�ȕ���

	static int nHighScore = 0;

	// �t�@�C�����J��
	pFile = fopen("data/Ranking/RankingData.txt", "r");

	// �J���Ă���Ƃ�
	if (pFile != NULL)
	{
		// SCRIPT������܂Ń��[�v
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile); // �ꕶ�ǂݍ���
			sscanf(cReadText, "%s", &cHeadText);		// ��r�p�e�L�X�g�ɕ�������
		}

		// SCRIPT��������
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// END_SCRIPT������܂Ń��[�v
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile); // �ꕶ�ǂݍ���
				sscanf(cReadText, "%s", &cHeadText);		// ��r�p�e�L�X�g�ɕ�������

				// RANKINGSET��������
				if (strcmp(cHeadText, "RANKINGSET") == 0)
				{
					// END_RANKINGSET������܂Ń��[�v
					while (strcmp(cHeadText, "END_RANKINGSET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile); // �ꕶ�ǂݍ���
						sscanf(cReadText, "%s", &cHeadText);		// ��r�p�e�L�X�g�ɕ�������

						if (strcmp(cHeadText, "RANKING_1st") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nHighScore);		// ��r�p�e�L�X�g��RANKIG_1st����
						}
						else if (strcmp(cHeadText, "END_RANKINGSET") == 0)
						{
							// �n�C�X�R�A�̐���
							m_pHighScoreNumber = CMultiNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.8f), 80.0f, 0.0f),
								SCORE_SIZE,
								nHighScore,
								SCORE_DIGITS,
								CScene::OBJTYPE_UI);
						}
					}
				}
			}
		}
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{
		MessageBox(NULL, "�����L���O�̃f�[�^�ǂݍ��ݎ��s", "�x��", MB_ICONWARNING);
	}

}

//------------------------------------------------------------------------------
//�Q�[���X�e�[�g�ݒ�
//------------------------------------------------------------------------------
void CGame_2D::HighScoreUpdate()
{
	// �X�R�A���n�C�X�R�A�𒴂����Ƃ�
	if (m_pScoreNumber->GettMultiNumber() > m_pHighScoreNumber->GettMultiNumber())
	{
		// �n�C�X�R�A�̏㏑��
		m_pHighScoreNumber->SetMultiNumber(m_pScoreNumber->GettMultiNumber());
	}
}

//------------------------------------------------------------------------------
//�Q�[���X�e�[�g�ݒ�
//------------------------------------------------------------------------------
void CGame_2D::SetGamestate(STATE gamestate)
{
	if (GetGamestate() != gamestate)
	{
		CGame::SetGamestate(gamestate);
		//�X�e�[�g���i��ł���ꍇ

		if (gamestate == CGame::STATE_GAMEOVER)
		{
			m_nCnt = 60;
		}
	}
}
