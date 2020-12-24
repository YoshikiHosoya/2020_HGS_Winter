//------------------------------------------------------------------------------
//
//道  [way.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _WAY_H_
#define _WAY_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "scene2D.h"

//------------------------------------------------------------------------------
//前方宣言
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CWay : public CScene2D
{
public:

	enum WAY_TYPE
	{
		UP,
		RIGHT_01,
		RIGHT_02,
		LEFT_01,
		LEFT_02,
	};

	CWay();																					//コンストラクタ
	~CWay();																				//デストラクタ

	virtual HRESULT Init();																	//初期化
	virtual void Uninit();																	//終了
	virtual void Update();																	//更新
	virtual void Draw();																	//描画
	virtual void ShowDebugInfo();															//デバッグ情報表記
	static std::shared_ptr<CWay> Create(D3DXVECTOR3 pos,WAY_TYPE waytype);					//生成

	WAY_TYPE GetWayType() { return m_waytype; };

	bool Collision(D3DXVECTOR3 pos);					//どのＷａｙにいるか
	bool CollisionPlayerHit(D3DXVECTOR3 pos);			//プレイヤーの衝突判定用
	bool CollisionPlayerAddTimer(D3DXVECTOR3 pos);						//どれくらいギリギリでかわしたか判定してタイマー加算

protected:

private:
	WAY_TYPE m_waytype;

};
#endif