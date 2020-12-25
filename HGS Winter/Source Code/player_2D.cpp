//------------------------------------------------------------------------------
//
//�v���C���[2D����  [player_2D.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "player_2D.h"
#include "game_2D.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "particle.h"
#include "Pad_XInput.h"
#include "enemy_2D.h"
#include "bomb.h"
#include "score_up_item.h"
//------------------------------------------------------------------------------
//�ÓI�����o�ϐ��̏�����
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�}�N��
//------------------------------------------------------------------------------
#define PLAYER_SYZE			(D3DXVECTOR3(60.0f, 60.0f, 0.0f))
#define MOVE_SPEED			(2.0f)

//------------------------------------------------------------------------------
//�R���X�g���N�^
//------------------------------------------------------------------------------
CPlayer_2D::CPlayer_2D()
{
	m_bMove = false;
}

//------------------------------------------------------------------------------
//�f�X�g���N�^
//------------------------------------------------------------------------------
CPlayer_2D::~CPlayer_2D()
{

}
//------------------------------------------------------------------------------
//����������
//------------------------------------------------------------------------------
HRESULT CPlayer_2D::Init()
{
	CCharacter_2D::Init();

	//�������m��
	std::unique_ptr<CScene2D> pScene2D = CScene2D::Create_SelfManagement(GetPos(), PLAYER_SYZE);

	//�e�N�X�`���ݒ�
	pScene2D->BindTexture(CTexture::GetTexture(CTexture::TEX_PLAYER));

	//Scene2D�Z�b�g
	SetScene2D(std::move(pScene2D));

	//�d�͖���
	CCharacter::SetGravity(false);

	//�v���C���[�̃|�C���^�ݒ�
	CEnemy_2D::SetPlayerPosPtr(GetPosPtr());
	CBomb::SetPlayerPosPtr(GetPosPtr());
	CScoreUpItem::SetPlayerPosPtr(GetPosPtr());

	//����
	return S_OK;
}

//------------------------------------------------------------------------------
//�X�V����
//------------------------------------------------------------------------------
void CPlayer_2D::Update()
{
	if (CManager::GetGame()->GetGamestate() != CGame::STATE_NORMAL)
	{
		return;
	}

	//�O��
	CParticle::CreateFromText(GetPos(), ZeroVector3, CParticleParam::EFFECT_PLAYER_ORBIT, true);

	//�ړ�����
	MoveInput();

	//�X�V
	CCharacter_2D::Update();
}
//------------------------------------------------------------------------------
//�`�揈��
//------------------------------------------------------------------------------
void CPlayer_2D::Draw()
{
	CCharacter_2D::Draw();
}

//------------------------------------------------------------------------------
//�ړ��̓���
//------------------------------------------------------------------------------
void CPlayer_2D::MoveInput()
{
	m_bMove = false;

	//�Q�[���p�b�h�̏��擾
	CPad_XInput *pGamePad = CManager::GetXInput();

	//�ϐ��錾
	float joypadX, joypadY;

	//�W���C�p�b�h�̈ړ�����
	pGamePad->GetStickLeft(&joypadX, &joypadY);

	if (joypadX != 0 || joypadY != 0)
	{
		GetMove().x += joypadX * MOVE_SPEED / 32768.0f;
		GetMove().y -= joypadY * MOVE_SPEED / 32768.0f;
	}
}

//------------------------------------------------------------------------------
//�f�o�b�O���\�L
//------------------------------------------------------------------------------
void CPlayer_2D::ShowDebugInfo()
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
//�����蔻�菈��
//------------------------------------------------------------------------------
void CPlayer_2D::Collision()
{
	CCharacter_2D::Collision();

}

//------------------------------------------------------------------------------
//�_���[�W�󂯂���̏���
//------------------------------------------------------------------------------
void CPlayer_2D::DamageAction()
{

}
//------------------------------------------------------------------------------
//���S���̏���
//------------------------------------------------------------------------------
void CPlayer_2D::DeathAction()
{
	////�Q�[���I��
	//CManager::GetGame()->SetGamestate(CGame::STATE_GAMECLEAR);
}
//------------------------------------------------------------------------------
//�X�e�[�g�ύX����
//------------------------------------------------------------------------------
void CPlayer_2D::SetState(STATE nextstate)
{
	CCharacter_2D::SetState(nextstate);
	switch (nextstate)
	{
	case CCharacter::STATE_NORMAL:
		break;
	}
}


//------------------------------------------------------------------------------
//��������
//------------------------------------------------------------------------------
std::shared_ptr<CPlayer_2D> CPlayer_2D::Create(D3DXVECTOR3 pos)
{
	//�ϐ��錾
	std::shared_ptr<CPlayer_2D> pPlayer = std::make_shared<CPlayer_2D>();


	if (pPlayer)
	{
		//������
		pPlayer->Init();

		//���W�ݒ�
		pPlayer->SetPos(pos);

		//�I�u�W�F�N�g�^�C�v�ݒ�
		pPlayer->SetObjType(OBJTYPE_PLAYER);

		//���X�g�ɒǉ�
		pPlayer->AddSharedList(pPlayer);

		return pPlayer;
	}

	//�����������
	return nullptr;
}

