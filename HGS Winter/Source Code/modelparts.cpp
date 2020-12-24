//------------------------------------------------------------------------------
//
//パーツとなるモデルの処理  [modelparts.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "modelparts.h"
#include "renderer.h"
#include "manager.h"

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModelParts::CModelParts()
{
	//初期化
	m_pParentMtx = nullptr;
	m_nIndex = 0;
	m_nParent = 0;
	m_rot1F = ZeroVector3;
}
//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModelParts::~CModelParts()
{
	m_pParentMtx = nullptr;
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModelParts::Init()
{
	//初期化
	CSceneX::Init();

	return S_OK;
}
//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CModelParts::Uninit()
{
	CSceneX::Uninit();
}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModelParts::Update()
{
	//現在の回転取得
	D3DXVECTOR3 &rRot = GetRot();

	//回転に加算
	rRot += m_rot1F;

	//3.14以内にする
	CHossoLibrary::CalcRotation(rRot.y);

	//原点の場合
	if (m_nIndex == 0)
	{
		//原点の座標更新
		D3DXVECTOR3 &rPos = GetPos();
		rPos.y += m_fHeight1F;
	}

	//更新
	CSceneX::Update();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CModelParts::Draw()
{
	//デバイス情報取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//マトリックス情報
	D3DXMATRIX			mtxShadow;		//影のワールドマトリックス
	D3DXMATRIX			DrawMtx;		//描画用のワールドマトリックス

	//マトリックス計算
	CHossoLibrary::CalcMatrix(GetMtxWorldPtr(), GetPos(), GetRot());

	//親の情報をいれる
	D3DXMatrixMultiply(GetMtxWorldPtr(), GetMtxWorldPtr(), m_pParentMtx);

	//モデルの描画
	CSceneX::DrawMesh();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	//モデルのワールド座標取得
	pDevice->GetTransform(D3DTS_WORLD, &DrawMtx);

	//影のマトリックス計算
	CHossoLibrary::CalcShadowMatrix(mtxShadow, D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	//描画用のマトリックスにかけ合わせる
	D3DXMatrixMultiply(&DrawMtx, &DrawMtx, &mtxShadow);

	//モデル座標からワールド座標に設定
	pDevice->SetTransform(D3DTS_WORLD, &DrawMtx);

	//影の描画
	CSceneX::DrawShadow();
}

//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CModelParts::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}

//------------------------------------------------------------------------------
//親のマトリックス設定
//------------------------------------------------------------------------------
void CModelParts::SetParentMtx(D3DXMATRIX *pParentMtx)
{
	m_pParentMtx = pParentMtx;
}

//------------------------------------------------------------------------------
//いろんな情報セット
//------------------------------------------------------------------------------
void CModelParts::SetPartsInfo(int nIndex, int nParent, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CSceneX::SetPos(pos);
	CSceneX::SetRot(rot);
	m_nParent = nParent;
	m_nIndex = nIndex;
}

//------------------------------------------------------------------------------
//生成
//------------------------------------------------------------------------------
std::unique_ptr<CModelParts> CModelParts::Create(std::shared_ptr<CModelInfo> pModelInfo)
{
	//メモリ確保
	std::unique_ptr<CModelParts> pModelParts(new CModelParts);

	//初期化
	pModelParts->Init();

	//モデル設定
	pModelParts->BindModelInfo(pModelInfo);

	//生成した情報
	return std::move(pModelParts);
}
//------------------------------------------------------------------------------
//回転の行先を設定
//------------------------------------------------------------------------------
void CModelParts::SetMotionRotDest(CMotion::MOTION_TYPE motiontype, int nKey)
{
	//変数宣言
	D3DXVECTOR3 rotDest = CMotion::GetRotDest(motiontype, nKey,m_nIndex);		//回転の行先
	D3DXVECTOR3 rot = GetRot();													//現在の回転値
	int nFrame = CMotion::GetFrame(motiontype, nKey);							//かかるフレーム数

	//差分計算
	D3DXVECTOR3 rotDif = rotDest - rot;

	//3.14以内にする
	CHossoLibrary::CalcRotation(rotDif.y);

	//1Fあたりの回転量を取得
	m_rot1F = rotDif / (float)nFrame;
}
//------------------------------------------------------------------------------
//回転の行先に回転
//------------------------------------------------------------------------------
void CModelParts::SetForcedRot(CMotion::MOTION_TYPE motiontype, int nKey)
{
	//回転量設定
	CSceneX::SetRot(CMotion::GetRotDest(motiontype, nKey, m_nIndex));
}

//------------------------------------------------------------------------------
//原点の1F当たりの移動
//------------------------------------------------------------------------------
void CModelParts::SetMotionPosDest(CMotion::MOTION_TYPE motiontype, int nKey)
{
	//変数宣言
	float fHeightDest = CMotion::GetOriginHeight(motiontype, nKey);				//回転の行先
	float fNowHeight = GetPos().y;												//現在の回転値
	int nFrame = CMotion::GetFrame(motiontype, nKey);							//かかるフレーム数

	//差分計算
	float fHeightDif = fHeightDest - fNowHeight;

	//1Fあたりの回転量を取得
	m_fHeight1F = fHeightDif / (float)nFrame;
}

//------------------------------------------------------------------------------
//回転の行先に回転
//------------------------------------------------------------------------------
void CModelParts::SetForcedPos(CMotion::MOTION_TYPE motiontype, int nKey)
{
	//原点の高さ取得して設定
	CSceneX::SetPos(D3DXVECTOR3(GetPos().x, (CMotion::GetOriginHeight(motiontype, nKey)), GetPos().z));
}