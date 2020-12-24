//------------------------------------------------------------------------------
//
//2Dプレイヤー処理  [3Dplayer.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _BOMB_H_
#define _BOMB_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "character_2D.h"

//------------------------------------------------------------------------------
//前方宣言
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CBomb : public CCharacter_2D
{
public:
	CBomb();																						//コンストラクタ
	~CBomb();																						//デストラクタ

	virtual HRESULT Init();																			//初期化
	virtual void Update();																			//更新
	virtual void Draw();																			//描画
	virtual void ShowDebugInfo();																	//デバッグ情報表記

	void DamageAction();																			//ダメージ時のリアクション
	void DeathAction();																				//死亡時のリアクション
	void SetState(STATE nextstate);																	//ステート変更処理
	void Collision() override;

	void Explosion();																				//爆発

	static std::shared_ptr<CBomb> Create(D3DXVECTOR3 pos);											//生成
	static void SetPlayerPosPtr(D3DXVECTOR3 *pPos) { m_pPlayerPos = pPos; };						//プレイヤーの座標のポインタ設定

protected:

private:
	bool m_bMove;
	static D3DXVECTOR3 *m_pPlayerPos;			//プレイヤーの座標
	float m_fRotation;							//回転量

};
#endif