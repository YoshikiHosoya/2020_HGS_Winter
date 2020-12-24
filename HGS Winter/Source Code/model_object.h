//------------------------------------------------------------------------------
//
//パーツとなるモデルの処理  [modelparts.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODELPARTS_H_
#define _MODELPARTS_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "sceneX.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CCollision;
class CModel_Object : public CSceneX
{
public:
	CModel_Object();
	virtual ~CModel_Object();

	HRESULT Init();														//初期化
	void Uninit();														//終了
	void Update();														//更新
	void Draw();														//描画
	void ShowDebugInfo();												//デバッグ情報表記
	static void Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);				//生成処理
	CCollision *GetCollision() { return m_pCollision.get(); };			//判定の取得

private:
	std::shared_ptr<CCollision> m_pCollision;							//当たり判定のポインタ

};
#endif