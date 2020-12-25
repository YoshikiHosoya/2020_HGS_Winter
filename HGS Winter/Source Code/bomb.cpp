//------------------------------------------------------------------------------
//
//�v���C���[2D����  [player_2D.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "bomb.h"
#include "game_2D.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "particle.h"
#include "Pad_XInput.h"
#include "enemy_2D.h"
//------------------------------------------------------------------------------
//�ÓI�����o�ϐ��̏�����
//------------------------------------------------------------------------------
D3DXVECTOR3 *CBomb::m_pPlayerPos = nullptr;

//------------------------------------------------------------------------------
//�}�N��
//------------------------------------------------------------------------------
#define BOMB_SYZE			(D3DXVECTOR3(80.0f, 80.0f, 0.0f))
#define EXPLOSION_LENGTH	(200.0f)

//------------------------------------------------------------------------------
//�R���X�g���N�^
//------------------------------------------------------------------------------
CBomb::CBomb()
{
	m_bMove = false;

	m_fRotation = CHossoLibrary::Random(0.05f);
}

//------------------------------------------------------------------------------
//�f�X�g���N�^
//------------------------------------------------------------------------------
CBomb::~CBomb()
{

}
//------------------------------------------------------------------------------
//����������
//------------------------------------------------------------------------------
HRESULT CBomb::Init()
{
	CCharacter_2D::Init();

	//�������m��
	std::unique_ptr<CScene2D> pScene2D = CScene2D::Create_SelfManagement(GetPos(), BOMB_SYZE);

	//�e�N�X�`���ݒ�
	pScene2D->BindTexture(CTexture::GetTexture(CTexture::TEX_ITEM_BOMB));

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
void CBomb::Update()
{
	//��]��
	GetRot().z += m_fRotation;

	//�X�V
	CCharacter_2D::Update();
}
//------------------------------------------------------------------------------
//�`�揈��
//------------------------------------------------------------------------------
void CBomb::Draw()
{
	CCharacter_2D::Draw();
}

//------------------------------------------------------------------------------
//�f�o�b�O���\�L
//------------------------------------------------------------------------------
void CBomb::ShowDebugInfo()
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
void CBomb::DamageAction()
{

}
//------------------------------------------------------------------------------
//���S���̏���
//------------------------------------------------------------------------------
void CBomb::DeathAction()
{
	////�Q�[���I��
	//CManager::GetGame()->SetGamestate(CGame::STATE_GAMECLEAR);
}
//------------------------------------------------------------------------------
//�X�e�[�g�ύX����
//------------------------------------------------------------------------------
void CBomb::SetState(STATE nextstate)
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
void CBomb::Collision()
{
	float fDistance = D3DXVec3Length(&(*m_pPlayerPos - GetPos()));

	if (fDistance < 50.0f)
	{
		Explosion();

	}
	CCharacter_2D::Collision();


}

//------------------------------------------------------------------------------
//����
//------------------------------------------------------------------------------
void CBomb::Explosion()
{

	//�����G�t�F�N�g
	CParticle::CreateFromText(GetPos(), ZeroVector3, CParticleParam::EFFECT_EXPLOSION);
	CParticle::CreateFromText(GetPos(), ZeroVector3, CParticleParam::EFFECT_IMPACT);

	//���Đ�
	CManager::GetSound()->Play(CSound::LABEL_SE_BOMB_EXOLOSION);

	//�G�̃��X�g�擾
	Vec<S_ptr<CScene>> pEnemyList;
	CScene::GetSceneList(CScene::OBJTYPE_ENEMY, pEnemyList);

	//����
	float Distance = 0.0f;
	int nKillEnemyNum = 0;

	for (auto ptr : pEnemyList)
	{
		//�L���X�g
		CEnemy_2D *pEnemy = ((CEnemy_2D*)ptr.get());

		//�����v��
		Distance = D3DXVec3Length(&(pEnemy->GetPos() - GetPos()));

		//������x�߂�������
		if (Distance < EXPLOSION_LENGTH)
		{
			//�_���[�W
			pEnemy->ApplyDamage(999);
			nKillEnemyNum++;
		}
	}

	if (nKillEnemyNum > 10)
	{
		CManager::GetSound()->Play(CSound::LABEL_SE_KILL_MANY);
	}
	else if (nKillEnemyNum > 0)
	{
		CManager::GetSound()->Play(CSound::LABEL_SE_KILL);

	}


	//�j��
	Release();
}


//------------------------------------------------------------------------------
//��������
//------------------------------------------------------------------------------
std::shared_ptr<CBomb> CBomb::Create(D3DXVECTOR3 pos)
{
	//�ϐ��錾

	std::shared_ptr<CBomb> pBomb = std::make_shared<CBomb>();


	if (pBomb)
	{
		//������
		pBomb->Init();

		//���W�ݒ�
		pBomb->SetPos(pos);

		//�I�u�W�F�N�g�^�C�v�ݒ�
		pBomb->SetObjType(OBJTYPE_BOMB);

		pBomb->GetScene2DPtr()->SetPos(pos);
		pBomb->GetScene2DPtr()->Update();

		CParticle::CreateFromText(pos, ZeroVector3, CParticleParam::EFFECT_BOMB);

		//���X�g�ɒǉ�
		pBomb->AddSharedList(pBomb);

		return pBomb;
	}

	//�����������
	return nullptr;
}

