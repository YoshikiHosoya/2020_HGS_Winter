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
#include "Way.h"
#include "renderer.h"
#include "sound.h"
#include "particle.h"
#include "Pad_XInput.h"

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

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
	std::unique_ptr<CScene2D> pScene2D = CScene2D::Create_SelfManagement(GetPos(), D3DXVECTOR3(40.0f, 60.0f, 0.0f));

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
//終了処理
//------------------------------------------------------------------------------
void CPlayer_2D::Uninit()
{
	CCharacter_2D::Uninit();
}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CPlayer_2D::Update()
{
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

	////変数宣言
	//float joypadX, joypadY;

	////ジョイパッドの移動処理
	//pGamePad->GetStickLeft(&joypadX, &joypadY);

	//if (joypadX != 0 || joypadY != 0)
	//{
	//	GetMove().x += joypadX * CCharacter::GetDefaultParam(GetParam())->GetMoveSpeed() / 32768.0f;
	//	GetMove().y -= joypadY * CCharacter::GetDefaultParam(GetParam())->GetMoveSpeed() / 32768.0f;
	//}

	CGame_2D *pGame2D = (CGame_2D*)CManager::GetGame();

	if (pGame2D->GetGamestate() != CGame::STATE::STATE_NORMAL)
	{
		return;
	}

	//上に進行してるとき
	if (pGame2D->GetDirection() == DIRECTION::UP)
	{
		GetScene2D()->SetRot(ZeroVector3);


		CParticle::CreateFromText(GetPos() + D3DXVECTOR3(15.0f, 20.0f, 0.0f), ZeroVector3, CParticleParam::EFFECT_PLAYERENGINE);
		CParticle::CreateFromText(GetPos() + D3DXVECTOR3(-15.0f, 20.0f, 0.0f), ZeroVector3, CParticleParam::EFFECT_PLAYERENGINE);


		//[D]キーを押した時
		if (CHossoLibrary::CheckMove(CHossoLibrary::RIGHT))
		{
			pGame2D->PlayerBending(DIRECTION::RIGHT);
			m_bMove = true;
		}
		//[A]キーを押した時
		if (CHossoLibrary::CheckMove(CHossoLibrary::LEFT))
		{
			pGame2D->PlayerBending(DIRECTION::LEFT);
			m_bMove = true;

		}
	}

	else if (pGame2D->GetDirection() == DIRECTION::LEFT)
	{
		GetScene2D()->SetRot(D3DXVECTOR3(0.0f, 0.0f, -1.57f));
	}

	else if (pGame2D->GetDirection() == DIRECTION::RIGHT)
	{
		GetScene2D()->SetRot(D3DXVECTOR3(0.0f, 0.0f, 1.57f));
	}
	std::vector<std::shared_ptr<CScene>> pWayList;

	CScene::GetSceneList(OBJTYPE::OBJTYPE_WAY, pWayList);


	//サイズ分
	for (size_t nCnt = 0; nCnt < pWayList.size(); nCnt++)
	{
		if (!pWayList[nCnt])
		{
			continue;
		}

		CWay *pWay = (CWay*)pWayList[nCnt].get();

		if (pWay)
		{
			//Ｗａｙポリゴンの上かどうか
			if (pWay->Collision(GetPos()))
			{
				CDebugProc::Print(CDebugProc::PLACE_LEFT, "WayPos.y >> %.2f\n", pWay->GetPos().y);

				//衝突してるか
				if (pWay->CollisionPlayerHit(GetPos()))
				{
					//エフェクト発生
					CParticle::CreateFromText(GetPos(), ZeroVector3, CParticleParam::EFFECT_PLAYERDEATH01);
					CParticle::CreateFromText(GetPos(), ZeroVector3, CParticleParam::EFFECT_PLAYERDEATH02);

					GetScene2D()->SetDisp(false);
					CManager::GetGame()->SetGamestate(CGame::STATE_GAMEOVER);

					//音再生
					CManager::GetSound()->Play(CSound::LABEL_SE_TIMEUP);


					break;
				}
				else
				{
					//プレイヤーが動いたフレーム
					if (m_bMove)
					{
						//時間加算するかどうかの判定
						if (pWay->CollisionPlayerAddTimer(GetPos()))
						{
							break;
						}
					}
				}
			}
		}
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

