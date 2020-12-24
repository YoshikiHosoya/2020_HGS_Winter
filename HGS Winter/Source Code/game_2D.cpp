//------------------------------------------------------------------------------
//
//ゲーム処理  [game.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "game_2D.h"
#include "manager.h"
#include "renderer.h"
#include "player_2D.h"
#include "fade.h"
#include "keyboard.h"
#include "camera.h"
#include "ParticleManager.h"
#include "multinumber.h"
#include "bg.h"
#include "sound.h"
#include "particle.h"
#include "enemy_2D.h"
#include "bomb.h"
#include "scene2D.h"

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
int CGame_2D::m_nCntTime = 0;		// カウントタイム
int CGame_2D::m_nScore = 0;			// スコア

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define DEFAULT_CREATE_POS (2000.0f)
#define WAY_SIZE (400.0f)
#define BENDING_TIME (4)
#define DEFAULT_TIME (20)
#define COUNTDOWN	(3)

#define SCORE_UI_SIZE				(D3DXVECTOR3(200.0f, 80.0f, 0.0f))					// スコアのUIサイズ
#define TIME_UI_SIZE				(D3DXVECTOR3(200.0f, 80.0f, 0.0f))					// タイムのUIサイズ
#define CROSS_UI_SIZE				(D3DXVECTOR3(80.0f, 80.0f, 0.0f))					// バツのUIサイズ
#define HIGHSCORE_UI_SIZE			(D3DXVECTOR3(200.0f, 80.0f, 0.0f))					// ハイスコアのUIサイズ
#define SCORE_SIZE					(D3DXVECTOR3(20.0f, 30.0f, 0.0f))					// スコアのUIサイズ
#define SCORE_DIGITS				(7)													// スコアの桁数
#define TIME_DIGITS					(3)													// タイムの桁数
#define MAGNIFICATION_DIGITS		(2)													// 倍率の桁数

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CGame_2D::CGame_2D()
{
	m_nSpeed = 15;
	m_nScoreMag = 1;
	m_direction = DIRECTION::UP;
	m_nCnt = 0;
	m_nScoreDistance = 0;
	SetScore(0);
	m_bBendingFlag = false;
	m_nTime = 0;
	m_nCntTime = 0;
	m_nScore = 0;
	m_nBendingCountDown = COUNTDOWN;
	m_apScene2D.clear();						// ゲームUI
	m_pScoreNumber		= nullptr;				// スコア
	m_pTimeNumber		= nullptr;				// タイム
	m_pMagnification	= nullptr;				// 倍率
	m_pHighScoreNumber	= nullptr;				// ハイスコア
}
//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CGame_2D::~CGame_2D()
{
	m_apScene2D.clear();						// ゲームUI
	m_pScoreNumber		= nullptr;				// スコア
	m_pTimeNumber		= nullptr;				// タイム
	m_pMagnification	= nullptr;				// 倍率
	m_pHighScoreNumber	= nullptr;				// ハイスコア
}

