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
#include "Pad_XInput.h"

//------------------------------------------------------------------------------
//�ÓI�����o�ϐ��̏�����
//------------------------------------------------------------------------------
int CGame_2D::m_nCntTime = 0;
int CGame_2D::m_nScore = 0;

//------------------------------------------------------------------------------
//�}�N��
//------------------------------------------------------------------------------
#define DEFAULT_CREATE_POS (2000.0f)
#define WAY_SIZE (400.0f)
#define BENDING_TIME (4)
#define DEFAULT_TIME (20)
#define COUNTDOWN	(3)

#define SCORE_UI_SIZE				(D3DXVECTOR3(100.0f, 20.0f, 0.0f))					// �X�R�A��UI�T�C�Y
#define TIME_UI_SIZE				(D3DXVECTOR3(100.0f, 20.0f, 0.0f))					// �^�C����UI�T�C�Y
#define CROSS_UI_SIZE				(D3DXVECTOR3(20.0f, 20.0f, 0.0f))					// �o�c��UI�T�C�Y
#define HIGHSCORE_UI_SIZE			(D3DXVECTOR3(150.0f, 20.0f, 0.0f))					// �n�C�X�R�A��UI�T�C�Y
#define SCORE_SIZE					(D3DXVECTOR3(20.0f, 30.0f, 0.0f))					// �X�R�A��UI�T�C�Y
#define SCORE_DIGITS				(8)													// �X�R�A�̌���
#define TIME_DIGITS					(3)													// �^�C���̌���
#define MAGNIFICATION_DIGITS		(3)													// �{���̌���

#define PERPLE_APPEAR_FRAME			(2000)
#define RED_APPEAR_FRAME			(3600)


//------------------------------------------------------------------------------
//�R���X�g���N�^
//------------------------------------------------------------------------------
CGame_2D::CGame_2D()
{
	m_nScore = 0;
	m_nSpeed = 15;
	m_bScoreUpdate = false;
	m_nScoreMag = 1;
	m_direction = DIRECTION::UP;
	m_nCnt = 0;
	m_nScoreDistance = 0;
	SetScore(0);
	m_nTime = 0;
	m_nCntTime = 0;
	m_nBendingCountDown = COUNTDOWN;
	m_apScene2D.clear();						// �Q�[��UI
	m_pScoreNumber		= nullptr;				// �X�R�A
	m_pTimeNumber		= nullptr;				// �^�C��
	m_pMagnification	= nullptr;				// �{��
	m_pHighScoreNumber	= nullptr;				// �n�C�X�R�A
	m_pReady_Go.reset();						// ���f�B�[�S�[
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
	m_pReady_Go.reset();						// ���f�B�[�S�[

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

	//�G�̏�񃊃Z�b�g
	CEnemy_2D::ResetEnemyInfo();

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
	m_pMagnification = CMultiNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.65f), 80.0f, 0.0f),
		SCORE_SIZE,
		0,
		MAGNIFICATION_DIGITS,
		CScene::OBJTYPE_UI);

	// �n�C�X�R�A�̐���
	HighScoreCreate();

	//�Q�[���X�e�[�g������
	SetGamestate(CGame::STATE_READY);

	//���Đ�
	//CManager::GetSound()->Play(CSound::LABEL_SE_READY);

	return S_OK;
}

//------------------------------------------------------------------------------
//�X�V����
//------------------------------------------------------------------------------
void CGame_2D::Update()
{
	switch (GetGamestate())
	{

	case CGame::STATE_READY:
		m_nCnt--;

		if (m_nCnt < 0)
		{
			SetGamestate(STATE_NORMAL);

			CParticle::CreateFromText(SCREEN_CENTER_POS, ZeroVector3, CParticleParam::EFFECT_GAMEOVER, true);


			if (m_pReady_Go)
			{
				m_pReady_Go->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_GAME_GO));
				CManager::GetSound()->Play(CSound::LABEL_SE_GO);

			}
		}
		break;

	case CGame::STATE_NORMAL:

		//nullcheck
		if (m_pReady_Go)
		{
			m_pReady_Go->SetColor(m_pReady_Go->GetColor() -= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.02f));

			if (m_pReady_Go->GetColor().a <= 0)
			{
				m_pReady_Go.reset();
			}
		}

		// �G�̏o��
		EnemySpawn();

		// �J�E���g�A�b�v�^�C�}�[
		AddTimer();

		// �n�C�X�R�A�̍X�V
		HighScoreUpdate();
		break;

	case CGame::STATE_GAMEOVER:
		m_nCnt--;

		CParticle::CreateFromText(GetPlayer()->GetPlayerPos(), ZeroVector3, CParticleParam::EFFECT_GAMEOVER, true);

		GetPlayer()->GetPlayerPos();

		if (m_nCnt < 0)
		{
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RANKING);
		}
		break;

	default:
		break;
	}

	if (CManager::GetKeyboard()->GetTrigger(DIK_L))
	{
		CreateEnemyGroup(SCREEN_CENTER_POS);
	}

#ifdef _DEBUG
#endif // _DEBUG


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

