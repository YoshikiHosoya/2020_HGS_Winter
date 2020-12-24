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
class CScene2D;

class CResult : public CBaseMode
{
public:

	// リザルトUIの種類
	enum class RESULT_UI
	{
		NONE = -1,
		RESULT_SCORE,				// 名前ロゴ
		SURVIVED_TIME,				// 生存時間
		KILL,						// キル
		ANY_BUTTON,					// エニーボタン
		RESULT_MAX,
	};

	CResult();
	~CResult();

	HRESULT Init(HWND hWnd);			//初期化
	void Update();						//更新
	void Draw();						//描画
	void ShowDebugInfo() {};			//デバッグ情報表記

	CPlayer* GetPlayer() { return nullptr; };		//プレイヤー取得処理

private:
	void					ResultUICreate();										// リザルトUIの生成
	void					ResultScoreCreate();									// リザルトスコアの生成
	void					SurvivedTimeCreate();									// プレイヤースコアの生成
	void					NumKillCreate();										// キル数の生成

	int m_nCntResult;	//カウンタ
	std::vector<std::shared_ptr<CScene2D>>		m_apScene2D;						// ランキングUI 
	std::shared_ptr<CMultiNumber>				m_pResultScore;						// リザルトスコア
	std::shared_ptr<CMultiNumber>				m_pSurvivedTime;					// 生存時間
	std::shared_ptr<CMultiNumber>				m_pNumKill;							// キル数

};

#endif