//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CGame_2D::Init(HWND hWnd)
{
	//カメラ座標設定
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_DEBUG);

	CGame::SetPlayerPtr(CPlayer_2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 650.0f, 0.0f)));

	//パーティクルのマネージャ
	CParticleManager::Create();

	// 背景の生成
	CBg::Create(65);

	// ゲームUIの生成
	GameUICreate();
	
	// スコアの生成
	m_pScoreNumber = CMultiNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.2f), 80.0f, 0.0f),
		SCORE_SIZE,
		0,
		SCORE_DIGITS,
		CScene::OBJTYPE_UI);

	// タイムの生成
	m_pTimeNumber = CMultiNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.4f), 80.0f, 0.0f),
		SCORE_SIZE,
		0,
		TIME_DIGITS,
		CScene::OBJTYPE_UI);

	// 倍率の生成
	m_pMagnification = CMultiNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.63f), 55.0f, 0.0f),
		SCORE_SIZE,
		0,
		MAGNIFICATION_DIGITS,
		CScene::OBJTYPE_UI);

	// ハイスコアの生成
	HighScoreCreate();

	//ゲームステート初期化
	SetGamestate(CGame::STATE_READY);

	//音再生
	CManager::GetSound()->Play(CSound::LABEL_SE_READY);

	CEnemy_2D::Create(D3DXVECTOR3(100.0f, 100.0f, 0.0f), CEnemy_2D::BLUE);

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CGame_2D::Update()
{
	// 敵の出現
	EnemySpawn();

	// カウントアップタイマー
	AddTimer();

	// スコアの値の設定 ( 仮 )
	m_pScoreNumber->SetMultiNumber(m_nScore);

	// ハイスコアの更新
	HighScoreUpdate();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CGame_2D::Draw()
{

}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CGame_2D::ShowDebugInfo()
{
#ifdef _DEBUG
	//キーボードのポインタ
	CKeyboard *pKeyboard = CManager::GetKeyboard();


	CDebugProc::Print(CDebugProc::PLACE_LEFT, "m_nScoreDistance >> %d\n", m_nScoreDistance);
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "m_fNextBendingPoint >> %.2f\n", m_fNextBendingPoint);
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "NextDirection >> %d\n", m_NextBendingDirection);


#endif //_DEBUG
}

//------------------------------------------------------------------------------
//敵の出現
//------------------------------------------------------------------------------
void CGame_2D::EnemySpawn()
{
	m_nCnt++;

	CDebugProc::Print(CDebugProc::PLACE_LEFT, "EnemuNum >> %d\n", CEnemy_2D::GetEnemyNum());


	if (m_nCnt % 60 == 0)
	{
		CreateEnemyGroup(SCREEN_CENTER_POS + D3DXVECTOR3(CHossoLibrary::Random(500.0f), CHossoLibrary::Random(250.0f), 0.0f));

		if (m_nCnt % 120 == 0)
		{
			CBomb::Create(SCREEN_CENTER_POS + D3DXVECTOR3(CHossoLibrary::Random(500.0f), CHossoLibrary::Random(250.0f), 0.0f));
		}
	}

}

//------------------------------------------------------------------------------
//敵の集団生成
//------------------------------------------------------------------------------
void CGame_2D::CreateEnemyGroup(D3DXVECTOR3 posOrigin)
{
	int nLocalValue = m_nCnt / 120;

	CHossoLibrary::RangeLimit_Equal_Int(nLocalValue, 3, 10);

	CEnemy_2D::Create(posOrigin + D3DXVECTOR3(CHossoLibrary::Random(70.0f), CHossoLibrary::Random(70.0f), 0.0f), CEnemy_2D::RED);


	for (int nCnt = 0; nCnt < nLocalValue ; nCnt++)
	{
		//CEnemy_2D::Create(posOrigin + D3DXVECTOR3(CHossoLibrary::Random(70.0f), CHossoLibrary::Random(70.0f), 0.0f), CEnemy_2D::PURPLE);

		//CEnemy_2D::Create(posOrigin + D3DXVECTOR3(CHossoLibrary::Random(70.0f), CHossoLibrary::Random(70.0f), 0.0f), CEnemy_2D::BLUE);

		if (nLocalValue > 2000)
		{
			CEnemy_2D::Create(posOrigin + D3DXVECTOR3(CHossoLibrary::Random(70.0f), CHossoLibrary::Random(70.0f), 0.0f), CEnemy_2D::PURPLE);
		}
	}
}