#endif //_DEBUG
}

//------------------------------------------------------------------------------
//�G�̏o��
//------------------------------------------------------------------------------
void CGame_2D::EnemySpawn()
{
	m_nCnt++;

	CDebugProc::Print(CDebugProc::PLACE_LEFT, "EnemuNum >> %d\n", CEnemy_2D::GetEnemyNum());

	if (m_nCnt < 30)
	{
		return;
	}

	if (m_nCnt % 60 == 0)
	{
		CreateEnemyGroup(SCREEN_CENTER_POS + D3DXVECTOR3(CHossoLibrary::Random(600.0f), CHossoLibrary::Random(320.0f), 0.0f));
		CManager::GetSound()->Play(CSound::LABEL_SE_RESPAWN);

		//���Ƃ��Ԃ̃^�C�~���O
		if (m_nCnt == PERPLE_APPEAR_FRAME || m_nCnt == RED_APPEAR_FRAME)
		{
			//�x����
			CManager::GetSound()->Play(CSound::LABEL_SE_WARNING);
		}

		if (m_nCnt > PERPLE_APPEAR_FRAME)
		{
			if (m_nCnt % 180 == 0)
			{
				CEnemy_2D::Create(CHossoLibrary::RandomScreenOutPos(ZeroVector3), CEnemy_2D::PURPLE);
			}
		}

		if (m_nCnt > RED_APPEAR_FRAME)
		{
			CEnemy_2D::Create(SCREEN_CENTER_POS + D3DXVECTOR3(CHossoLibrary::Random(600.0f), CHossoLibrary::Random(320.0f), 0.0f), CEnemy_2D::RED);
		}

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
	int nLocalValue = m_nCnt / 300;

	CHossoLibrary::RangeLimit_Equal_Int(nLocalValue, 1, 10);

	for (int nCnt = 0; nCnt < nLocalValue; nCnt++)
	{
		CEnemy_2D::Create(posOrigin + D3DXVECTOR3(CHossoLibrary::Random(70.0f), CHossoLibrary::Random(70.0f), 0.0f), CEnemy_2D::BLUE);
	}
}

//------------------------------------------------------------------------------
//�X�R�A�{�����Z
//------------------------------------------------------------------------------
void CGame_2D::AddScoreMag()
{
	//�{�����Z
	m_nScoreMag++;

	if (m_nScoreMag % 100 == 0)
	{
		CManager::GetSound()->Play(CSound::LABEL_SE_HUNDREDFOLD);

	}

	//�e�N�X�`���X�V
	m_pMagnification->SetMultiNumber(m_nScoreMag);

}

//------------------------------------------------------------------------------
//�X�R�A���Z
//------------------------------------------------------------------------------
void CGame_2D::AddScore(int nValue)
{
	//�{���v�Z
	m_nScore += nValue * m_nScoreMag;

	//�e�N�X�`���X�V
	m_pScoreNumber->SetMultiNumber(m_nScore);
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
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_SCORE));
		}
		// �^�C��
		else if (nCnt == (int)GAME_UI::TIME)
		{
			// �V�[��2D�̐���
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.4f), 50.0f, 0.0f), TIME_UI_SIZE, CScene::OBJTYPE_UI));
			// �e�N�X�`���̊��蓖��
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_TIME));
		}
		// �o�c ( ������ )
		else if (nCnt == (int)GAME_UI::CROSS)
		{
			// �V�[��2D�̐���
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.6f), 80.0f, 0.0f), CROSS_UI_SIZE, CScene::OBJTYPE_UI));
			// �e�N�X�`���̊��蓖��
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_GAME_CROSS));
		}
		// �n�C�X�R�A
		else if (nCnt == (int)GAME_UI::HIGHSCORE)
		{
			// �V�[��2D�̐���
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.8f), 50.0f, 0.0f), HIGHSCORE_UI_SIZE, CScene::OBJTYPE_UI));
			// �e�N�X�`���̊��蓖��
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_GAME_HIGHSCORE));
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

		//�X�R�A�܂��X�V���Ă��Ȃ�������
		if (!m_bScoreUpdate)
		{
			//�X�V��
			CManager::GetSound()->Play(CSound::LABEL_SE_HIGH_SCORE);

		}

		m_bScoreUpdate = true;
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
		if (gamestate == CGame::STATE_READY)
		{
			m_nCnt = 120;

			m_pReady_Go = CScene2D::Create_Shared(SCREEN_CENTER_POS, D3DXVECTOR3(400.0f, 150.0f, 0.0f), CScene::OBJTYPE_UI);
			m_pReady_Go->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_GAME_READY));
		}

		if (gamestate == CGame::STATE_GAMEOVER)
		{
			m_nCnt = 60;

			//CScene::ReleaseSpecificObject(CScene::OBJTYPE_ENEMY);
			CParticle::CreateFromText(SCREEN_CENTER_POS, ZeroVector3, CParticleParam::EFFECT_IMPACT);
			CManager::GetSound()->Play(CSound::LABEL_SE_DEATH);
			CManager::GetXInput()->StartVibration(40);


		}
	}
}
