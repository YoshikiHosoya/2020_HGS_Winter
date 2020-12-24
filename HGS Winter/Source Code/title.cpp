//------------------------------------------------------------------------------
//
//�^�C�g������  [title.cpp]
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
#include "title.h"
#include "scene2D.h"
#include "bg.h"
#include "sound.h"
//------------------------------------------------------------------------------
//�}�N��
//------------------------------------------------------------------------------
#define DEMOPLAY_FADE_COUNT		(1000)		//�f���v���C�ɍs���܂ł̃J�E���^

#define TITLE_LOGO_SIZE			(D3DXVECTOR3(900.0f, 400.0f, 0.0f))

#define ENEMY_SYZE				(D3DXVECTOR3(60.0f, 60.0f, 0.0f))			// �G�̃T�C�Y

//------------------------------------------------------------------------------
//�ÓI�����o�ϐ��̏�����
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�R���X�g���N�^
//------------------------------------------------------------------------------
CTitle::CTitle()
{
	m_apScene2D.clear();
	m_apEnemy.clear();

	m_nCntState = DEMOPLAY_FADE_COUNT;
	m_titlestate = STATE_START;
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		m_EnemyMove[nCnt] = ZeroVector3;		// �G�̈ړ���
	}
}

//------------------------------------------------------------------------------
//�f�X�g���N�^
//------------------------------------------------------------------------------
CTitle::~CTitle()
{
	m_apScene2D.clear();
	m_apEnemy.clear();
}

//------------------------------------------------------------------------------
//����������
//------------------------------------------------------------------------------
HRESULT CTitle::Init(HWND hWnd)
{
	//�J�������W�ݒ�
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_DEBUG);

	// �G�̐���
	EnemyCreate();

	// �^�C�g��UI�̐���
	TitleUICreate();

	CBg::Create(95);
	return S_OK;
}

//------------------------------------------------------------------------------
//�X�V����
//------------------------------------------------------------------------------
void CTitle::Update()
{
	//�J�E���g�_�E��
	m_nCntState--;

	// �G�̍X�V
	EnemyUpdate();

	// �^�C�g��UI�̍X�V
	for (int nCnt = 0; nCnt < (int)TITLE_UI::TITLE_MAX; nCnt++)
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
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_2DGAME);
			CManager::GetSound()->Play(CSound::LABEL_SE_DECISION);
		}
	}

}

//------------------------------------------------------------------------------
//�`�揈��
//------------------------------------------------------------------------------
void CTitle::Draw()
{
	// �G�̕`��
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (m_apEnemy[nCnt])
		{
			m_apEnemy[nCnt]->Draw();
		}
	}

	// �^�C�g��UI�̕`��
	for (int nCnt = 0; nCnt < (int)TITLE_UI::TITLE_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt])
		{
			m_apScene2D[nCnt]->Draw();
		}
	}
}

//------------------------------------------------------------------------------
//�^�C�g��UI�̐���
//------------------------------------------------------------------------------
void CTitle::TitleUICreate()
{
	for (int nCnt = 0; nCnt < (int)TITLE_UI::TITLE_MAX; nCnt++)
	{
		if (nCnt == (int)TITLE_UI::TITLE_BLACK)
		{
			// �V�[��2D�̐���
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT, 0.0f), CScene::OBJTYPE_UI));
			// �e�N�X�`���̊��蓖��
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_NONE));
			// �F�ύX
			m_apScene2D[nCnt]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		}

		if (nCnt == (int)TITLE_UI::TITLE_NAME)
		{
			// �V�[��2D�̐���
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), 200.0f, 0.0f), TITLE_LOGO_SIZE, CScene::OBJTYPE_UI));
			// �e�N�X�`���̊��蓖��
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_TITLE_NAME));
		}

		if (nCnt == (int)TITLE_UI::TITLE_ENTER)
		{
			// �V�[��2D�̐���
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), 600.0f, 0.0f), D3DXVECTOR3(500.0f, 100.0f, 0.0f), CScene::OBJTYPE_UI));
			// �e�N�X�`���̊��蓖��
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_ENTER));
		}
	}
}

//------------------------------------------------------------------------------
// �G�̐���
//------------------------------------------------------------------------------
void CTitle::EnemyCreate()
{
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		switch (nCnt)
		{
		case 0:
			// �V�[��2D�̐���
			m_apEnemy.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.2f), SCREEN_HEIGHT * 0.2f, 0.0f), ENEMY_SYZE, CScene::OBJTYPE_UI));
			// �e�N�X�`���̊��蓖��
			m_apEnemy[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_ENEMY_BLUE));
			// �G�̈ړ���
			m_EnemyMove[nCnt] = D3DXVECTOR3(2.0f, 4.0f, 0.0f);
			break;

		case 1:
			// �V�[��2D�̐���
			m_apEnemy.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.6f), 400.0f, 0.0f), ENEMY_SYZE, CScene::OBJTYPE_UI));
			// �e�N�X�`���̊��蓖��
			m_apEnemy[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_ENEMY_PURPLE));
			// �G�̈ړ���
			m_EnemyMove[nCnt] = D3DXVECTOR3(4.0f, 3.0f, 0.0f);
			break;

		case 2:
			// �V�[��2D�̐���
			m_apEnemy.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.9f), 600.0f, 0.0f), ENEMY_SYZE, CScene::OBJTYPE_UI));
			// �e�N�X�`���̊��蓖��
			m_apEnemy[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_ENEMY_RED));
			// �G�̈ړ���
			m_EnemyMove[nCnt] = D3DXVECTOR3(6.0f, 7.0f, 0.0f);
			break;

		default:
			break;
		}
	}
}

//------------------------------------------------------------------------------
// �G�̍X�V
//------------------------------------------------------------------------------
void CTitle::EnemyUpdate()
{
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (m_apEnemy[nCnt])
		{
			// �ړ�
			m_apEnemy[nCnt]->GetPos() += m_EnemyMove[nCnt];

			// ����
			if (CHossoLibrary::RangeLimit_Equal_Float(m_apEnemy[nCnt]->GetPos().x, 0, SCREEN_WIDTH))
			{
				m_EnemyMove[nCnt].x *= -1;
			}
			if (CHossoLibrary::RangeLimit_Equal_Float(m_apEnemy[nCnt]->GetPos().y, 0, SCREEN_HEIGHT))
			{
				m_EnemyMove[nCnt].y *= -1;
			}

			// �X�V
			m_apEnemy[nCnt]->Update();
		}
	}
}