//------------------------------------------------------------------------------
//
//3Dプレイヤー処理  [3Dplayer.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _PLAYER_3D_H_
#define _PLAYER_3D_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "player.h"
#include "character_3D.h"

//------------------------------------------------------------------------------
//前方宣言
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CPlayer_3D : public CCharacter_3D,public CPlayer
{
public:
	CPlayer_3D();															//コンストラクタ
	~CPlayer_3D();															//デストラクタ

	virtual HRESULT Init();												//初期化
	virtual void Uninit();												//終了
	virtual void Update();												//更新
	virtual void Draw();												//描画
	virtual void ShowDebugInfo();										//デバッグ情報表記
	void DamageAction();												//ダメージ時のリアクション
	void DeathAction();													//死亡時のリアクション
	void SetState(STATE nextstate);										//ステート変更処理
	static std::shared_ptr<CPlayer_3D> Create(D3DXVECTOR3 pos);			//生成

	D3DXVECTOR3 GetPlayerPos() override { return GetPos(); };
protected:

private:
};
#endif