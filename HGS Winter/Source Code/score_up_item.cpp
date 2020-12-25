//------------------------------------------------------------------------------
//
//プレイヤー2D処理  [player_2D.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
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
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
D3DXVECTOR3 *CScoreUpItem::m_pPlayerPos = nullptr;

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define ITEM_SIZE			(D3DXVECTOR3(15.0f, 15.0f, 0.0f))
#define TRACKING_LENGTH		(100.0f)
#define GETITEM_LENGTH		(30.0f)
#define TRACKING_SPEED		(7.0f)
#define DEFAULT_LIFE		(300)

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CScoreUpItem::CScoreUpItem()
{
	m_bMove = false;
	m_fRotation = CHossoLibrary::Random(0.05f);
	SetLife(DEFAULT_LIFE);
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CScoreUpItem::~CScoreUpItem()
{

}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CScoreUpItem::Init()
{
	CCharacter_2D::Init();

	//メモリ確保
	std::unique_ptr<CScene2D> pScene2D = CScene2D::Create_SelfManagement(GetPos(), ITEM_SIZE);

	//テクスチャ設定
	pScene2D->BindTexture(CTexture::GetTexture(CTexture::TEX_ITEM_MITOKONDORIA));

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
void CScoreUpItem::Update()
{
	GetRot().z += m_fRotation;

	//ライフ減少
	SetLife(GetLife() - 1);

	//ライフが一定以下になったら
	if ((GetLife() <= 50))
	{
		//徐々に透明
		GetScene2DPtr()->SetColor(GetScene2DPtr()->GetColor() -= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.02f));

		if (GetLife() <= 0)
		{
			Release();
		}
	}


	//更新
	CCharacter_2D::Update();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CScoreUpItem::Draw()
{
	CCharacter_2D::Draw();
}

//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CScoreUpItem::ShowDebugInfo()
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
void CScoreUpItem::DamageAction()
{

}
//------------------------------------------------------------------------------
//死亡時の処理
//------------------------------------------------------------------------------
void CScoreUpItem::DeathAction()
{
	////ゲーム終了
	//CManager::GetGame()->SetGamestate(CGame::STATE_GAMECLEAR);
}
//------------------------------------------------------------------------------
//ステート変更処理
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
//当たり判定処理
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
//生成処理
//------------------------------------------------------------------------------
std::shared_ptr<CScoreUpItem> CScoreUpItem::Create(D3DXVECTOR3 pos)
{
	//変数宣言
	std::shared_ptr<CScoreUpItem> pMitocondoria = std::make_shared<CScoreUpItem>();


	if (pMitocondoria)
	{
		//初期化
		pMitocondoria->Init();

		//座標設定
		pMitocondoria->SetPos(pos);

		//オブジェクトタイプ設定
		pMitocondoria->SetObjType(OBJTYPE_MITCONDORIA);

		pMitocondoria->GetScene2DPtr()->SetPos(pos);
		pMitocondoria->GetScene2DPtr()->Update();


		//リストに追加
		pMitocondoria->AddSharedList(pMitocondoria);

		return pMitocondoria;
	}

	//生成した情報
	return nullptr;
}

