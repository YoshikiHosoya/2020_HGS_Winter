//------------------------------------------------------------------------------
//
//パーツとなるモデルの処理  [modelparts.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODELPARTS_H_
#define _MODELPARTS_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "sceneX.h"
#include "motion.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class COrbit;
class CModelParts : public CSceneX
{
public:
	CModelParts();
	virtual ~CModelParts();

	virtual HRESULT Init();																	//初期化
	virtual void Uninit();																	//終了
	virtual void Update();																	//更新
	virtual void Draw();																	//描画
	virtual void ShowDebugInfo();															//デバッグ情報表記
	static std::unique_ptr<CModelParts> Create(std::shared_ptr<CModelInfo> pModelInfo);		//生成処理
	void SetPartsInfo(int nIndex,int nParent, D3DXVECTOR3 pos, D3DXVECTOR3 rot);			//パーツの情報設定
	void SetParentMtx(D3DXMATRIX *pParentMtx);												//マトリックス設定
	int GetParent() {return m_nParent;}														//親番号取得
	int GetIndex() { return m_nIndex; }														//自分の番号取得

	void SetMotionRotDest(CMotion::MOTION_TYPE motiontype,int nKey);						//モーションの回転の計算　1F当たりの回転量算出
	void SetForcedRot(CMotion::MOTION_TYPE motiontype, int nKey);							//モーション　強制回転
	void SetMotionPosDest(CMotion::MOTION_TYPE motiontype, int nKey);						//モーションの原点座標設定 1F当たりの移動量算出
	void SetForcedPos(CMotion::MOTION_TYPE motiontype, int nKey);							//モーション　強制座標設定　腰用

protected:

private:
	D3DXVECTOR3	m_rot1F;												//回転の差分
	float m_fHeight1F;													//座標の差分 腰のみ
	int m_nIndex;														//自分のID
	int m_nParent;														//親の番号
	D3DXMATRIX *m_pParentMtx;											//親のマトリックス

};
#endif