//------------------------------------------------------------------------------
//
//2Dプレイヤー処理  [3Dplayer.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _SCOREUP_ITEM_H_
#define _SCOREUP_ITEM_H_
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
class CScoreUpItem : public CCharacter_2D
{
public:
	CScoreUpItem();																						//コンストラクタ
	~CScoreUpItem();																						//デストラクタ

	virtual HRESULT Init();																			//初期化
	virtual void Update();																			//更新
	virtual void Draw();																			//描画
	virtual void ShowDebugInfo();																	//デバッグ情報表記

	void DamageAction();																			//ダメージ時のリアクション
	void DeathAction();																				//死亡時のリアクション
	void SetState(STATE nextstate);																	//ステート変更処理
	void Collision() override;

	static std::shared_ptr<CScoreUpItem> Create(D3DXVECTOR3 pos);											//生成
	static void SetPlayerPosPtr(D3DXVECTOR3 *pPos) { m_pPlayerPos = pPos; };						//プレイヤーの座標のポインタ設定

protected:

private:
	bool m_bMove;
	static D3DXVECTOR3 *m_pPlayerPos;			//プレイヤーの座標

};
#endif