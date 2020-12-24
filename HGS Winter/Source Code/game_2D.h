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
	static int GetTime() { return m_nCntTime; };					// 生存時間の取得
	DIRECTION GetDirection() { return m_direction; };
	void GameEnd();
	void SetGamestate(CGame::STATE state);
	void EnemySpawn();													//敵出現
	void CreateEnemyGroup(D3DXVECTOR3 posOrigin);						//敵の集団生成
	void AddScoreMag() { m_nScoreMag++; };								//スコア倍率

protected:
private:
	void					GameUICreate();							// ゲームUIの生成
	void					AddTimer();								// タイマー
	void					HighScoreCreate();						// ハイスコアの生成
	void					HighScoreUpdate();						// ハイスコアの更新

	int m_nCnt;
	int m_nSpeed;										//移動速度
	int m_nScoreDistance;								//総合距離
	DIRECTION m_direction;								//向き

	float m_fNextBendingPoint;							//次の曲がり角までの距離
	D3DXVECTOR3 *m_pNextBendingWayPos;					//次の曲がり箇所の座標
	DIRECTION m_NextBendingDirection;					//次の曲がり角の方向
	bool m_bBendingFlag;								//曲がりのフラグ
	int m_nBendingTime;									//曲がってる時間
	int m_nTime;										//タイム
	static int m_nCntTime;								// カウントタイム
	int m_nBendingCountDown;							//曲がるときのカウントダウン

	std::shared_ptr<CMultiNumber>				m_pScoreNumber;					// スコア
	std::shared_ptr<CMultiNumber>				m_pTimeNumber;					// タイム
	std::shared_ptr<CMultiNumber>				m_pMagnification;				// 倍率
	std::shared_ptr<CMultiNumber>				m_pHighScoreNumber;				// ハイスコア

	std::vector<std::shared_ptr<CScene2D>>		m_apScene2D;					// ゲームUI 
	int m_nScoreMag;
};

#endif