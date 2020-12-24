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
#include "ranking.h"
#include "sound.h"
#include "scene2D.h"
#include "multinumber.h"
#include "game.h"
#include "bg.h"

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define RANKING_LOGO_SIZE		(D3DXVECTOR3(700.0f, 200.0f, 0.0f))		// ランキングロゴのサイズ
#define RANKING_SIZE			(D3DXVECTOR3(80.0f, 50.0f, 0.0f))		// ランキングUIのサイズ
#define ANY_BUTTON_UI_SIZE		(D3DXVECTOR3(500.0f, 100.0f, 0.0f))		// エニーボタンのUIサイズ
#define RANKING_SCORE_SIZE		(D3DXVECTOR3(40.0f, 80.0f, 0.0f))		// ランキングスコアサイズ
#define VERTICAL_SPACE			(80.0f)									// 縦の間隔
#define RANKING_SPACE			(10)									// ランキングの間隔
#define RANKING_SCORE_DIGITS	(7)										// ランキングの桁数

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
char *CRanking::m_RankingFileName =
{
	"data/Ranking/RankingData.txt" 			// ランキングの情報
};

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CRanking::CRanking()
{
	// 初期化
	m_nRankingScore.clear();
	m_apScene2D.clear();
	m_apRankScore.clear();
	m_nCntResult = 0;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CRanking::~CRanking()
{
	m_apScene2D.clear();
	m_apRankScore.clear();
}

//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CRanking::Init(HWND hWnd)
{
	//カメラ座標設定
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_FIXED);

	// 背景の生成
	CBg::Create(60);
	// スコアの読み込み
	RankingDataLoad();
	// UI生成
	RankingUICreate();
	// スコア生成
	RankingScoreCreate();
	// ランキングの計算
	RankingCalculation();

	// スコアの書き込み
	RankingDataSave();

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CRanking::Update()
{
	//カウンタ++
	m_nCntResult++;

	// ランキングUIの更新
	for (int nCnt = 0; nCnt < (int)RANKING_UI::RANKING_MAX; nCnt++)
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
				CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_TITLE);
				CManager::GetSound()->Play(CSound::LABEL_SE_DECISION);
		}
	}
}

//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CRanking::Draw()
{
	// ランキングUIの描画
	for (int nCnt = 0; nCnt < (int)RANKING_UI::RANKING_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt])
		{
			m_apScene2D[nCnt]->Draw();
		}
	}
}

//------------------------------------------------------------------------------
//ランキングUIの生成
//------------------------------------------------------------------------------
void CRanking::RankingUICreate()
{
	for (int nCnt = 0; nCnt < (int)RANKING_UI::RANKING_MAX; nCnt++)
	{
		// ランキングタイトルロゴ
		if (nCnt == (int)RANKING_UI::RANKING_NAME)
		{
			// シーン2Dの生成
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), 90.0f, 0.0f), RANKING_LOGO_SIZE, CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_RANKING_NAME));
		}
		// エニーボタン
		else if (nCnt == (int)RANKING_UI::ANY_BUTTON)
		{
			// シーン2Dの生成
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.8f), 650.0f, 0.0f), ANY_BUTTON_UI_SIZE, CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_ENTER));
		}
		// 順位
		else
		{
			// シーン2Dの生成
			m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((SCREEN_WIDTH * 0.35f), (100.0f + (VERTICAL_SPACE * nCnt)) + RANKING_SPACE * nCnt, 0.0f), RANKING_SIZE, CScene::OBJTYPE_UI));
			// テクスチャの割り当て
			m_apScene2D[nCnt]->BindTexture(CTexture::GetTexture((CTexture::TEX_TYPE)(CTexture::TEX_UI_RANKING_1st + nCnt - 1)));
		}
	}
}

//------------------------------------------------------------------------------
//ランキングスコアの生成
//------------------------------------------------------------------------------
void CRanking::RankingScoreCreate()
{
	for (int nCnt = 0; nCnt < (int)RANKING_SCORE::SCORE_MAX; nCnt++)
	{
		// スコアの生成
		m_apRankScore.emplace_back((CMultiNumber::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), ((200.0f - 15.0f) + (VERTICAL_SPACE * nCnt)) + RANKING_SPACE * nCnt, 0.0f),
								RANKING_SCORE_SIZE,
								m_nRankingScore[nCnt],
								RANKING_SCORE_DIGITS,
								CScene::OBJTYPE_UI)));
	}
}


