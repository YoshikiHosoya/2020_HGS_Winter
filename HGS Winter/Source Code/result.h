//------------------------------------------------------------------------------
//
//リザルト処理  [result.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _RESULT_H_
#define _RESULT_H_
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

class CResult : public CBaseMode
{
public:

	// リザルトUIの種類
	enum class RANKING_UI
	{
		NONE = -1,
		RESULT_SCORE,				// 名前ロゴ
		RANKING_SCORE,				// プレイヤースコア
		LIVED_TIME,					// 生存時間
		KILL,						// キル
		ANY_BUTTON,					// エニーボタン
		RANKING_MAX,
	};

	CResult();
	~CResult();

	HRESULT Init(HWND hWnd);			//初期化
	void Uninit();						//終了
	void Update();						//更新
	void Draw();						//描画
	void ShowDebugInfo() {};			//デバッグ情報表記

	CPlayer* GetPlayer() { return nullptr; };		//プレイヤー取得処理

private:
	int m_nCntResult;	//カウンタ
	//std::vector<std::shared_ptr<CScene2D>>		m_apScene2D;						// ランキングUI 

};

#endif