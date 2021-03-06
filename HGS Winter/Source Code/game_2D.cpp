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
#include "Pad_XInput.h"

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
int CGame_2D::m_nCntTime = 0;
int CGame_2D::m_nScore = 0;

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define DEFAULT_CREATE_POS (2000.0f)
#define WAY_SIZE (400.0f)
#define BENDING_TIME (4)
#define DEFAULT_TIME (20)
#define COUNTDOWN	(3)

#define SCORE_UI_SIZE				(D3DXVECTOR3(100.0f, 20.0f, 0.0f))					// スコアのUIサイズ
#define TIME_UI_SIZE				(D3DXVECTOR3(100.0f, 20.0f, 0.0f))					// タイムのUIサイズ
#define CROSS_UI_SIZE				(D3DXVECTOR3(20.0f, 20.0f, 0.0f))					// バツのUIサイズ
#define HIGHSCORE_UI_SIZE			(D3DXVECTOR3(150.0f, 20.0f, 0.0f))					// ハイスコアのUIサイズ
#define SCORE_SIZE					(D3DXVECTOR3(20.0f, 30.0f, 0.0f))					// スコアのUIサイズ
#define SCORE_DIGITS				(8)													// スコアの桁数
#define TIME_DIGITS					(3)													// タイムの桁数
#define MAGNIFICATION_DIGITS		(3)													// 倍率の桁数

#define PERPLE_APPEAR_FRAME			(2000)
#define RED_APPEAR_FRAME			(3600)


//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CGame_2D::CGame_2D()
{
	m_nScore = 0;
	m_nSpeed = 15;
	m_bScoreUpdate = false;
	m_nScoreMag = 1;
	m_direction = DIRECTION::UP;
	m_nCnt = 0;
	m_nScoreDistance = 0;
	SetScore(0);
	m_nTime = 0;
	m_nCntTime = 0;
	m_nBendingCountDown = COUNTDOWN;
	m_apScene2D.clear();						// ゲームUI
	m_pScoreNumber		= nullptr;				// スコア
	m_pTimeNumber		= nullptr;				// タイム
	m_pMagnification	= nullptr;				// 倍率
	m_pHighScoreNumber	= nullptr;				// ハイスコア
	m_pReady_Go.reset();						// レディーゴー
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
	m_pReady_Go.reset();						// レディーゴー

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

	//敵の情報リセット
	CEnemy_2D::ResetEnemyInfo();

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
	m_pMagnification = CMultiNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.65f), 80.0f, 0.0f),
		SCORE_SIZE,
		0,
		MAGNIFICATION_DIGITS,
		CScene::OBJTYPE_UI);

	// ハイスコアの生成
	HighScoreCreate();

	//ゲームステート初期化
	SetGamestate(CGame::STATE_READY);

	//音再生
	//CManager::GetSound()->Play(CSound::LABEL_SE_READY);

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CGame_2D::Update()
{
	switch (GetGamestate())
	{

	case CGame::STATE_READY:
		m_nCnt--;

		if (m_nCnt < 0)
		{
			SetGamestate(STATE_NORMAL);

			CParticle::CreateFromText(SCREEN_CENTER_POS, ZeroVector3, CParticleParam::EFFECT_GAMEOVER, true);


			if (m_pReady_Go)
			{
				m_pReady_Go->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_GAME_GO));
				CManager::GetSound()->Play(CSound::LABEL_SE_GO);

			}
		}
		break;

	case CGame::STATE_NORMAL:

		//nullcheck
		if (m_pReady_Go)
		{
			m_pReady_Go->SetColor(m_pReady_Go->GetColor() -= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.02f));

			if (m_pReady_Go->GetColor().a <= 0)
			{
				m_pReady_Go.reset();
			}
		}

		// 敵の出現
		EnemySpawn();

		// カウントアップタイマー
		AddTimer();

		// ハイスコアの更新
		HighScoreUpdate();
		break;

	case CGame::STATE_GAMEOVER:
		m_nCnt--;

		CParticle::CreateFromText(GetPlayer()->GetPlayerPos(), ZeroVector3, CParticleParam::EFFECT_GAMEOVER, true);

		GetPlayer()->GetPlayerPos();

		if (m_nCnt < 0)
		{
			CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RANKING);
		}
		break;

	default:
		break;
	}

	if (CManager::GetKeyboard()->GetTrigger(DIK_L))
	{
		CreateEnemyGroup(SCREEN_CENTER_POS);
	}

#ifdef _DEBUG
#endif // _DEBUG


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

#endif //_DEBUG
}

