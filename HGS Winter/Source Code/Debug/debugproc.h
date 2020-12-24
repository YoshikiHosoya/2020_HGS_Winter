//------------------------------------------------------------------------------
//
//デバッグ表記処理  [debugproc.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "../main.h"

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define MAX_DATA (4096)

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
//前方宣言
class CDebugProc
{
public:
	//場所
	enum PLACE
	{
		PLACE_LEFT = 0,	//左
		PLACE_RIGHT,	//右
		PLACE_MAX,		//
	};

	CDebugProc();
	~CDebugProc();
	HRESULT Init(HWND hWnd);							//初期化
	static void Print(PLACE place ,char* fmt, ...);		//デバッグ情報格納
	static void Draw(void);								//描画
	static void ResetStr();								//配列初期化

private:
	static LPD3DXFONT m_pFont;							//フォントへのポインタ
	static char m_aStr[PLACE_MAX][MAX_DATA];			//情報を格納する配列
	static RECT m_rect[PLACE_MAX];						//描画位置
};

#endif