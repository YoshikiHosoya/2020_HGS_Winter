//------------------------------------------------------------------------------
//
//シーン2D処理  [scene2D.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "keyboard.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
int CScene2D::m_nNumScene2D = 0;

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CScene2D::CScene2D()
{
	//初期化
	GetPos() = ZeroVector3;
	GetSize() = ZeroVector3;
	m_fAngle = 0.0f;
	m_fLength = 0.0f;

	//総数加算
	m_nNumScene2D++;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CScene2D::~CScene2D()
{
	//総数減算
	m_nNumScene2D--;
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CScene2D::Init()
{
	MakeVertex();

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CScene2D::Update()
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	m_fAngle = atan2f(GetSize().x, GetSize().y);
	m_fLength = (float)sqrt(GetSize().x * GetSize().x + GetSize().y * GetSize().y) / 2;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos.x = GetPos().x + sinf(-D3DX_PI + m_fAngle + GetRot().z)	* m_fLength;
	pVtx[0].pos.y = GetPos().y + cosf(-D3DX_PI + m_fAngle + GetRot().z)	* m_fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = GetPos().x + sinf(D3DX_PI - m_fAngle + GetRot().z)	* m_fLength;
	pVtx[1].pos.y = GetPos().y + cosf(D3DX_PI - m_fAngle + GetRot().z)	* m_fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = GetPos().x + sinf(-m_fAngle + GetRot().z)	* m_fLength;
	pVtx[2].pos.y = GetPos().y + cosf(-m_fAngle + GetRot().z)	* m_fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = GetPos().x + sinf(m_fAngle + GetRot().z)	* m_fLength;
	pVtx[3].pos.y = GetPos().y + cosf(m_fAngle + GetRot().z)	* m_fLength;
	pVtx[3].pos.z = 0.0f;

	//頂点データをアンロック
	GetVtxBuff()->Unlock();
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CScene2D::Draw()
{
	//写すかどうか
	if (GetDisp())
	{
		//デバイス取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		//頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, GetVtxBuff(), 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, GetTexture());

		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
								0,					//開始するインデックス(頂点)
								2);					//ポリゴンの枚数
	}
}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CScene2D::ShowDebugInfo()
{
#ifdef _DEBUG
	//2D情報
	if (ImGui::TreeNode("Scene2DInfo"))
	{
		//座標
		if (ImGui::TreeNode("pos"))
		{
			if (ImGui::InputFloat3("pos", GetPos()))
			{
				//座標設定
				SetPos(GetPos());
			}
			ImGui::TreePop();
		}
		//サイズ
		if (ImGui::TreeNode("size"))
		{
			if (ImGui::InputFloat3("size", GetSize()))
			{
				//座標設定
				SetSize(GetSize());
			}
			ImGui::TreePop();
		}

		//tree終了
		ImGui::TreePop();
	}
#endif //DEBUG
}

//------------------------------------------------------------------------------
//テクスチャアニメーションの更新
//------------------------------------------------------------------------------
void CScene2D::SetAnimation(D3DXVECTOR2 UV, D3DXVECTOR2 size)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(UV.x, UV.y);
	pVtx[1].tex = D3DXVECTOR2(UV.x + size.x, UV.y);
	pVtx[2].tex = D3DXVECTOR2(UV.x, UV.y + size.y);
	pVtx[3].tex = D3DXVECTOR2(UV.x + size.x, UV.y + size.y);

	//頂点データをアンロック
	GetVtxBuff()->Unlock();
}

//------------------------------------------------------------------------------
//色設定
//------------------------------------------------------------------------------
void CScene2D::SetColor(D3DXCOLOR col)
{
	CSceneBase::SetColor(col);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の色
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点データをアンロック
	GetVtxBuff()->Unlock();
}

//------------------------------------------------------------------------------
//生成処理　共有管理用
//------------------------------------------------------------------------------
std::shared_ptr<CScene2D> CScene2D::Create_Shared(D3DXVECTOR3 pos, D3DXVECTOR3 size, OBJTYPE objtype)
{
	//メモリ確保
	std::shared_ptr<CScene2D> pScene2D = std::make_shared<CScene2D>();

	//初期化
	pScene2D->Init();

	//座標とサイズ設定
	pScene2D->SetPos(pos);
	pScene2D->SetSize(size);

	//Sceneで管理
	pScene2D->SetObjType(objtype);
	pScene2D->AddSharedList(pScene2D);

	//return
	return pScene2D;
}
//------------------------------------------------------------------------------
//生成処理　Csceneで管理用
//------------------------------------------------------------------------------
void CScene2D::Create_SceneManagement(D3DXVECTOR3 pos, D3DXVECTOR3 size, OBJTYPE objtype)
{
	//メモリ確保
	std::unique_ptr<CScene2D> pScene2D(new CScene2D);

	//初期化
	pScene2D->Init();

	//座標とサイズ設定
	pScene2D->SetPos(pos);
	pScene2D->SetSize(size);

	//Sceneで管理
	pScene2D->SetObjType(objtype);
	pScene2D->AddUniqueList(std::move(pScene2D));
}
//------------------------------------------------------------------------------
//生成処理　Return先で管理用
//------------------------------------------------------------------------------
std::unique_ptr<CScene2D> CScene2D::Create_SelfManagement(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//メモリ確保
	std::unique_ptr<CScene2D> pScene2D(new CScene2D);


	//座標とサイズ設定
	pScene2D->SetPos(pos);
	pScene2D->SetSize(size);

	//初期化
	pScene2D->Init();

	//return
	return std::move(pScene2D);
}

//------------------------------------------------------------------------------
//頂点バッファ作成
//------------------------------------------------------------------------------
HRESULT CScene2D::MakeVertex()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	//頂点バッファ

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL)))
	{
		//頂点バッファ作成失敗
		return E_FAIL;
	}

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = GetPos() + D3DXVECTOR3(-GetSize().x * 0.5f, -GetSize().y * 0.5f, 0.0f);
	pVtx[1].pos = GetPos() + D3DXVECTOR3(GetSize().x * 0.5f, -GetSize().y * 0.5f, 0.0f);
	pVtx[2].pos = GetPos() + D3DXVECTOR3(-GetSize().x * 0.5f, GetSize().y * 0.5f, 0.0f);
	pVtx[3].pos = GetPos() + D3DXVECTOR3(GetSize().x * 0.5f, GetSize().y * 0.5f, 0.0f);

	//頂点の同次座標
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点の色
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データをアンロック
	pVtxBuff->Unlock();

	//頂点バッファ設定
	BindVtxBuff(pVtxBuff);

	return S_OK;
}
