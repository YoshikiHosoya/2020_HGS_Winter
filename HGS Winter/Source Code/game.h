//------------------------------------------------------------------------------
//
//ゲーム処理  [game.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _GAME_H_
#define _GAME_H_
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
//前方宣言
class CMap;
class CPlayer;
class CMultiNumber;

class CGame : public CBaseMode
{
public:
	//ゲームの状態遷移
	enum STATE
	{
		STATE_NONE = -1,				//始まってない
		STATE_READY,					//開始前の状態
		STATE_NORMAL,					//通常状態　信仰
		STATE_PAUSE,					//ポーズ状態
		STATE_GAMEOVER,					//ゲームオーバー
		STATE_GAMECLEAR,				//ゲームクリア

		STATE_DEBUG,
		STATE_MAX
	};

	CGame();
	~CGame();
	virtual HRESULT Init(HWND hWnd) = 0;								//初期化
	virtual void Update() = 0;											//更新
	virtual void Draw() = 0;											//描画
	virtual void ShowDebugInfo() = 0;									//デバッグ情報表記

	virtual void SetGamestate(STATE gamestate);									//ゲームの状態設定

	STATE GetGamestate() { return m_gamestate; };						//ゲームの状態取得
	CPlayer *GetPlayer(); 												//プレイヤーのポインタ取得
	static int GetScore() { return m_nScore; };							//スコア取得
	static void SetScore(int nScore) { m_nScore = nScore; };
	int &GetCntState() { return m_nCntState; };						//ステートカウント
protected:
	void SetPlayerPtr(std::shared_ptr<CPlayer> pPlayer) { m_pPlayer = pPlayer; };
private:
	std::shared_ptr<CPlayer> m_pPlayer;					//プレイヤーのポインタ

	STATE m_gamestate;									//ゲームステート
	int m_nCntState;									//ステートのカウンタ
	static int m_nScore;
};

#endif