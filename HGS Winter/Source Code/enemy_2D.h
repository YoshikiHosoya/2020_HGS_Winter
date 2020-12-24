//------------------------------------------------------------------------------
//
//2Dエネミー処理  [2DEnemy.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _ENEMY_2D_H_
#define _ENEMY_2D_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "enemy.h"
#include "character_2D.h"

//------------------------------------------------------------------------------
//前方宣言
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CEnemy_2D : public CCharacter_2D,public CEnemy
{
public:

	enum ENEMY_TYPE
	{
		BLUE,			//青　普通　□　プレイヤー追尾
		RED,			//赤　早い　⇒　直線
		PURPLE,			//紫　早い　☆　プレイヤー追尾？
	};


	CEnemy_2D();																						//コンストラクタ
	~CEnemy_2D();																						//デストラクタ

	virtual HRESULT Init();																			//初期化
	virtual void Update();																			//更新
	virtual void Draw();																			//描画
	virtual void ShowDebugInfo();																	//デバッグ情報表記
	static std::shared_ptr<CEnemy_2D> Create(D3DXVECTOR3 pos, CEnemy_2D::ENEMY_TYPE type);			//生成

	void Collision() override;
	void DamageAction();																			//ダメージ時のリアクション
	void DeathAction();																				//死亡時のリアクション
	void SetState(STATE nextstate);																	//ステート変更処理

	void SetType(ENEMY_TYPE type);																	//タイプ設定

	ENEMY_TYPE GetEnemyType() { return m_type; };													//敵のタイプ取得
	D3DXVECTOR3 GetEnemyPos() override { return GetPos(); };										//座標取得

	void MoveAI();																					//移動処理

	static void SetPlayerPosPtr(D3DXVECTOR3 *pPos) { m_pPlayerPos = pPos; };						//プレイヤーの座標のポインタ設定
	static int GetEnemyNum() { return m_nNumEnemy; };
protected:

private:
	bool m_bMove;								//移動してるかどうか
	ENEMY_TYPE m_type;							//タイプ
	D3DXVECTOR3 m_DifPos;						//座標の差分
	static D3DXVECTOR3 *m_pPlayerPos;			//プレイヤーの座標
	static int m_nNumEnemy;						//敵の総数

};
#endif