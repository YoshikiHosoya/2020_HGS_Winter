//------------------------------------------------------------------------------
//
//当たり判定処理  [collision.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _COLLISION_H_
#define _COLLISION_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"

//------------------------------------------------------------------------------
//前方宣言
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CCollision
{
public:
	CCollision();
	~CCollision();

	//コリジョンの種類
	enum COLLISION_TYPE
	{
		COLLISION_CHARACTER,		//キャラクター
		COLLISION_WEAPON,			//武器
		COLLISION_EXPLOSION,		//爆発
		COLLISION_OBJECT,			//オブジェクト
		COLLISION_CAMERA,			//カメラ
	};
	void ShowDebugInfo();																				//デバッグ情報表記
	static std::unique_ptr<CCollision> Create(D3DXMATRIX *pMtx, COLLISION_TYPE ColisionType);			//生成処理

private:
	D3DXMATRIX *m_pParentMtx;			//持ち主のマトリックス
	MODEL_VTX m_ModelVtx;				//□で判定取るときに使う　最大頂点と最小頂点
	float m_fRadius;					//半径
	float m_fLength;					//判定の長さ

#ifdef _DEBUG
#endif // _DEBUG

};
#endif