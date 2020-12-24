//------------------------------------------------------------------------------
//
//キャラクター処理  [character.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _CHARACTER_H_
#define _CHARACTER_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModelCharacter;
class CCollision;

//キャラクターのクラス
class CCharacter : public CScene
{
public:

	//状態
	enum STATE
	{
		STATE_NONE = -1,		//none
		STATE_APPEAR,			//出現
		STATE_NORMAL,			//通常状態
		STATE_DAMAGE,			//被ダメージ中
	};

	enum PARAM
	{
		PARAM_PLAYER,
		PARAM_MAX,
	};

	CCharacter();
	virtual ~CCharacter();

	virtual HRESULT Init() = 0;									//初期化
	virtual void Update() = 0;									//更新
	virtual void Draw() = 0;									//描画
	virtual void ShowDebugInfo();								//デバッグ情報表記
	virtual void SetState(STATE nextstate);						//ステート変更処理
	void SetParam(PARAM param);									//パラメータ設定
	bool ApplyDamage(int nDamage);								//ダメージ処理

	//Set関数
	void SetPos(D3DXVECTOR3 const &pos)						{ m_pos = pos; };												//座標設定
	void SetMove(D3DXVECTOR3 const &move)					{ m_move = move; };												//移動設定
	void SetRot(D3DXVECTOR3 const &rot)						{ m_rot = rot; }; 												//回転設定
	void SetLife(int nLife)									{ m_nLife = nLife; };											//ライフ
	void SetAttack(bool bAttack)							{ m_bAttack = bAttack; };										//攻撃してる状態
	void SetJump(bool bJump)								{ m_bJump = bJump; };											//ジャンプ状態
	void SetInvincible(bool bInvincible)					{ m_bInvincible = bInvincible; };								//無敵時間かどうか
	void SetAlive(bool bAlive)								{ m_bAlive = bAlive; };											//生きてるかどうか
	void SetCntState(int nCntState)							{ m_nCntState = nCntState; };									//カウントステート設定
	void SetGravity(bool bGravity)							{ m_bGravity = bGravity; };										//グラビティ設定

	//Get関数
	D3DXVECTOR3		 &GetPos()								{ return m_pos; };									//座標取得
	D3DXVECTOR3		 &GetMove()								{ return m_move; };									//移動取得
	D3DXVECTOR3		 &GetRot()								{ return m_rot; };									//回転取得
	CCollision		*GetCollision()							{ return m_pCollision.get(); };						//コリジョン   のポインタ取得
	int				&GetCntState()							{ return m_nCntState; };							//カウントステート取得
	int				GetLife()								{ return m_nLife; };								//ライフ取得
	bool			&GetJump()								{ return m_bJump; };								//ジャンプしているかどうかの取得
	bool			GetAttack()								{ return m_bAttack; };								//攻撃してるかどうか取得
	bool			GetInvincible()							{ return m_bInvincible; };							//無敵かどうか取得
	bool			GetGravity()							{ return m_bGravity; };								//重力かけるかどうか
	bool			GetAlive()								{ return m_bAlive; };								//生きていたかどうか
	STATE			GetState()								{ return m_State; };								//状態取得
	PARAM			GetParam()								{ return m_Param; };								//パラメータ取得

	D3DXVECTOR3		*GetPosPtr()							{ return &m_pos; };									//座標のポインタ取得

protected:
	void ShowCharacterInfo();								//キャラクター情報表示

	//純粋仮想関数　継承先がオーバーライドする
	virtual void State() = 0;								//状態に応じた処理
	virtual void Collision() = 0;							//当たり判定系の処理
	virtual void Move() = 0;								//移動関数
	virtual void Rot() = 0;									//回転関数
	virtual void DamageAction() = 0;						//ダメージ受けた時のリアクション
	virtual void DeathAction() = 0;							//死亡時のリアクション

private:
	D3DXVECTOR3 m_pos;										//座標
	D3DXVECTOR3 m_posOld;									//1F前の座標
	D3DXVECTOR3 m_move;										//移動距離
	D3DXVECTOR3 m_rot;										//現在の回転
	std::unique_ptr<CCollision> m_pCollision;				//コリジョンのポインタ
	STATE m_State;											//キャラクターの状態
	PARAM m_Param;											//キャラクターのパラメータの種類

	bool m_bJump;											//ジャンプしているかどうか
	bool m_bAttack;											//攻撃してるか
	bool m_bInvincible;										//無敵状態かどうか
	bool m_bGravity;										//重力かかるかどうか
	bool m_bAlive;											//生きてるか

	int m_nLife;											//HP
	int m_nCntState;										//状態遷移のカウンタ
};
#endif