//-----------------------------------------------------------------------------
// マウス処理 [mouse.h]
// Author : Yoshiki Hosoya
//
//-----------------------------------------------------------------------------
#ifndef _MOUSE_H_
#define _MOUSE_H_

#define _CRT_SECURE_NO_WARNINGS

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"
#include "input.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define	NUM_BUTTON_MAX			(8)		// ボタン(マウス)の最大数

//-----------------------------------------------------------------------------
// クラス定義
//-----------------------------------------------------------------------------
class CMouse : public CInput
{
public :
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//初期化処理
	void Uninit();									//終了処理
	void Update();									//更新処理
	bool GetPress(int nButton) { return (m_mouseState.rgbButtons[nButton] & 0x80) ? true : false; };					//プレス処理
	bool GetTrigger(int nButton) { return (m_aButtonStateTrigger[nButton] & 0x80) ? true : false; };					//トリガー処理
	bool GetRelease(int nButton) { return (m_aButtonStateRelease[nButton] & 0x80) ? true : false; };					//リリース処理
	D3DXVECTOR2 GetMousePos() { return D3DXVECTOR2(m_mousepos.x, m_mousepos.y); };



private:
	DIMOUSESTATE2 m_mouseState;						//マウス状態
	D3DXVECTOR2 m_mousepos;							//マウス座標

	LPDIRECTINPUT8			m_pMouseput = NULL;						// DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8	m_pDevMouse = NULL;						// 入力デバイス(マウス)へのポインタ
	BYTE					m_aButtonState[NUM_BUTTON_MAX];			// マウスの入力情報ワーク
	BYTE					m_aButtonStateTrigger[NUM_BUTTON_MAX];	// マウスのトリガー
	BYTE					m_aButtonStateRelease[NUM_BUTTON_MAX];	// マウスの放した瞬間

	HWND					m_hWnd;									// ウィンドウハンドル

};
#endif
