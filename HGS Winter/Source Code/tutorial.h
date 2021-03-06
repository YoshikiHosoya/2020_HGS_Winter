//------------------------------------------------------------------------------
//
//チュートリアル処理  [tutorial.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_
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

class CTutorial : public CBaseMode
{
public:
	CTutorial();
	~CTutorial();

	HRESULT Init(HWND hWnd);			//初期化
	void Update();						//更新
	void Draw();						//描画
	void ShowDebugInfo() {};			//デバッグ情報表記

	CPlayer* GetPlayer() { return nullptr; };		//プレイヤー取得処理

private:
	int m_nCntResult;	//カウンタ
};

#endif