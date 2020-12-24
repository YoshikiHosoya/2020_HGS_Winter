//------------------------------------------------------------------------------
//
//3Dキャラクター処理  [character_3D.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _CHARACTER_3D_H_
#define _CHARACTER_3D_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "character.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModelCharacter;
class CCollision;

//キャラクターのクラス
class CCharacter_3D : public CCharacter
{
public:


	CCharacter_3D();
	virtual ~CCharacter_3D();

	virtual HRESULT Init();									//初期化
	virtual void Uninit();									//終了
	virtual void Update();									//更新
	virtual void Draw();									//描画
	virtual void ShowDebugInfo();							//デバッグ情報表記
	virtual void DamageAction() = 0;						//ダメージ受けた時のリアクション
	virtual void DeathAction() = 0;							//死亡時のリアクション
	virtual void SetState(STATE nextstate);					//ステート変更処理
	virtual void Move();									//移動関数
	virtual void Rot();										//回転関数
	virtual void State();									//状態に応じた処理
	void CreateModelCharacter(int type);
	void SetRotDest(D3DXVECTOR3 const &rotdest) { m_rotDest = rotdest; };										//回転の決定先設定


	D3DXMATRIX		*GetMtx()								{ return &m_mtxWorld; };							//ワールドマトリックス取得
	CModelCharacter	*GetModelCharacterPtr()					{ return m_pModelCharacter.get(); };				//モデルキャラクター情報取得
	D3DXVECTOR3		 &GetRotDest()							{ return m_rotDest; };								//回転の決定先取得

	CMotion::MOTION_TYPE GetNowMotion();																		//現在のモーション取得 インクルードされてないからインライン不可
protected:

	void ShowCharacterInfo();								//キャラクター情報表示
	void ChangeMotion(CMotion::MOTION_TYPE nextmotion);		//モーション切り替え
	bool ContinueAttack();									//連続攻撃
	virtual void Collision();								//当たり判定系の処理
private:
	D3DXVECTOR3 m_rotDest;									//回転の目的地
	D3DXVECTOR3 m_rotDif;									//回転の差分


	D3DXMATRIX m_mtxWorld;									//ワールドマトリックス
	std::unique_ptr<CModelCharacter> m_pModelCharacter;		//モデルのポインタ

};
#endif