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

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CTitle::CTitle()
{
	m_apScene2D.clear();

	m_nCntState = DEMOPLAY_FADE_COUNT;
	m_titlestate = STATE_START;

}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CTitle::~CTitle()
{
	m_apScene2D.clear();
}

//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CTitle::Init(HWND hWnd)
{
	//カメラ座標設定
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_DEBUG);

	// タイトルUIの生成
	TitleUICreate();

	CBg::Create(95);
	return S_OK;
}

//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CTitle::Uninit()
{
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CTitle::Update()
{
	//カウントダウン
	m_nCntState--;

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