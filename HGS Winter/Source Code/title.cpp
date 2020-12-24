//------------------------------------------------------------------------------
//
//タイトル処理  [title.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "fade.h"
#include "title.h"
#include "scene2D.h"
#include "bg.h"
#include "sound.h"
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define DEMOPLAY_FADE_COUNT		(1000)		//デモプレイに行くまでのカウンタ

#define TITLE_LOGO_SIZE			(D3DXVECTOR3(900.0f, 400.0f, 0.0f))

#define ENEMY_SYZE				(D3DXVECTOR3(60.0f, 60.0f, 0.0f))			// 敵のサイズ

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CTitle::CTitle()
{
	m_apScene2D.clear();
	m_apEnemy.clear();

	m_nCntState = DEMOPLAY_FADE_COUNT;
	m_titlestate = STATE_START;
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		m_EnemyMove[nCnt] = ZeroVector3;		// 敵の移動量
	}
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CTitle::~CTitle()
{
	m_apScene2D.clear();
	m_apEnemy.clear();
}

//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CTitle::Init(HWND hWnd)
{
	//カメラ座標設定
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_DEBUG);

	// 敵の生成
	EnemyCreate();

	// タイトルUIの生成
	TitleUICreate();

	CBg::Create(95);
	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CTitle::Update()
{
	//カウントダウン
	m_nCntState--;

	// 敵の更新
	EnemyUpdate();

	// タイトルUIの更新
	for (int nCnt = 0; nCnt < (int)TITLE_UI::TITLE_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt])
		{
			m_apScene2D[nCnt]->Update();
		}
	}


	//フェードしてない時
	if (CManager::GetRenderer()->GetFade()->GetFade() == CFade::FADE_NONE)
	{
		//何かボタン押したとき
		if (CHossoLibrary::CheckAnyButton())
		{
			//ステート変更
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_2DGAME);
			CManager::GetSound()->Play(CSound::LABEL_SE_DECISION);
		}
	}

}

//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CTitle::Draw()
{
	// 敵の描画
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (m_apEnemy[nCnt])
		{
			m_apEnemy[nCnt]->Draw();
		}
	}

	// タイトルUIの描画
	for (int nCnt = 0; nCnt < (int)TITLE_UI::TITLE_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt])
		{
			m_apScene2D[nCnt]->Draw();
		}
	}
}

//------------------------------------------------------------------------------
//タイトルUIの生成
//------------------------------------------------------------------------------
void CTitle::TitleUICreate()
{
	for (int nCnt = 0; nCnt < (int)TITLE_UI::TITLE_MAX; nCnt++)
	{
		if (nCnt == (int)TITLE_UI::TITLE_BLACK)
		{
			// シーン2Dの生成
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT, 0.0f), CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_NONE));
			// 色変更
			m_apScene2D[nCnt]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		}

		if (nCnt == (int)TITLE_UI::TITLE_NAME)
		{
			// シーン2Dの生成
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), 200.0f, 0.0f), TITLE_LOGO_SIZE, CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_TITLE_NAME));
		}

		if (nCnt == (int)TITLE_UI::TITLE_ENTER)
		{
			// シーン2Dの生成
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), 600.0f, 0.0f), D3DXVECTOR3(500.0f, 100.0f, 0.0f), CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_ENTER));
		}
	}
}

//------------------------------------------------------------------------------
// 敵の生成
//------------------------------------------------------------------------------
void CTitle::EnemyCreate()
{
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		switch (nCnt)
		{
		case 0:
			// シーン2Dの生成
			m_apEnemy.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.2f), SCREEN_HEIGHT * 0.2f, 0.0f), ENEMY_SYZE, CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apEnemy[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_ENEMY_BLUE));
			// 敵の移動量
			m_EnemyMove[nCnt] = D3DXVECTOR3(2.0f, 4.0f, 0.0f);
			break;

		case 1:
			// シーン2Dの生成
			m_apEnemy.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.6f), 400.0f, 0.0f), ENEMY_SYZE, CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apEnemy[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_ENEMY_PURPLE));
			// 敵の移動量
			m_EnemyMove[nCnt] = D3DXVECTOR3(4.0f, 3.0f, 0.0f);
			break;

		case 2:
			// シーン2Dの生成
			m_apEnemy.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.9f), 600.0f, 0.0f), ENEMY_SYZE, CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apEnemy[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_ENEMY_RED));
			// 敵の移動量
			m_EnemyMove[nCnt] = D3DXVECTOR3(6.0f, 7.0f, 0.0f);
			break;

		default:
			break;
		}
	}
}

//------------------------------------------------------------------------------
// 敵の更新
//------------------------------------------------------------------------------
void CTitle::EnemyUpdate()
{
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (m_apEnemy[nCnt])
		{
			// 移動
			m_apEnemy[nCnt]->GetPos() += m_EnemyMove[nCnt];

			// 反射
			if (CHossoLibrary::RangeLimit_Equal_Float(m_apEnemy[nCnt]->GetPos().x, 0, SCREEN_WIDTH))
			{
				m_EnemyMove[nCnt].x *= -1;
			}
			if (CHossoLibrary::RangeLimit_Equal_Float(m_apEnemy[nCnt]->GetPos().y, 0, SCREEN_HEIGHT))
			{
				m_EnemyMove[nCnt].y *= -1;
			}

			// 更新
			m_apEnemy[nCnt]->Update();
		}
	}
}