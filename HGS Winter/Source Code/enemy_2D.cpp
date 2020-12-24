//------------------------------------------------------------------------------
//
//�G�l�~�[2D����  [enemy_2D.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "enemy_2D.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "particle.h"

//------------------------------------------------------------------------------
//�ÓI�����o�ϐ��̏�����
//------------------------------------------------------------------------------
D3DXVECTOR3 *CEnemy_2D::m_pPlayerPos = nullptr;

//------------------------------------------------------------------------------
//�}�N��
//------------------------------------------------------------------------------
#define ENEMY_SYZE			(D3DXVECTOR3(80.0f, 80.0f, 0.0f))
#define MOVESPEED_BLUE		(1.8f)
#define MOVESPEED_PURPLE	(2.5f)
#define MOVESPEED_RED		(3.0f)


//------------------------------------------------------------------------------
//�R���X�g���N�^
//------------------------------------------------------------------------------
CEnemy_2D::CEnemy_2D()
{
	m_bMove = false;
}

//------------------------------------------------------------------------------
//�f�X�g���N�^
//------------------------------------------------------------------------------
CEnemy_2D::~CEnemy_2D()
{

}
//------------------------------------------------------------------------------
//����������
//------------------------------------------------------------------------------
HRESULT CEnemy_2D::Init()
{
	CCharacter_2D::Init();

	//�������m��
	std::unique_ptr<CScene2D> pScene2D = CScene2D::Create_SelfManagement(GetPos(), ENEMY_SYZE);

	//�e�N�X�`���ݒ�
	pScene2D->BindTexture(CTexture::GetTexture(CTexture::TEX_PLAYER));

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
void CEnemy_2D::Update()
{
	//���W�̍����Z�o
	m_DifPos = *m_pPlayerPos - GetPos();

	//�ړ��̂`�h
	MoveAI();

	//�X�V
	CCharacter_2D::Update();
}
//------------------------------------------------------------------------------
//�`�揈��
//------------------------------------------------------------------------------
void CEnemy_2D::Draw()
{
	CCharacter_2D::Draw();
}


//------------------------------------------------------------------------------
//�f�o�b�O���\�L
//------------------------------------------------------------------------------
void CEnemy_2D::ShowDebugInfo()
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
//�����蔻��
//------------------------------------------------------------------------------
void CEnemy_2D::Collision()
{
	float fLocalDistance = D3DXVec3Length(&m_DifPos);

	CDebugProc::Print(CDebugProc::PLACE_LEFT, "fLocalDistance >> %.2f\n", fLocalDistance);

	if (fLocalDistance < 50.0f)
	{
		CDebugProc::Print(CDebugProc::PLACE_LEFT, "DEATH\n");
	}

}

//------------------------------------------------------------------------------
//�_���[�W�󂯂���̏���
//------------------------------------------------------------------------------
void CEnemy_2D::DamageAction()
{

}
//------------------------------------------------------------------------------
//���S���̏���
//------------------------------------------------------------------------------
void CEnemy_2D::DeathAction()
{
	////�Q�[���I��
	//CManager::GetGame()->SetGamestate(CGame::STATE_GAMECLEAR);
}
//------------------------------------------------------------------------------
//�X�e�[�g�ύX����
//------------------------------------------------------------------------------
void CEnemy_2D::SetState(STATE nextstate)
{
	CCharacter_2D::SetState(nextstate);
	switch (nextstate)
	{
	case CCharacter::STATE_NORMAL:
		break;
	}
}

//------------------------------------------------------------------------------
//�^�C�v�ݒ�
//------------------------------------------------------------------------------
void CEnemy_2D::SetType(ENEMY_TYPE type)
{
	m_type = type;

	switch (m_type)
	{
	case CEnemy_2D::BLUE:
		GetScene2DPtr()->BindTexture(CTexture::GetTexture(CTexture::TEX_ENEMY_BLUE));

		break;
	case CEnemy_2D::RED:
		GetScene2DPtr()->BindTexture(CTexture::GetTexture(CTexture::TEX_ENEMY_RED));

		break;
	case CEnemy_2D::PURPLE:
		GetScene2DPtr()->BindTexture(CTexture::GetTexture(CTexture::TEX_ENEMY_PURPLE));

		break;
	default:
		break;
	}


}


//------------------------------------------------------------------------------
//��������
//------------------------------------------------------------------------------
std::shared_ptr<CEnemy_2D> CEnemy_2D::Create(D3DXVECTOR3 pos,CEnemy_2D::ENEMY_TYPE type)
{
	//�ϐ��錾
	std::shared_ptr<CEnemy_2D> pEnemy = std::make_shared<CEnemy_2D>();


	if (pEnemy)
	{
		//������
		pEnemy->Init();

		//���W�ݒ�
		pEnemy->SetPos(pos);

		//�^�C�v�ݒ�
		pEnemy->SetType(type);

		//�I�u�W�F�N�g�^�C�v�ݒ�
		pEnemy->SetObjType(OBJTYPE_PLAYER);

		//���X�g�ɒǉ�
		pEnemy->AddSharedList(pEnemy);

		return pEnemy;
	}

	//�����������
	return nullptr;
}

//------------------------------------------------------------------------------
//�ړ�����
//------------------------------------------------------------------------------
void CEnemy_2D::MoveAI()
{
	D3DXVECTOR3 vec = ZeroVector3;

	switch (m_type)
	{
	case CEnemy_2D::BLUE:
		vec = *D3DXVec3Normalize(&vec, &m_DifPos);

		GetMove() += vec * MOVESPEED_BLUE;

		break;
	case CEnemy_2D::RED:
		vec = *D3DXVec3Normalize(&vec, &m_DifPos);

		GetMove() += vec * MOVESPEED_RED;

		break;
	case CEnemy_2D::PURPLE:
		vec = *D3DXVec3Normalize(&vec, &m_DifPos);

		GetMove() += vec * MOVESPEED_PURPLE;
		GetRot().z += 0.01f;

		D3DXVECTOR3;

		break;

	default:
		break;
	}


}

