//------------------------------------------------------------------------------
//
//シーン2D処理  [scene2D.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _SCENEBASE_H_
#define _SCENEBASE_H_

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CSceneBase :public CScene
{
public:
	CSceneBase();													//コンストラクタ
	virtual	~CSceneBase();											//デストラクタ
	virtual	HRESULT Init(void)	= 0;								//初期化
	virtual	void Uninit(void)	= 0;								//終了
	virtual	void Update(void)	= 0;								//更新
	virtual	void Draw(void)		= 0;								//描画
	virtual void ShowDebugInfo() = 0;								//デバッグ情報表記

	//Set関数
	//仮想関数　頂点バッファの再調整とかオーバーライドでしてもらう
	virtual void SetPos(D3DXVECTOR3 pos)							{ m_pos = pos; };					//座標
	virtual void SetSize(D3DXVECTOR3 size)							{ m_size = size; };					//サイズ
	virtual void SetColor(D3DXCOLOR col)							{ m_col = col; };					//色
	virtual void SetRot(D3DXVECTOR3 rot)							{ m_rot = rot; };					//回転量
	virtual void SetAnimation(D3DXVECTOR2 UV, D3DXVECTOR2 UVsize) {};

	void BindTexture(LPDIRECT3DTEXTURE9 tex)				{ m_pTexture = tex; };						//テクスチャ
	void BindVtxBuff(LPDIRECT3DVERTEXBUFFER9 pVtxBuff)		{ m_pVtxBuff = pVtxBuff; };					//頂点バッファ
	void SetMtxWorld(D3DXMATRIX mtxWorld)					{ m_mtxWorld = mtxWorld; };					//ワールドマトリックス
	void SetDisp(bool bDisp)								{ m_bDisp = bDisp; };						//表示非表示

	//Get関数
	D3DXVECTOR3 &GetPos(void)										{ return m_pos; };					//座標
	D3DXVECTOR3 *GetPosPtr()										{ return &m_pos; };					//座標のポインタ　当たり判定とかはこれを使う
	D3DXVECTOR3 &GetSize(void)										{ return m_size; };					//サイズ
	D3DXCOLOR &GetColor(void)										{ return m_col; };					//色
	D3DXVECTOR3 &GetRot(void)										{ return m_rot; };					//回転量
	LPDIRECT3DTEXTURE9 GetTexture(void)								{ return m_pTexture; };				//テクスチャ
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void)						{ return m_pVtxBuff; };				//頂点バッファ
	D3DXMATRIX *GetMtxWorldPtr(void)								{ return &m_mtxWorld; };			//ワールドマトリックス
	bool GetDisp()													{ return m_bDisp; };				//表示非表示
protected:

private:
	LPDIRECT3DTEXTURE9 m_pTexture;							//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;						//頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;										//座標
	D3DXVECTOR3 m_size;										//サイズ
	D3DXCOLOR m_col;										//色
	D3DXVECTOR3 m_rot;										//回転量
	D3DXMATRIX m_mtxWorld;									//ワールドマトリックス
	bool m_bDisp;											//表示非表示の設定

};
#endif