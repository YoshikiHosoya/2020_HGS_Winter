//------------------------------------------------------------------------------
//
//プレイヤー2D処理  [player_2D.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
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
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define PLAYER_SYZE			(D3DXVECTOR3(60.0f, 60.0f, 0.0f))
#define MOVE_SPEED			(2.0f)

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CPlayer_2D::CPlayer_2D()
{
	m_bMove = false;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CPlayer_2D::~CPlayer_2D()
{

}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CPlayer_2D::Init()
{
	CCharacter_2D::Init();

	//メモリ確保
	std::unique_ptr<CScene2D> pScene2D = CScene2D::Create_SelfManagement(GetPos(), PLAYER_SYZE);

	//テクスチャ設定
	pScene2D->BindTexture(CTexture::GetTexture(CTexture::TEX_PLAYER));

	//Scene2Dセット
	SetScene2D(std::move(pScene2D));

	//重力無効
	CCharacter::SetGravity(false);

	//プレイヤーのポインタ設定
	CEnemy_2D::SetPlayerPosPtr(GetPosPtr());
	CBomb::SetPlayerPosPtr(GetPosPtr());
	CScoreUpItem::SetPlayerPosPtr(GetPosPtr());

	//成功
	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CPlayer_2D::Update()
{
	if (CManager::GetGame()->GetGamestate() != CGame::STATE_NORMAL)
	{
		return;
	}

	//軌跡
	CParticle::CreateFromText(GetPos(), ZeroVector3, CParticleParam::EFFECT_PLAYER_ORBIT, true);

	//移動入力
	MoveInput();

	//更新
	CCharacter_2D::Update();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CPlayer_2D::Draw()
{
	CCharacter_2D::Draw();
}

//------------------------------------------------------------------------------
//移動の入力
//------------------------------------------------------------------------------
void CPlayer_2D::MoveInput()
{
	m_bMove = false;

	//ゲームパッドの情報取得
	CPad_XInput *pGamePad = CManager::GetXInput();

	//変数宣言
	float joypadX, joypadY;

	//ジョイパッドの移動処理
	pGamePad->GetStickLeft(&joypadX, &joypadY);

	if (joypadX != 0 || joypadY != 0)
	{
		GetMove().x += joypadX * MOVE_SPEED / 32768.0f;
		GetMove().y -= joypadY * MOVE_SPEED / 32768.0f;
	}
}

//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CPlayer_2D::ShowDebugInfo()
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
//当たり判定処理
//------------------------------------------------------------------------------
void CPlayer_2D::Collision()
{
	CCharacter_2D::Collision();

}

//------------------------------------------------------------------------------
//ダメージ受けた後の処理
//------------------------------------------------------------------------------
void CPlayer_2D::DamageAction()
{

}
//------------------------------------------------------------------------------
//死亡時の処理
//------------------------------------------------------------------------------
void CPlayer_2D::DeathAction()
{
	////ゲーム終了
	//CManager::GetGame()->SetGamestate(CGame::STATE_GAMECLEAR);
}
//------------------------------------------------------------------------------
//ステート変更処理
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
//生成処理
//------------------------------------------------------------------------------
std::shared_ptr<CPlayer_2D> CPlayer_2D::Create(D3DXVECTOR3 pos)
{
	//変数宣言
	std::shared_ptr<CPlayer_2D> pPlayer = std::make_shared<CPlayer_2D>();


	if (pPlayer)
	{
		//初期化
		pPlayer->Init();

		//座標設定
		pPlayer->SetPos(pos);

		//オブジェクトタイプ設定
		pPlayer->SetObjType(OBJTYPE_PLAYER);

		//リストに追加
		pPlayer->AddSharedList(pPlayer);

		return pPlayer;
	}

	//生成した情報
	return nullptr;
}

