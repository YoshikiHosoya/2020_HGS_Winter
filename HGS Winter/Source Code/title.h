//------------------------------------------------------------------------------
//
//タイトル処理  [title.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _TITLE_H_
#define _TITLE_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "basemode.h"

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define MAX_ENEMY				(3)			// 出現させるてきの数

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CScene2D;

class CTitle : public CBaseMode
{
public:
	enum class TITLE_UI
	{
		NONE = -1,
		TITLE_BLACK,			// 黒
		TITLE_NAME,				// 名前ロゴ
		TITLE_ENTER,			// Enter
		TITLE_MAX,
	};
	//状態
	enum STATE
	{
		STATE_NONE = -1,				//無し
		STATE_START,					//最初の演出
		STATE_NORMAL,					//通常画面
		STATE_FADE,						//フェード
	};
	CTitle();							//コンストラクタ
	~CTitle();							//デストラクタ

	HRESULT Init(HWND hWnd);			//初期化
	void Update();						//更新
	void Draw();						//描画
	void ShowDebugInfo(){};				//デバッグ情報表記

	CPlayer* GetPlayer() { return nullptr; };
private:
	void TitleUICreate();				// タイトルUIの生成
	void EnemyCreate();					// 敵の生成
	void EnemyUpdate();					// 敵の更新

	STATE m_titlestate;					//ステート
	int m_nCntState;					//ステートのカウント
	D3DXVECTOR3	m_EnemyMove[MAX_ENEMY];	// 敵の移動量
	std::vector<std::shared_ptr<CScene2D>>		m_apScene2D;						// タイトルUI
	std::vector<std::shared_ptr<CScene2D>>		m_apEnemy;							// 敵

};

#endif