//------------------------------------------------------------------------------
//敵の出現
//------------------------------------------------------------------------------
void CGame_2D::EnemySpawn()
{
	m_nCnt++;

	CDebugProc::Print(CDebugProc::PLACE_LEFT, "EnemuNum >> %d\n", CEnemy_2D::GetEnemyNum());

	if (m_nCnt < 30)
	{
		return;
	}

	if (m_nCnt % 60 == 0)
	{
		CreateEnemyGroup(SCREEN_CENTER_POS + D3DXVECTOR3(CHossoLibrary::Random(600.0f), CHossoLibrary::Random(320.0f), 0.0f));
		CManager::GetSound()->Play(CSound::LABEL_SE_RESPAWN);

		//紫とか赤のタイミング
		if (m_nCnt == PERPLE_APPEAR_FRAME || m_nCnt == RED_APPEAR_FRAME)
		{
			//警告音
			CManager::GetSound()->Play(CSound::LABEL_SE_WARNING);
		}

		if (m_nCnt > PERPLE_APPEAR_FRAME)
		{
			if (m_nCnt % 180 == 0)
			{
				CEnemy_2D::Create(CHossoLibrary::RandomScreenOutPos(ZeroVector3), CEnemy_2D::PURPLE);
			}
		}

		if (m_nCnt > RED_APPEAR_FRAME)
		{
			CEnemy_2D::Create(SCREEN_CENTER_POS + D3DXVECTOR3(CHossoLibrary::Random(600.0f), CHossoLibrary::Random(320.0f), 0.0f), CEnemy_2D::RED);
		}

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
	int nLocalValue = m_nCnt / 300;

	CHossoLibrary::RangeLimit_Equal_Int(nLocalValue, 1, 10);

	for (int nCnt = 0; nCnt < nLocalValue; nCnt++)
	{
		CEnemy_2D::Create(posOrigin + D3DXVECTOR3(CHossoLibrary::Random(70.0f), CHossoLibrary::Random(70.0f), 0.0f), CEnemy_2D::BLUE);
	}
}

//------------------------------------------------------------------------------
//スコア倍率加算
//------------------------------------------------------------------------------
void CGame_2D::AddScoreMag()
{
	//倍率加算
	m_nScoreMag++;

	if (m_nScoreMag % 100 == 0)
	{
		CManager::GetSound()->Play(CSound::LABEL_SE_HUNDREDFOLD);

	}

	//テクスチャ更新
	m_pMagnification->SetMultiNumber(m_nScoreMag);

}

//------------------------------------------------------------------------------
//スコア加算
//------------------------------------------------------------------------------
void CGame_2D::AddScore(int nValue)
{
	//倍率計算
	m_nScore += nValue * m_nScoreMag;

	//テクスチャ更新
	m_pScoreNumber->SetMultiNumber(m_nScore);
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
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_SCORE));
		}
		// タイム
		else if (nCnt == (int)GAME_UI::TIME)
		{
			// シーン2Dの生成
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.4f), 50.0f, 0.0f), TIME_UI_SIZE, CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_TIME));
		}
		// バツ ( かける )
		else if (nCnt == (int)GAME_UI::CROSS)
		{
			// シーン2Dの生成
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.6f), 80.0f, 0.0f), CROSS_UI_SIZE, CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_GAME_CROSS));
		}
		// ハイスコア
		else if (nCnt == (int)GAME_UI::HIGHSCORE)
		{
			// シーン2Dの生成
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.8f), 50.0f, 0.0f), HIGHSCORE_UI_SIZE, CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_GAME_HIGHSCORE));
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

		//スコアまだ更新していなかった時
		if (!m_bScoreUpdate)
		{
			//更新音
			CManager::GetSound()->Play(CSound::LABEL_SE_HIGH_SCORE);

		}

		m_bScoreUpdate = true;
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
		if (gamestate == CGame::STATE_READY)
		{
			m_nCnt = 120;

			m_pReady_Go = CScene2D::Create_Shared(SCREEN_CENTER_POS, D3DXVECTOR3(400.0f, 150.0f, 0.0f), CScene::OBJTYPE_UI);
			m_pReady_Go->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_GAME_READY));
		}

		if (gamestate == CGame::STATE_GAMEOVER)
		{
			m_nCnt = 60;

			//CScene::ReleaseSpecificObject(CScene::OBJTYPE_ENEMY);
			CParticle::CreateFromText(SCREEN_CENTER_POS, ZeroVector3, CParticleParam::EFFECT_IMPACT);
			CManager::GetSound()->Play(CSound::LABEL_SE_DEATH);
			CManager::GetXInput()->StartVibration(40);


		}
	}
}
