//------------------------------------------------------------------------------
//
//2Dプレイヤー処理  [3Dplayer.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _PLAYER_2D_H_
#define _PLAYER_2D_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "player.h"
#include "character_2D.h"

//------------------------------------------------------------------------------
//前方宣言
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CPlayer_2D : public CCharacter_2D,public CPlayer
{
public:
	CPlayer_2D();															//コンストラクタ
	~CPlayer_2D();															//デストラクタ

	virtual HRESULT Init();												//初期化
	virtual void Uninit();												//終了
	virtual void Update();												//更新
	virtual void Draw();												//描画
	virtual void ShowDebugInfo();										//デバッグ情報表記
	D3DXVECTOR3 GetPlayerPos() override { return GetPos(); };
	void DamageAction();												//ダメージ時のリアクション
	void DeathAction();													//死亡時のリアクション
	void SetState(STATE nextstate);										//ステート変更処理
	void MoveInput();													//移動の入力
	static std::shared_ptr<CPlayer_2D> Create(D3DXVECTOR3 pos);			//生成

protected:

private:
	bool m_bMove;

};
#endif