//------------------------------------------------------------------------------
//ゲームUIの生成
//------------------------------------------------------------------------------
void CGame_2D::GameUICreate()
{
	for (int nCnt = 0; nCnt < (int)GAME_UI::GAME_UI_MAX; nCnt++)
	{
		// スコア
		if (nCnt == (int)GAME_UI::SCORE)
		{
			// シーン2Dの生成
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.2f), 50.0f, 0.0f), SCORE_UI_SIZE, CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_RANKING_NAME));
		}
		// タイム
		else if (nCnt == (int)GAME_UI::TIME)
		{
			// シーン2Dの生成
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.4f), 50.0f, 0.0f), TIME_UI_SIZE, CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_RANKING_NAME));
		}
		// バツ ( かける )
		else if (nCnt == (int)GAME_UI::CROSS)
		{
			// シーン2Dの生成
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.6f), 60.0f, 0.0f), CROSS_UI_SIZE, CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_RANKING_NAME));
		}
		// ハイスコア
		else if (nCnt == (int)GAME_UI::HIGHSCORE)
		{
			// シーン2Dの生成
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.8f), 50.0f, 0.0f), HIGHSCORE_UI_SIZE, CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_RANKING_NAME));
		}
	}
}

//------------------------------------------------------------------------------
//ゲーム終了
//------------------------------------------------------------------------------
void CGame_2D::GameEnd()
{
	CGame::SetScore(m_nScoreDistance);
	CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RANKING);
	SetGamestate(CGame::STATE_NONE);

}
//------------------------------------------------------------------------------
//タイマー
//------------------------------------------------------------------------------
void CGame_2D::AddTimer()
{
	m_nTime ++;

	if (m_nTime >= 60)
	{
		m_nTime = 0;
		m_nCntTime++;
	}
	m_pTimeNumber->SetMultiNumber(m_nCntTime);
}

//------------------------------------------------------------------------------
// ハイスコアの生成
//------------------------------------------------------------------------------
void CGame_2D::HighScoreCreate()
{
	// ファイルポイント
	FILE *pFile;

	char cReadText[128];			// 文字として読み取る
	char cHeadText[128];			// 比較用
	char cDie[128];					// 不要な文字

	static int nHighScore = 0;

	// ファイルを開く
	pFile = fopen("data/Ranking/RankingData.txt", "r");

	// 開いているとき
	if (pFile != NULL)
	{
		// SCRIPTが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile); // 一文読み込み
			sscanf(cReadText, "%s", &cHeadText);		// 比較用テキストに文字を代入
		}

		// SCRIPTが来たら
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// END_SCRIPTが来るまでループ
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile); // 一文読み込み
				sscanf(cReadText, "%s", &cHeadText);		// 比較用テキストに文字を代入

				// RANKINGSETが来たら
				if (strcmp(cHeadText, "RANKINGSET") == 0)
				{
					// END_RANKINGSETが来るまでループ
					while (strcmp(cHeadText, "END_RANKINGSET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile); // 一文読み込み
						sscanf(cReadText, "%s", &cHeadText);		// 比較用テキストに文字を代入

						if (strcmp(cHeadText, "RANKING_1st") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nHighScore);		// 比較用テキストにRANKIG_1stを代入
						}
						else if (strcmp(cHeadText, "END_RANKINGSET") == 0)
						{
							// ハイスコアの生成
							m_pHighScoreNumber = CMultiNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.8f), 80.0f, 0.0f),
								SCORE_SIZE,
								nHighScore,
								SCORE_DIGITS,
								CScene::OBJTYPE_UI);
						}
					}
				}
			}
		}
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		MessageBox(NULL, "ランキングのデータ読み込み失敗", "警告", MB_ICONWARNING);
	}

}

//------------------------------------------------------------------------------
//ゲームステート設定
//------------------------------------------------------------------------------
void CGame_2D::HighScoreUpdate()
{
	// スコアがハイスコアを超えたとき
	if (m_pScoreNumber->GettMultiNumber() > m_pHighScoreNumber->GettMultiNumber())
	{
		// ハイスコアの上書き
		m_pHighScoreNumber->SetMultiNumber(m_pScoreNumber->GettMultiNumber());
	}
}

//------------------------------------------------------------------------------
//ゲームステート設定
//------------------------------------------------------------------------------
void CGame_2D::SetGamestate(STATE gamestate)
{
	if (GetGamestate() != gamestate)
	{
		CGame::SetGamestate(gamestate);
		//ステートが進んでいる場合

		if (gamestate == CGame::STATE_GAMEOVER)
		{
			m_nCnt = 60;
		}
	}
}
