//------------------------------------------------------------------------------
//
//ランキング処理  [ranking.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _RANKING_H_
#define _RANKING_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "basemode.h"
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CMultiNumber;
class CScene2D;

class CRanking : public CBaseMode
{
public:

	// ランキングUIの種類
	enum class RANKING_UI
	{
		NONE = -1,
		RANKING_NAME,				// 名前ロゴ
		RANKING_SCORE,				// プレイヤースコア
		RANKING_1st,				// 順位1位
		RANKING_2nd,				// 順位2位
		RANKING_3rd,				// 順位3位
		RANKING_4th,				// 順位4位
		RANKING_5th,				// 順位5位
		RANKING_GO_TITLE,			// タイトルへ
		RANKING_MAX,
	};

	// ランキングデータ
	enum class RANKING_SCORE
	{
		NONE = -1,
		SCORE_1st,					// 順位1位
		SCORE_2nd,					// 順位2位
		SCORE_3rd,					// 順位3位
		SCORE_4th,					// 順位4位
		SCORE_5th,					// 順位5位
		SCORE_MAX,
	};

	CRanking();
	~CRanking();

	HRESULT Init(HWND hWnd);			//初期化
	void Update();						//更新
	void Draw();						//描画
	void ShowDebugInfo() {};			//デバッグ情報表記

	CPlayer* GetPlayer() { return nullptr; };		//プレイヤー取得処理

private:
	void					RankingUICreate();										// ランキングUIの生成
	void					RankingScoreCreate();									// ランキングスコアの生成
	void					PlayerScoreCreate();									// プレイヤースコアの生成
	void					RankingDataLoad();										// ランキングのデータのロード
	void					RankingCalculation();									// 順位の計算
	void					BubbleSort(std::vector<int> &data);						// 優先度の入れ替え 高い順に0番目から
	void					RankingDataSave();										// ランキングのデータのセーブ

	static char				*m_RankingFileName;										// ランキングのファイル名

	std::vector<int>							m_nRankingScore;					// 上位5位のランキング情報
	std::vector<std::shared_ptr<CMultiNumber>>	m_pPlayerScore;						// スコアのUI
	std::vector<std::shared_ptr<CScene2D>>		m_apScene2D;						// ランキングUI 
	std::vector<std::shared_ptr<CMultiNumber>>	m_apRankScore;						// スコアのUI
	int											m_nCntResult;						// カウンタ

};

#endif