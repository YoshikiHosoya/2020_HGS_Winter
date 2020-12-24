//------------------------------------------------------------------------------
//
//エネミー2D処理  [enemy_2D.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "enemy_2D.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "particle.h"
#include "score_up_item.h"

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
D3DXVECTOR3 *CEnemy_2D::m_pPlayerPos = nullptr;
int CEnemy_2D::m_nNumEnemy = 0;
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define ENEMY_SYZE			(D3DXVECTOR3(60.0f, 60.0f, 0.0f))
#define MOVESPEED_BLUE		(1.2f)
#define MOVESPEED_PURPLE	(1.6f)
#define MOVESPEED_RED		(2.5f)
#define RED_TURN_TIME		(40)

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CEnemy_2D::CEnemy_2D()
{
	m_bMove = false;
	m_nNumEnemy++;
	m_direction = DIRECTION::UP;
	m_nCntDirection = 0;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CEnemy_2D::~CEnemy_2D()
{
	m_nNumEnemy--;
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CEnemy_2D::Init()
{
	CCharacter_2D::Init();

	//メモリ確保
	std::unique_ptr<CScene2D> pScene2D = CScene2D::Create_SelfManagement(GetPos(), ENEMY_SYZE);

	//Scene2Dセット
	SetScene2D(std::move(pScene2D));

	//重力無効
	CCharacter::SetGravity(false);

	//ステート設定
	SetState(CCharacter::STATE_APPEAR);

	//成功
	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CEnemy_2D::Update()
{
	//座標の差分算出
	m_DifPos = *m_pPlayerPos - GetPos();

	//更新
	CCharacter_2D::Update();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CEnemy_2D::Draw()
{
	CCharacter_2D::Draw();
}


//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CEnemy_2D::ShowDebugInfo()
{
#ifdef _DEBUG
	//プレイヤー情報情報
	if (ImGui::TreeNode("PlayerInfo"))
	{
		//キャラクター情報表示
		ShowCharacterInfo();

		ImGui::TreePop();
	}
#endif //DEBUG
}

//------------------------------------------------------------------------------
//当たり判定
//------------------------------------------------------------------------------
void CEnemy_2D::Collision()
{
	//CCharacter_2D::Collision();

	//フィールド外に出れないようにする
	if (CHossoLibrary::RangeLimit_Equal_Float(GetPos().x, 0, SCREEN_WIDTH) ||
		CHossoLibrary::RangeLimit_Equal_Float(GetPos().y, 0, SCREEN_HEIGHT))
	{
		if (m_type == ENEMY_TYPE::RED)
		{
			RedEnemy_Turn();
		}
	}

	//ステートが通常じゃない時はreturn
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
//ダメージ受けた後の処理
//------------------------------------------------------------------------------
void CEnemy_2D::DamageAction()
{

}
//------------------------------------------------------------------------------
//死亡時の処理
//------------------------------------------------------------------------------
void CEnemy_2D::DeathAction()
{
	//ゲーム終了
	CParticle::CreateFromText(GetPos(), ZeroVector3, CParticleParam::EFFECT_DEFAULT);

	//ミトコンドリア生成
	CScoreUpItem::Create(GetPos() + CHossoLibrary::RandomVector3(20.0f));
}
//------------------------------------------------------------------------------
//ステート変更処理
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
		//タイプに応じたテクスチャ設定
		SetType(m_type);

		GetScene2DPtr()->SetSize(ENEMY_SYZE);

		break;
	}
}

//------------------------------------------------------------------------------
//ステート処理
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

		//移動のＡＩ
		MoveAI();

		break;

	default:
		break;
	}
}

//------------------------------------------------------------------------------
//ターン
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
//タイプ設定
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
//生成処理
//------------------------------------------------------------------------------
std::shared_ptr<CEnemy_2D> CEnemy_2D::Create(D3DXVECTOR3 pos,CEnemy_2D::ENEMY_TYPE type)
{
	//変数宣言
	std::shared_ptr<CEnemy_2D> pEnemy = std::make_shared<CEnemy_2D>();

	if (pEnemy)
	{
		//初期化
		pEnemy->Init();

		//座標設定
		pEnemy->SetPos(pos);

		//当たり判定処理
		pEnemy->Collision();

		//タイプ設定
		pEnemy->SetType(type);

		pEnemy->GetScene2DPtr()->SetPos(pos);
		pEnemy->GetScene2DPtr()->Update();
		//pEnemy->GetScene2DPtr()->BindTexture(CTexture::GetTexture(CTexture::TEX_NONE));

		//オブジェクトタイプ設定
		pEnemy->SetObjType(CScene::OBJTYPE_ENEMY);

		//リストに追加
		pEnemy->AddSharedList(pEnemy);

		return pEnemy;
	}

	//生成した情報
	return nullptr;
}

//------------------------------------------------------------------------------
//移動処理
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

