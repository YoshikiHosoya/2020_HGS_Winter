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
#include "score_up_item.h"

//------------------------------------------------------------------------------
//�ÓI�����o�ϐ��̏�����
//------------------------------------------------------------------------------
D3DXVECTOR3 *CEnemy_2D::m_pPlayerPos = nullptr;
int CEnemy_2D::m_nNumEnemy = 0;
//------------------------------------------------------------------------------
//�}�N��
//------------------------------------------------------------------------------
#define ENEMY_SYZE			(D3DXVECTOR3(60.0f, 60.0f, 0.0f))
#define MOVESPEED_BLUE		(1.2f)
#define MOVESPEED_PURPLE	(1.6f)
#define MOVESPEED_RED		(2.5f)
#define RED_TURN_TIME		(40)

//------------------------------------------------------------------------------
//�R���X�g���N�^
//------------------------------------------------------------------------------
CEnemy_2D::CEnemy_2D()
{
	m_bMove = false;
	m_nNumEnemy++;
	m_direction = DIRECTION::UP;
	m_nCntDirection = 0;
}

//------------------------------------------------------------------------------
//�f�X�g���N�^
//------------------------------------------------------------------------------
CEnemy_2D::~CEnemy_2D()
{
	m_nNumEnemy--;
}
//------------------------------------------------------------------------------
//����������
//------------------------------------------------------------------------------
HRESULT CEnemy_2D::Init()
{
	CCharacter_2D::Init();

	//�������m��
	std::unique_ptr<CScene2D> pScene2D = CScene2D::Create_SelfManagement(GetPos(), ENEMY_SYZE);

	//Scene2D�Z�b�g
	SetScene2D(std::move(pScene2D));

	//�d�͖���
	CCharacter::SetGravity(false);

	//�X�e�[�g�ݒ�
	SetState(CCharacter::STATE_APPEAR);

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
	//CCharacter_2D::Collision();

	//�t�B�[���h�O�ɏo��Ȃ��悤�ɂ���
	if (CHossoLibrary::RangeLimit_Equal_Float(GetPos().x, 0, SCREEN_WIDTH) ||
		CHossoLibrary::RangeLimit_Equal_Float(GetPos().y, 0, SCREEN_HEIGHT))
	{
		if (m_type == ENEMY_TYPE::RED)
		{
			RedEnemy_Turn();
		}
	}

	//�X�e�[�g���ʏ킶��Ȃ�����return
	if (GetState() != STATE::STATE_NORMAL)
	{
		return;
	}

	float fLocalDistance = D3DXVec3Length(&m_DifPos);

	//CDebugProc::Print(CDebugProc::PLACE_LEFT, "fLocalDistance >> %.2f\n", fLocalDistance);

	if (fLocalDistance < 50.0f)
	{

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
	//�Q�[���I��
	CParticle::CreateFromText(GetPos(), ZeroVector3, CParticleParam::EFFECT_DEFAULT);

	//�~�g�R���h���A����
	CScoreUpItem::Create(GetPos() + CHossoLibrary::RandomVector3(20.0f));
}
//------------------------------------------------------------------------------
//�X�e�[�g�ύX����
//------------------------------------------------------------------------------
void CEnemy_2D::SetState(STATE nextstate)
{
	CCharacter_2D::SetState(nextstate);

	switch (nextstate)
	{
	case CCharacter::STATE_APPEAR:
		GetScene2DPtr()->BindTexture(CTexture::GetTexture(CTexture::TEX_NONE));
		SetCntState(30);
		break;

	case CCharacter::STATE_NORMAL:
		//�^�C�v�ɉ������e�N�X�`���ݒ�
		SetType(m_type);

		GetScene2DPtr()->SetSize(ENEMY_SYZE);

		break;
	}
}

//------------------------------------------------------------------------------
//�X�e�[�g����
//------------------------------------------------------------------------------
void CEnemy_2D::State()
{
	GetCntState()--;

	switch (GetState())
	{
	case CCharacter::STATE_APPEAR:

		GetScene2DPtr()->SetSize((ENEMY_SYZE / 5) * (5 - (GetCntState() % 6)));

		if (GetCntState() < 0)
		{
			SetState(STATE_NORMAL);
		}

		break;

	case CCharacter::STATE_NORMAL:

		//�ړ��̂`�h
		MoveAI();

		break;

	default:
		break;
	}
}

//------------------------------------------------------------------------------
//�^�[��
//------------------------------------------------------------------------------
void CEnemy_2D::RedEnemy_Turn()
{
	switch (m_direction)
	{
	case DIRECTION::LEFT:
		m_direction = DIRECTION::RIGHT;
		break;
	case DIRECTION::RIGHT:
		m_direction = DIRECTION::LEFT;
		break;
	case DIRECTION::UP:
		m_direction = DIRECTION::DOWN;
		break;
	case DIRECTION::DOWN:
		m_direction = DIRECTION::UP;
		break;

	default:
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

		m_direction = (DIRECTION)(rand() % (int)DIRECTION::MAX);

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

		//�����蔻�菈��
		pEnemy->Collision();

		//�^�C�v�ݒ�
		pEnemy->SetType(type);

		pEnemy->GetScene2DPtr()->SetPos(pos);
		pEnemy->GetScene2DPtr()->Update();
		//pEnemy->GetScene2DPtr()->BindTexture(CTexture::GetTexture(CTexture::TEX_NONE));

		//�I�u�W�F�N�g�^�C�v�ݒ�
		pEnemy->SetObjType(CScene::OBJTYPE_ENEMY);

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

		m_nCntDirection++;

		switch (m_direction)
		{
		case DIRECTION::LEFT:
			GetMove().x -= MOVESPEED_RED;
			SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI));
			break;

		case DIRECTION::RIGHT:
			GetMove().x += MOVESPEED_RED;

			break;
		case DIRECTION::UP:
			GetMove().y -= MOVESPEED_RED;

			SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));

			break;
		case DIRECTION::DOWN:
			GetMove().y += MOVESPEED_RED;

			SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * -0.5f));

			break;

		default:
			break;
		}

		break;
	case CEnemy_2D::PURPLE:
		vec = *D3DXVec3Normalize(&vec, &m_DifPos);

		GetMove() += vec * MOVESPEED_PURPLE;
		GetRot().z += 0.1f;

		D3DXVECTOR3;

		break;

	default:
		break;
	}


}

