//------------------------------------------------------------------------------
//
//3Dキャラクター処理  [character_3D.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _CHARACTER_2D_H_
#define _CHARACTER_2D_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "character.h"
#include "scene2D.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CScene2D;

//キャラクターのクラス
class CCharacter_2D : public CCharacter
{
public:


	CCharacter_2D();
	virtual ~CCharacter_2D();

	virtual HRESULT Init();									//初期化
	virtual void Uninit();									//終了
	virtual void Update();									//更新
	virtual void Draw();									//描画
	virtual void ShowDebugInfo();							//デバッグ情報表記
	virtual void DamageAction() = 0;						//ダメージ受けた時のリアクション
	virtual void DeathAction() = 0;							//死亡時のリアクション
	virtual void SetState(STATE nextstate);					//ステート変更処理
	void ShowCharacterInfo();
	virtual void Move();									//移動関数
	virtual void Rot();										//回転関数
	virtual void State();									//状態に応じた処理

	void SetScene2D(std::unique_ptr<CScene2D> pScene2D) { m_pScene2D = std::move(pScene2D); };
	CScene2D *GetScene2D() { return m_pScene2D.get(); };
protected:

	virtual void Collision();								//当たり判定系の処理
private:
	std::unique_ptr<CScene2D> m_pScene2D;

};
#endif