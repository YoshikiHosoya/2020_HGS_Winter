//------------------------------------------------------------------------------
//
//ゲーム処理  [game.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _GAME_2D_H_
#define _GAME_2D_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "game.h"
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CWay;
class CScene2D;
class CMultiNumber;

class CGame_2D : public CGame
{
public:
	// ゲームUIの種類
	enum class GAME_UI
	{
		NONE = -1,
		SCORE,				// スコア
		TIME,				// タイム
		CROSS,				// バツ ( かける )
		HIGHSCORE,			// ハイスコア
		GAME_UI_MAX,
	};

	CGame_2D();
	~CGame_2D();
	HRESULT Init(HWND hWnd);								//初期化
	void Update();											//更新
	void Draw();											//描画
	void ShowDebugInfo();									//デバッグ情報表記

	int GetSpeed() { return m_nSpeed; };
	static int GetTime() { return m_nCntTime; };				// 生存時間の取得
	static int GetScore() { return m_nScore; }					// スコアの取得
	DIRECTION GetDirection() { return m_direction; };
	void GameEnd();
	void SetGamestate(CGame::STATE state) override;
	void EnemySpawn();													//敵出現
	void CreateEnemyGroup(D3DXVECTOR3 posOrigin);						//敵の集団生成
	void AddScoreMag();								//スコア倍率
	void AddScore(int nValue);							//スコア加算

	int GetScoreMag() { return m_nScoreMag; };

protected:
private:
	void					GameUICreate();							// ゲームUIの生成
	void					AddTimer();								// タイマー
	void					HighScoreCreate();						// ハイスコアの生成
	void					HighScoreUpdate();						// ハイスコアの更新

	int m_nCnt;
	int m_nSpeed;										//移動速度
	int m_nScoreDistance;								//総合距離
	bool m_bScoreUpdate;								//スコア更新
	DIRECTION m_direction;								//向き

	int m_nTime;										//タイム
	static int m_nScore;								// スコア
	static int m_nCntTime;								// カウントタイム
	int m_nBendingCountDown;							//曲がるときのカウントダウン

	std::shared_ptr<CMultiNumber>				m_pScoreNumber;					// スコア
	std::shared_ptr<CMultiNumber>				m_pTimeNumber;					// タイム
	std::shared_ptr<CMultiNumber>				m_pMagnification;				// 倍率
	std::shared_ptr<CMultiNumber>				m_pHighScoreNumber;				// ハイスコア

	std::vector<std::shared_ptr<CScene2D>>		m_apScene2D;					// ゲームUI

	S_ptr<CScene2D>	m_pReady_Go;												// ready_go
	int m_nScoreMag;
};

#endif