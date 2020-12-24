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
	CGame_2D();
	~CGame_2D();
	HRESULT Init(HWND hWnd);								//初期化
	void Update();											//更新
	void Draw();											//描画
	void ShowDebugInfo();									//デバッグ情報表記

	int GetSpeed() { return m_nSpeed; };
	DIRECTION GetDirection() { return m_direction; };
	void GameEnd();
	void AddTimer(int nAddTime);
	void SetGamestate(CGame::STATE state);
	void EnemySpawn();													//敵出現
	void CreateEnemyGroup(D3DXVECTOR3 posOrigin);						//敵の集団生成

protected:
private:
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
	int m_nBendingCountDown;							//曲がるときのカウントダウン

	std::shared_ptr<CMultiNumber> m_pScoreNumber;
	std::shared_ptr<CMultiNumber> m_pTimeNumber;
	std::shared_ptr<CScene2D> m_pScore;

	std::shared_ptr<CScene2D> m_pReadyGo;


};

#endif