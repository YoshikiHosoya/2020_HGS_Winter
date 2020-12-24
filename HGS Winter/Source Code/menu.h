//------------------------------------------------------------------------------
//
//メニュー処理  [menu.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MENU_H_
#define _MENU_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CScene2D;

class CMenu
{
public:
	CMenu();
	~CMenu();

	enum TYPE
	{
		MENU_VERTICAL = 0,		//縦のメニュー
		MENU_HORIZON,			//横のメニュー
		MAX
	} ;

	HRESULT Init();					//初期化
	void Uninit();					//終了
	void Update();					//更新
	void Draw();					//描画
	void ShowDebugInfo();			//デバッグ情報表記
	static std::unique_ptr<CMenu> Create(D3DXVECTOR3 pos,D3DXVECTOR3 onesize, LPDIRECT3DTEXTURE9 pTex,D3DXVECTOR2 UVCnt,CMenu::TYPE type);	//生成処理
	void MenuSelect();				//メニュー選択処理
	int GetSelectNum();				//現在選択しているメニュー番号取得

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;						//テクスチャへのポインタ
	std::vector<std::shared_ptr<CScene2D>> m_apScene2DList;		//2Dポリゴンのポリゴン
	D3DXVECTOR2 m_UVCnt;										//UVの枚数
	D3DXVECTOR2 m_UVSize;										//UVの大きさ
	TYPE m_type;												//種類
	int m_nChoiseNum;											//選択肢の数
	int m_nSelectNum;											//決定の番号

};

#endif