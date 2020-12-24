//------------------------------------------------------------------------------
//
// フェード処理 [fade.h]
// Author : Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _FADE_H_
#define _FADE_H_

#define _CRT_SECURE_NO_WARNINGS

//------------------------------------------------------------------------------
// インクルードファイル
//------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"

class CFade
{
public:
	// フェードの状態
	typedef enum
	{
		FADE_NONE = 0,								// 何もない状態
		FADE_IN,									// フェードイン処理
		FADE_OUT,									// フェードアウト処理
		FADE_MAX
	} FADESTATE;

	CFade();
	~CFade();
	void Init();									//フェードの初期化処理
	void Uninit();									//フェードの終了処理
	void Update();									//フェードの更新処理
	void Draw();									//フェードの描画処理
	static std::unique_ptr<CFade> Create();			//生成処理

	void SetModeFade(CManager::MODE nextmode);		//フェードセット
	FADESTATE GetFade(void);						//フェード状態取得

private:

	LPDIRECT3DTEXTURE9		m_pTextureFade;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffFade;			// 頂点バッファへのポインタ
	FADESTATE				m_fadestate;			// フェード状態
	CManager::MODE			m_modeNext;				// 次の画面（モード）
	D3DXCOLOR				m_colorFade;			// フェード色

	void UpdateMode();		//更新
};


#endif
