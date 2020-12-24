//------------------------------------------------------------------------------
//
//リザルト処理  [result.cpp]
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
#include "result.h"
#include "sound.h"
#include "scene2D.h"
#include "multinumber.h"
#include "game_2D.h"

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define RESULT_SCORE_UI_SIZE		(D3DXVECTOR3(200.0f, 100.0f, 0.0f))					// リザルトスコアのUIサイズ
#define SURVIVED_TIME_UI_SIZE		(D3DXVECTOR3(200.0f, 100.0f, 0.0f))					// 生存時間のUIサイズ
#define KILL_UI_SIZE				(D3DXVECTOR3(200.0f, 100.0f, 0.0f))					// キルのUIサイズ
#define ANY_BUTTON_UI_SIZE			(D3DXVECTOR3(500.0f, 100.0f, 0.0f))					// エニーボタンのUIサイズ
#define SCORE_SIZE					(D3DXVECTOR3(50.0f, 70.0f, 0.0f))					// スコアのUIサイズ

#define RESULT_SCORE_DIGITS			(7)													// リザルトスコアの桁数
#define SURVIVED_TIME_DIGITS		(2)													// 生存時間の桁数
#define NUM_KILL_DIGITS				(3)													// キル数の桁数

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CResult::CResult()
{
	// 初期化
	m_nCntResult = 0;
	m_apScene2D.clear();					// リザルトUI
	m_pResultScore = nullptr;					// リザルトスコア
	m_pSurvivedTime = nullptr;				// 生存時間
	m_pNumKill = nullptr;						// キル数
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CResult::~CResult()
{
	// 初期化
	m_apScene2D.clear();					// リザルトUI
	m_pResultScore = nullptr;					// リザルトスコア
	m_pSurvivedTime = nullptr;				// 生存時間
	m_pNumKill = nullptr;						// キル数
}

//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CResult::Init(HWND hWnd)
{
	//カメラ座標設定
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_FIXED);

	// リザルトUIの生成
	ResultUICreate();
	// リザルトスコアの生成
	ResultScoreCreate();
	// 生存時間の生成
	SurvivedTimeCreate();
	// キル数の生成
	NumKillCreate();

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CResult::Update()
{
	//カウンタ++
	m_nCntResult++;

	// ランキングUIの更新
	for (int nCnt = 0; nCnt < (int)RESULT_UI::RESULT_MAX; nCnt++)
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
				CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RANKING);
				CManager::GetSound()->Play(CSound::LABEL_SE_DECISION);
		}
	}
}

//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CResult::Draw()
{
	// ランキングUIの描画
	for (int nCnt = 0; nCnt < (int)RESULT_UI::RESULT_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt])
		{
			m_apScene2D[nCnt]->Draw();
		}
	}
}

//------------------------------------------------------------------------------
// リザルトUIの生成
//------------------------------------------------------------------------------
void CResult::ResultUICreate()
{
	for (int nCnt = 0; nCnt < (int)RESULT_UI::RESULT_MAX; nCnt++)
	{
		// リザルトスコア
		if (nCnt == (int)RESULT_UI::RESULT_SCORE)
		{
			// シーン2Dの生成
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.25f), 120.0f, 0.0f), RESULT_SCORE_UI_SIZE, CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_SCORE));
		}
		// 生存時間
		else if (nCnt == (int)RESULT_UI::SURVIVED_TIME)
		{
			// シーン2Dの生成
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.75f), 120.0f, 0.0f), SURVIVED_TIME_UI_SIZE, CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_TIME));
		}
		// キル
		else if (nCnt == (int)RESULT_UI::KILL)
		{
			// シーン2Dの生成
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), 370.0f, 0.0f), KILL_UI_SIZE, CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_RESULT_KILL));
		}
		// エニーボタン
		else if (nCnt == (int)RESULT_UI::ANY_BUTTON)
		{
			// シーン2Dの生成
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), 650.0f, 0.0f), ANY_BUTTON_UI_SIZE, CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_ENTER));
		}
	}
}

//------------------------------------------------------------------------------
// リザルトスコアの生成
//------------------------------------------------------------------------------
void CResult::ResultScoreCreate()
{
	// スコアの生成
	m_pResultScore = CMultiNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.25f), 220.0f, 0.0f),
		SCORE_SIZE,
		CGame_2D::GetScore(),
		RESULT_SCORE_DIGITS,
		CScene::OBJTYPE_UI);
}

//------------------------------------------------------------------------------
// 生存時間の生成
//------------------------------------------------------------------------------
void CResult::SurvivedTimeCreate()
{
	// スコアの生成
	m_pSurvivedTime = CMultiNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.75f), 220.0f, 0.0f),
		SCORE_SIZE,
		CGame_2D::GetTime(),
		SURVIVED_TIME_DIGITS,
		CScene::OBJTYPE_UI);
}

//------------------------------------------------------------------------------
// キル数の生成
//------------------------------------------------------------------------------
void CResult::NumKillCreate()
{
	// スコアの生成
	m_pNumKill = CMultiNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), 470.0f, 0.0f),
		SCORE_SIZE,
		/*CGame_2D::GetScore()*/ 0,
		NUM_KILL_DIGITS,
		CScene::OBJTYPE_UI);
}