//------------------------------------------------------------------------------
//ランキングのデータのロード
//------------------------------------------------------------------------------
void CRanking::RankingDataLoad()
{
	// ファイルポイント
	FILE *pFile;

	char cReadText[128];			// 文字として読み取る
	char cHeadText[128];			// 比較用
	char cDie[128];					// 不要な文字

	static int nRnak[(int)RANKING_SCORE::SCORE_MAX] = {};

	// ファイルを開く
	pFile = fopen(m_RankingFileName, "r");

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
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nRnak[(int)RANKING_SCORE::SCORE_1st]);		// 比較用テキストにRANKIG_1stを代入
						}
						else if (strcmp(cHeadText, "RANKING_2nd") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nRnak[(int)RANKING_SCORE::SCORE_2nd]);		// 比較用テキストにRANKIG_2ndを代入
						}
						else if (strcmp(cHeadText, "RANKING_3rd") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nRnak[(int)RANKING_SCORE::SCORE_3rd]);		// 比較用テキストにRANKIG_3rdを代入
						}
						else if (strcmp(cHeadText, "RANKING_4th") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nRnak[(int)RANKING_SCORE::SCORE_4th]);		// 比較用テキストにRANKIG_4thを代入
						}
						else if (strcmp(cHeadText, "RANKING_5th") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nRnak[(int)RANKING_SCORE::SCORE_5th]);		// 比較用テキストにRANKIG_5thを代入
						}
						else if (strcmp(cHeadText, "END_RANKINGSET") == 0)
						{
							for (int nCnt = 0; nCnt < (int)RANKING_SCORE::SCORE_MAX; nCnt++)
							{
								// 末尾にランキングのスコアデータを追加
								m_nRankingScore.emplace_back(nRnak[nCnt]);
							}
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
//ランキングのデータの計算
//------------------------------------------------------------------------------
void CRanking::RankingCalculation()
{
	// 末尾にプレイヤーのスコアデータを追加
	m_nRankingScore.emplace_back(CGame::GetScore());
	// スコアを大きい順に入れ替える
	BubbleSort(m_nRankingScore);

	for (int nCnt = 0; nCnt < (int)m_nRankingScore.size(); nCnt++)
	{
		if (m_nRankingScore[nCnt] == CGame::GetScore())
		{
			m_apRankScore[nCnt]->Settype(CMultiNumber::TYPE_FLASHING);
		}
		// マルチナンバーに値を代入
		m_apRankScore[nCnt]->SetMultiNumber((int)m_nRankingScore[nCnt]);
	}
}

//------------------------------------------------------------------------------
//バブルソート
//------------------------------------------------------------------------------
void CRanking::BubbleSort(std::vector<int>& data)
{
	for (int nCnt = 0; nCnt < (int)data.size() - 1; nCnt++)
	{
		for (int num = (int)data.size() - 1; num > nCnt; num--)
		{
			if (data[num - 1] <= data[num])
			{  // 大きさが逆転している箇所があったら swap
				std::swap(data[num - 1], data[num]);
			}
		}
	}

	// 末尾を削除
	data.pop_back();
}

//------------------------------------------------------------------------------
//ランキングセーブ
//------------------------------------------------------------------------------
void CRanking::RankingDataSave()
{
	// ファイルポイント
	FILE	*pFile;

	// 各モデルファイルのファイルを開く
	pFile = fopen(m_RankingFileName, "w");

	// 開いているとき
	if (pFile != NULL)
	{
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "# ランキングデータの情報\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "\n");

		fprintf(pFile, "SCRIPT\n\n");
		fprintf(pFile, "RANKINGSET\n");

		// セーブするランキングの情報
		fprintf(pFile, "	RANKING_1st	= %d\n", m_nRankingScore[(int)RANKING_SCORE::SCORE_1st]);
		fprintf(pFile, "	RANKING_2nd	= %d\n", m_nRankingScore[(int)RANKING_SCORE::SCORE_2nd]);
		fprintf(pFile, "	RANKING_3rd	= %d\n", m_nRankingScore[(int)RANKING_SCORE::SCORE_3rd]);
		fprintf(pFile, "	RANKING_4th	= %d\n", m_nRankingScore[(int)RANKING_SCORE::SCORE_4th]);
		fprintf(pFile, "	RANKING_5th	= %d\n", m_nRankingScore[(int)RANKING_SCORE::SCORE_5th]);


		fprintf(pFile, "END_RANKINGSET\n\n");
		fprintf(pFile, "END_SCRIPT\n");


#ifdef _DEBUG
		// 読み込み成功時の結果表示
		//MessageBox(NULL, "ランキング情報をセーブしました", "結果", MB_OK | MB_ICONINFORMATION);
#endif // DEBUG

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{
#ifdef _DEBUG
		// 読み込み失敗時の警告表示
		MessageBox(NULL, "ランキング情報の読み込み失敗", "警告", MB_ICONWARNING);
#endif // DEBUG
	}
}