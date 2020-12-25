//------------------------------------------------------------------------------
//
//2Dキャラクター処理  [character_2D.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "character_2D.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define INERTIA	(0.3f)			//摩擦
#define AIR_INERTIA (0.02f)		//空中時の摩擦
#define GRAVITY (0.7f)			//重力
//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CCharacter_2D::CCharacter_2D()
{
	m_pScene2D.reset();
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CCharacter_2D::~CCharacter_2D()
{
	m_pScene2D.reset();
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CCharacter_2D::Init()
{
	//成功
	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CCharacter_2D::Update()
{
	//移動処理
	Move();

	//回転処理
	Rot();

	//当たり判定
	Collision();

	//ステートに応じた処理
	State();

	//nullcheck
	if (m_pScene2D)
	{
		m_pScene2D->SetPos(GetPos());
		m_pScene2D->SetRot(GetRot());
		//更新
		m_pScene2D->Update();
	}
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CCharacter_2D::Draw()
{
	//nullcheck
	if (m_pScene2D)
	{
		//描画
		m_pScene2D->Draw();
	}
}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CCharacter_2D::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}


//------------------------------------------------------------------------------
//ステート設定
//------------------------------------------------------------------------------
void CCharacter_2D::SetState(STATE nextstate)
{
	CCharacter::SetState(nextstate);
}

//------------------------------------------------------------------------------
//キャラクター情報表示
//------------------------------------------------------------------------------
void CCharacter_2D::ShowCharacterInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}

//------------------------------------------------------------------------------
//移動処理
//------------------------------------------------------------------------------
void CCharacter_2D::Move()
{
	//移動量加算
	GetPos() += GetMove();

	//慣性
	GetMove().x += (0 - GetMove().x) * INERTIA;
	GetMove().y += (0 - GetMove().y) * INERTIA;
}
//------------------------------------------------------------------------------
//回転処理
//------------------------------------------------------------------------------
void CCharacter_2D::Rot()
{

}

//------------------------------------------------------------------------------
//当たり判定
//------------------------------------------------------------------------------
void CCharacter_2D::Collision()
{
	//フィールド外に出れないようにする
	CHossoLibrary::RangeLimit_Equal_Float(GetPos().x, 40.0f, 1230.0f);
	CHossoLibrary::RangeLimit_Equal_Float(GetPos().y, 35.0f, 690.0f);
}
//------------------------------------------------------------------------------
//状態に応じた処理
//------------------------------------------------------------------------------
void CCharacter_2D::State()
{

	GetCntState()--;

	switch (GetState())
	{
	case CCharacter::STATE_APPEAR:

		if (GetCntState() < 0)
		{
			SetState(STATE_NORMAL);
		}

		break;

	default:
		break;
	}
}

