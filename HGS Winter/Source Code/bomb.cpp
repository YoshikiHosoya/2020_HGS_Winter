//------------------------------------------------------------------------------
//
//プレイヤー2D処理  [player_2D.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
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
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
D3DXVECTOR3 *CBomb::m_pPlayerPos = nullptr;

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define PLAYER_SYZE			(D3DXVECTOR3(80.0f, 80.0f, 0.0f))
#define EXPLOSION_LENGTH	(200.0f)

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CBomb::CBomb()
{
	m_bMove = false;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CBomb::~CBomb()
{

}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CBomb::Init()
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

	//成功
	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CBomb::Update()
{
	//更新
	CCharacter_2D::Update();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CBomb::Draw()
{
	CCharacter_2D::Draw();
}

//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CBomb::ShowDebugInfo()
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
//ダメージ受けた後の処理
//------------------------------------------------------------------------------
void CBomb::DamageAction()
{

}
//------------------------------------------------------------------------------
//死亡時の処理
//------------------------------------------------------------------------------
void CBomb::DeathAction()
{
	////ゲーム終了
	//CManager::GetGame()->SetGamestate(CGame::STATE_GAMECLEAR);
}
//------------------------------------------------------------------------------
//ステート変更処理
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
//当たり判定処理
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
//爆発
//------------------------------------------------------------------------------
void CBomb::Explosion()
{

	//爆発エフェクト
	CParticle::CreateFromText(GetPos(), ZeroVector3, CParticleParam::EFFECT_IMPACT);

	//敵のリスト取得
	Vec<S_ptr<CScene>> pEnemyList;
	CScene::GetSceneList(CScene::OBJTYPE_ENEMY, pEnemyList);

	//距離
	float Distance = 0.0f;

	for (auto ptr : pEnemyList)
	{
		//キャスト
		CEnemy_2D *pEnemy = ((CEnemy_2D*)ptr.get());

		//距離計測
		Distance = D3DXVec3Length(&(pEnemy->GetPos() - GetPos()));

		//ある程度近かった時
		if (Distance < EXPLOSION_LENGTH)
		{
			//ダメージ
			pEnemy->ApplyDamage(999);
		}
	}

	//破棄
	Release();
}


//------------------------------------------------------------------------------
//生成処理
//------------------------------------------------------------------------------
std::shared_ptr<CBomb> CBomb::Create(D3DXVECTOR3 pos)
{
	//変数宣言
	std::shared_ptr<CBomb> pPlayer = std::make_shared<CBomb>();


	if (pPlayer)
	{
		//初期化
		pPlayer->Init();

		//座標設定
		pPlayer->SetPos(pos);

		//オブジェクトタイプ設定
		pPlayer->SetObjType(OBJTYPE_BOMB);

		//リストに追加
		pPlayer->AddSharedList(pPlayer);

		return pPlayer;
	}

	//生成した情報
	return nullptr;
}

