//------------------------------------------------------------------------------
//
// フェード処理 [fade.cpp]
// Author : Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// インクルードファイル
//------------------------------------------------------------------------------
#include "fade.h"
#include "renderer.h"
#include "manager.h"
//------------------------------------------------------------------------------
// マクロ定義
//------------------------------------------------------------------------------
#define	TEXTURE_FADE	"data/TEXTURE/other/Fade.png"	// 読み込むテクスチャファイル名
#define FADE_COLOR	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f))
#define FADE_SPEED	(0.06f)
//------------------------------------------------------------------------------
// コンストラクタ
//------------------------------------------------------------------------------
CFade::CFade()
{
	m_colorFade = ZeroColor;
	m_pTextureFade = nullptr;
	m_pVtxBuffFade = nullptr;
	m_fadestate = CFade::FADE_NONE;
	m_modeNext = CManager::MODE_NONE;
}

//------------------------------------------------------------------------------
// デストラクタ
//------------------------------------------------------------------------------
CFade::~CFade()
{
	// テクスチャの開放
	if (m_pTextureFade != NULL)
	{
		m_pTextureFade->Release();
		m_pTextureFade = NULL;
	}

	// 頂点バッファの開放
	if (m_pVtxBuffFade != NULL)
	{
		m_pVtxBuffFade->Release();
		m_pVtxBuffFade = NULL;
	}
}

//------------------------------------------------------------------------------
// 初期化
//------------------------------------------------------------------------------
void CFade::Init()
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//初期化
	m_fadestate = FADE_NONE;	//フェード状態
	m_colorFade = ZeroColor;	// 黒い画面（透明）

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_FADE,		// ファイルの名前
		&m_pTextureFade);	// 読み込むメモリー

	// 頂点情報の設定
	VERTEX_2D *pVtx;

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			//頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuffFade,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = m_colorFade;
	pVtx[1].col = m_colorFade;
	pVtx[2].col = m_colorFade;
	pVtx[3].col = m_colorFade;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点データをアンロックする
	m_pVtxBuffFade->Unlock();
}

//------------------------------------------------------------------------------
// 更新
//------------------------------------------------------------------------------
void CFade::Update()
{

	UpdateMode();
}
//------------------------------------------------------------------------------
// 描画
//------------------------------------------------------------------------------
void CFade::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//------------------------------------------------------------------------------
// モードのフェードセット
//------------------------------------------------------------------------------
void CFade::SetModeFade(CManager::MODE modenext)
{
	//フェードの状態がNONEの時
	if (m_fadestate == FADE_NONE)
	{
		m_fadestate = FADE_OUT;			//フェードアウトの状態にする
		m_modeNext = modenext;
		m_colorFade = FADE_COLOR;	//フェードする時の画面の色
	}
}

//------------------------------------------------------------------------------
// フェード取得
//------------------------------------------------------------------------------
CFade::FADESTATE CFade::GetFade(void)
{
	return m_fadestate;
}

//------------------------------------------------------------------------------
// 生成処理
//------------------------------------------------------------------------------
std::unique_ptr<CFade> CFade::Create()
{
	//メモリ確保
	std::unique_ptr<CFade> pFade(new CFade);

	//初期化
	pFade->Init();

	//リターン
	return pFade;
}

//------------------------------------------------------------------------------
// モード用アップデート
//------------------------------------------------------------------------------
void CFade::UpdateMode()
{
	//頂点バッファ
	VERTEX_2D *pVtx;

	//フェードイン中
	if (m_fadestate == FADE_IN)
	{
		//a値を減らす
		m_colorFade.a -= FADE_SPEED;

		if (m_colorFade.a <= 0.0f)
		{
			//a値を0に
			m_colorFade.a = 0.0f;

			//フェード処理
			m_fadestate = FADE_NONE;
		}
	}
	//フェードアウト中
	else if (m_fadestate == FADE_OUT)
	{
		//透明度を増やす
		m_colorFade.a += FADE_SPEED;

		//a値が1を超えた時
		if (m_colorFade.a > 1.0f)
		{
			//a値を1にする
			m_colorFade.a = 1.0f;

			//フェードイン
			m_fadestate = FADE_IN;

			//次のモードがNONE以外の時
			if (m_modeNext != CManager::MODE_NONE)
			{
				//次のモードにする
				CManager::SetMode(m_modeNext);
			}
			else
			{

			}
		}
	}

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = m_colorFade;
	pVtx[1].col = m_colorFade;
	pVtx[2].col = m_colorFade;
	pVtx[3].col = m_colorFade;

	// 頂点データをアンロックする
	m_pVtxBuffFade->Unlock();
}