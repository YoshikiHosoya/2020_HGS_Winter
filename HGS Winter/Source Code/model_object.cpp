//------------------------------------------------------------------------------
//
//パーツとなるモデルの処理  [modelparts.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "model_object.h"
#include "renderer.h"
#include "manager.h"
#include "collision.h"
#include "modelinfo.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModel_Object::CModel_Object()
{
}
//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModel_Object::~CModel_Object()
{

}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModel_Object::Init()
{
	//初期化
	CSceneX::Init();

	//モデル設定
	CSceneX::BindModelInfo(CModelInfo::GetModelInfo(CModelInfo::MODEL_MAPOBJECT_WALL));

	return S_OK;
}
//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CModel_Object::Uninit()
{
	CSceneX::Uninit();
}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModel_Object::Update()
{
	//更新
	CSceneX::Update();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CModel_Object::Draw()
{
	//デバイス情報取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//マトリックス情報
	D3DXMATRIX			mtxShadow;		//影のワールドマトリックス
	D3DXMATRIX			DrawMtx;		//描画用のワールドマトリックス

	//フォグON
	CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_FOG_ON);

	//マトリックス計算
	CHossoLibrary::CalcMatrix(GetMtxWorldPtr(), GetPos(), GetRot());

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

	//フォグOFF
	CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_FOG_OFF);
}

//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CModel_Object::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}

//------------------------------------------------------------------------------
//生成
//------------------------------------------------------------------------------
void CModel_Object::Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot)
{
	//メモリ確保
	std::shared_ptr<CModel_Object> pModelObject(new CModel_Object);

	//初期化
	pModelObject->Init();

	//情報設定
	pModelObject->SetPos(pos);
	pModelObject->SetRot(rot);

	//コリジョン生成
	pModelObject->m_pCollision = CCollision::Create(pModelObject->GetMtxWorldPtr(), CCollision::COLLISION_OBJECT);

	//マトリックス計算
	CHossoLibrary::CalcMatrix(pModelObject->GetMtxWorldPtr(), ZeroVector3, pModelObject->GetRot());

	//モデルの頂点設定
	MODEL_VTX modelVtx;
	//CHossoLibrary::SetModelVertex(modelVtx, *pModelObject->pModelInfo.get());

	//コリジョンの大きさ設定
	pModelObject->m_pCollision->SetCollisionVtx(modelVtx);

	//オブジェクトタイプ設定
	pModelObject->SetObjType(OBJTYPE_MAPOBJECT);
	pModelObject->AddSharedList(pModelObject);
}