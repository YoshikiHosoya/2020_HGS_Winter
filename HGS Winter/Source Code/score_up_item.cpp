//------------------------------------------------------------------------------
//
//�v���C���[2D����  [player_2D.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "score_up_item.h"
#include "game_2D.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "particle.h"
#include "Pad_XInput.h"
#include "enemy_2D.h"
#include "player_2D.h"
//------------------------------------------------------------------------------
//�ÓI�����o�ϐ��̏�����
//------------------------------------------------------------------------------
D3DXVECTOR3 *CScoreUpItem::m_pPlayerPos = nullptr;

//------------------------------------------------------------------------------
//�}�N��
//------------------------------------------------------------------------------
#define ITEM_SIZE			(D3DXVECTOR3(15.0f, 15.0f, 0.0f))
#define TRACKING_LENGTH		(100.0f)
#define GETITEM_LENGTH		(30.0f)
#define TRACKING_SPEED		(7.0f)
#define DEFAULT_LIFE		(300)

//------------------------------------------------------------------------------
//�R���X�g���N�^
//------------------------------------------------------------------------------
CScoreUpItem::CScoreUpItem()
{
	m_bMove = false;
	m_fRotation = CHossoLibrary::Random(0.05f);
	SetLife(DEFAULT_LIFE);
}

//------------------------------------------------------------------------------
//�f�X�g���N�^
//------------------------------------------------------------------------------
CScoreUpItem::~CScoreUpItem()
{

}
//------------------------------------------------------------------------------
//����������
//------------------------------------------------------------------------------
HRESULT CScoreUpItem::Init()
{
	CCharacter_2D::Init();

	//�������m��
	std::unique_ptr<CScene2D> pScene2D = CScene2D::Create_SelfManagement(GetPos(), ITEM_SIZE);

	//�e�N�X�`���ݒ�
	pScene2D->BindTexture(CTexture::GetTexture(CTexture::TEX_ITEM_MITOKONDORIA));

	//Scene2D�Z�b�g
	SetScene2D(std::move(pScene2D));

	//�d�͖���
	CCharacter::SetGravity(false);

	//����
	return S_OK;
}

//------------------------------------------------------------------------------
//�X�V����
//------------------------------------------------------------------------------
void CScoreUpItem::Update()
{
	GetRot().z += m_fRotation;

	//���C�t����
	SetLife(GetLife() - 1);

	//���C�t�����ȉ��ɂȂ�����
	if ((GetLife() <= 50))
	{
		//���X�ɓ���
		GetScene2DPtr()->SetColor(GetScene2DPtr()->GetColor() -= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.02f));

		if (GetLife() <= 0)
		{
			Release();
		}
	}


	//�X�V
	CCharacter_2D::Update();
}
//------------------------------------------------------------------------------
//�`�揈��
//------------------------------------------------------------------------------
void CScoreUpItem::Draw()
{
	CCharacter_2D::Draw();
}

//------------------------------------------------------------------------------
//�f�o�b�O���\�L
//------------------------------------------------------------------------------
void CScoreUpItem::ShowDebugInfo()
{
#ifdef _DEBUG
	//�v���C���[�����
	if (ImGui::TreeNode("PlayerInfo"))
	{
		//�L�����N�^�[���\��
		ShowCharacterInfo();

		ImGui::TreePop();
	}
#endif //DEBUG
}

//------------------------------------------------------------------------------
//�_���[�W�󂯂���̏���
//------------------------------------------------------------------------------
void CScoreUpItem::DamageAction()
{

}
//------------------------------------------------------------------------------
//���S���̏���
//------------------------------------------------------------------------------
void CScoreUpItem::DeathAction()
{
	////�Q�[���I��
	//CManager::GetGame()->SetGamestate(CGame::STATE_GAMECLEAR);
}
//------------------------------------------------------------------------------
//�X�e�[�g�ύX����
//------------------------------------------------------------------------------
void CScoreUpItem::SetState(STATE nextstate)
{
	CCharacter_2D::SetState(nextstate);
	switch (nextstate)
	{
	case CCharacter::STATE_NORMAL:
		break;
	}
}

//------------------------------------------------------------------------------
//�����蔻�菈��
//------------------------------------------------------------------------------
void CScoreUpItem::Collision()
{
	D3DXVECTOR3 DifPos = *m_pPlayerPos - GetPos();
	float fDistance = D3DXVec3Length(&DifPos);

	if (fDistance < TRACKING_LENGTH)
	{

		if (fDistance < GETITEM_LENGTH)
		{
			CGame_2D *pGame2D = (CGame_2D*)CManager::GetGame();
			pGame2D->AddScoreMag();
			CManager::GetSound()->Play(CSound::LABEL_SE_MITCONDORIA);

			Release();
		}
		else
		{
			D3DXVec3Normalize(&DifPos, &DifPos);
			GetMove() += DifPos * TRACKING_SPEED;
		}
	}


	CCharacter_2D::Collision();

}

//------------------------------------------------------------------------------
//��������
//------------------------------------------------------------------------------
std::shared_ptr<CScoreUpItem> CScoreUpItem::Create(D3DXVECTOR3 pos)
{
	//�ϐ��錾
	std::shared_ptr<CScoreUpItem> pMitocondoria = std::make_shared<CScoreUpItem>();


	if (pMitocondoria)
	{
		//������
		pMitocondoria->Init();

		//���W�ݒ�
		pMitocondoria->SetPos(pos);

		//�I�u�W�F�N�g�^�C�v�ݒ�
		pMitocondoria->SetObjType(OBJTYPE_MITCONDORIA);

		pMitocondoria->GetScene2DPtr()->SetPos(pos);
		pMitocondoria->GetScene2DPtr()->Update();


		//���X�g�ɒǉ�
		pMitocondoria->AddSharedList(pMitocondoria);

		return pMitocondoria;
	}

	//�����������
	return nullptr;
}

