//------------------------------------------------------------------------------
//
//�L�����N�^�[����  [character.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "character.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "game.h"
#include "collision.h"
#include "camera.h"
#include "Map.h"
//------------------------------------------------------------------------------
//�ÓI�����o�ϐ��̏�����
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�}�N��
//------------------------------------------------------------------------------
#define INERTIA	(0.2f)			//���C
#define AIR_INERTIA (0.05f)		//�󒆎��̖��C
#define GRAVITY (0.7f)			//�d��
//------------------------------------------------------------------------------
//�R���X�g���N�^
//------------------------------------------------------------------------------
CCharacter::CCharacter()
{
	m_pos = ZeroVector3;
	m_posOld = ZeroVector3;
	m_move = ZeroVector3;
	m_rot = ZeroVector3;
	m_nLife = 1;
	m_bAttack = false;
	m_bJump = false;
	m_bInvincible = false;
	m_bGravity = true;
	m_bAlive = true;
	m_nCntState = 0;
	m_State = STATE_NORMAL;
	m_Param = PARAM_PLAYER;
}

//------------------------------------------------------------------------------
//�f�X�g���N�^
//------------------------------------------------------------------------------
CCharacter::~CCharacter()
{
	if (m_pCollision)
	{
		m_pCollision.reset();
	}
}

//------------------------------------------------------------------------------
//�f�o�b�O���\�L
//------------------------------------------------------------------------------
void CCharacter::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}

//------------------------------------------------------------------------------
//�_���[�W����
//------------------------------------------------------------------------------
bool CCharacter::ApplyDamage(int nDamage)
{
	m_nLife -= nDamage;

	if (m_nLife < 0)
	{
		DeathAction();
		Release();

	}
	else
	{
		DamageAction();
	}
	return false;
}

//------------------------------------------------------------------------------
//�X�e�[�g�ݒ�
//------------------------------------------------------------------------------
void CCharacter::SetState(STATE nextstate)
{
	m_State = nextstate;
}

//------------------------------------------------------------------------------
//�p�����[�^�ݒ�
//------------------------------------------------------------------------------
void CCharacter::SetParam(PARAM param)
{
	//�p�����[�^�ݒ�
	m_Param = param;
}

//------------------------------------------------------------------------------
//�L�����N�^�[���\��
//------------------------------------------------------------------------------
void CCharacter::ShowCharacterInfo()
{
#ifdef _DEBUG
	//�L�����N�^�[�����
	if (ImGui::TreeNode("CharacterInfo"))
	{
		ImGui::Text("pos %.2f,%.2f,,%.2f", m_pos.x, m_pos.y, m_pos.z);
		ImGui::Text("move %.2f,%.2f,,%.2f", m_move.x, m_move.y, m_move.z);
		ImGui::Text("rot %.2f,%.2f,,%.2f", m_rot.x, m_rot.y, m_rot.z);


		ImGui::Text("bJump [%d] bInvincible [%d]", m_bJump, m_bInvincible);
		ImGui::Text("State [%d] nCntState [%d]", m_State, m_nCntState);


		ImGui::TreePop();
	}
#endif //DEBUG
}
