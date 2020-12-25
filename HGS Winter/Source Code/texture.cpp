//-----------------------------------------------------------------------------
//
//テクスチャ管理  [texture.cpp]
// Author : Yoshiki Hosoya
//
//-----------------------------------------------------------------------------
#include "texture.h"
#include "manager.h"
#include "renderer.h"
//-----------------------------------------------------------------------------
//静的メンバ変数の初期化
//-----------------------------------------------------------------------------
std::vector<LPDIRECT3DTEXTURE9> CTexture::m_apTextureList = {};

//テクスチャ名の設定
FILENAME_LIST CTexture::m_aTexFileName =
{
	{ "data/TEXTURE/TexNone.png" },									//白紙
	{ "data/TEXTURE/Effect/particle.jpg" },							//パーティクル用の丸いの
	{ "data/TEXTURE/Character/Player/Player000.png" },				//プレイヤー
	{ "data/TEXTURE/Character/Enemy/Blue.png" },					//エネミー 青
	{ "data/TEXTURE/Character/Enemy/red.png" },						//エネミー 赤
	{ "data/TEXTURE/Character/Enemy/purple.png" },					//エネミー 紫

	{ "data/TEXTURE/Item/Bomb.png" },								//ボム
	{ "data/TEXTURE/Item/Mitokondoria.png" },						//ミトコンドリア


	{ "data/TEXTURE/UI/BG/GridLine.png" },							//グリッド線

	{ "data/TEXTURE/UI/Title/Title.png" },							//タイトルロゴ

	{ "data/TEXTURE/UI/Ranking/Ranking.png" },						//ランキングロゴ
	{ "data/TEXTURE/UI/Ranking/Total.png" },						//ランキングスコア
	{ "data/TEXTURE/UI/Ranking/rank_1.png" },						//1位
	{ "data/TEXTURE/UI/Ranking/rank_2.png" },						//2位
	{ "data/TEXTURE/UI/Ranking/rank_3.png" },						//3位
	{ "data/TEXTURE/UI/Ranking/rank_4.png" },						//4位
	{ "data/TEXTURE/UI/Ranking/rank_5.png" },						//5位
	{ "data/TEXTURE/UI/Ranking/gototitle.png" },					//タイトルへ

	{ "data/TEXTURE/UI/Press_Lstick.png" },							//Enter
	{ "data/TEXTURE/UI/Num.png" },									//ナンバー
	{ "data/TEXTURE/UI/score.png" },								//スコア
	{ "data/TEXTURE/UI/time.png" },									//タイム

	{ "data/TEXTURE/UI/Game/cross.png" },							//バツ ( かける )
	{ "data/TEXTURE/UI/Game/highscore.png" },						//ハイスコア
	{ "data/TEXTURE/UI/Game/ready_go.png" },						//Ready

	{ "data/TEXTURE/UI/Result/Kill.png" },							//キル
};

//テクスチャ名の設定
FILENAME_LIST CTexture::m_aSeparateTexFileNameList =
{
	{ "data/TEXTURE/TexNone.png"},									//白紙
	{ "data/TEXTURE/Effect/Explosion00.png" },						//爆発
	{ "data/TEXTURE/Effect/Impact00.png" },							//インパクト
	{ "data/TEXTURE/Effect/Impact01.png" },							//インパクト
	{ "data/TEXTURE/Effect/BombAppear.png" },						//爆発

	{ "data/TEXTURE/UI/Num.png" },									//ナンバー
};


//テクスチャ名の設定
std::vector<CTexture::SEPARATE_TEX_INFO> CTexture::m_apSeparateTexInfoList =
{
	//テクスチャのポインタ、テクスチャのパス、テクスチャのタイプ、テクスチャの分割数、テクスチャのUVサイズ
	//ポインタとUVサイズに関してはLoad関数で読み込み＆計算する
	{ nullptr, CTexture::SEPARATE_TEX_NONE,					D3DXVECTOR2(1,1),	ZeroVector2 },		//白紙
	{ nullptr, CTexture::SEPARATE_TEX_EFFECT_EXPLOSION00,	D3DXVECTOR2(4,2),	ZeroVector2 },		//爆発
	{ nullptr, CTexture::SEPARATE_TEX_IMPACT_00,			D3DXVECTOR2(5,2),	ZeroVector2 },		//インパクト
	{ nullptr, CTexture::SEPARATE_TEX_IMPACT_01,			D3DXVECTOR2(5,3),	ZeroVector2 },		//インパクト
	{ nullptr, CTexture::SEPARATE_TEX_BOMB_APPEAR,			D3DXVECTOR2(7,1),	ZeroVector2 },		//爆発

	{ nullptr, CTexture::SEPARATE_TEX_UI_NUMBER,			D3DXVECTOR2(10,1),	ZeroVector2 },		//ナンバー

};

//-----------------------------------------------------------------------------
//テクスチャ一括ロード
//-----------------------------------------------------------------------------
void CTexture::TexLoad(HWND hwnd)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	LPDIRECT3DTEXTURE9 pTexture;

	//テクスチャ名のリストのサイズ分
	for (size_t nCnt = 0; nCnt < m_aTexFileName.size(); nCnt++)
	{
		//null
		pTexture = nullptr;

		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			m_aTexFileName[nCnt].data(),
			&pTexture);

		//debug
		std::cout << "Tex Load - " << nCnt << m_aTexFileName[nCnt].data() << NEWLINE;

		//nullcheck
		if (!pTexture)
		{
			//debug
			std::cout << "LOADFAILED!!! Tex - " << nCnt << m_aTexFileName[nCnt].data() << NEWLINE;

			//失敗
			MessageBox(hwnd, "テクスチャ読み込み失敗", m_aTexFileName[nCnt].data(), MB_OK | MB_ICONHAND);
		}

		//リストに追加
		m_apTextureList.emplace_back(pTexture);
	}

	//テクスチャ名のリストのサイズ分
	for (size_t nCnt = 0; nCnt < m_apSeparateTexInfoList.size(); nCnt++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			m_aSeparateTexFileNameList[nCnt].data(),
			&m_apSeparateTexInfoList[nCnt].m_apSeparateTexture);

		//debug
		std::cout << "SeparateTex Load - " << nCnt << m_aSeparateTexFileNameList[nCnt].data() << NEWLINE;

		//nullcheck
		if (!m_apSeparateTexInfoList[nCnt].m_apSeparateTexture)
		{
			//debug
			std::cout << "LOADFAILED!!! SeparateTex - " << nCnt << m_aSeparateTexFileNameList[nCnt].data() << NEWLINE;

			//失敗
			MessageBox(hwnd, "テクスチャ読み込み失敗", m_aSeparateTexFileNameList[nCnt].data(), MB_OK | MB_ICONHAND);
		}

		//どちらかの値が0だった場合は計算しない
		//0除算防止
		if (m_apSeparateTexInfoList[nCnt].m_UVCnt.x * m_apSeparateTexInfoList[nCnt].m_UVCnt.y)
		{
			//UVサイズ計算
			m_apSeparateTexInfoList[nCnt].m_UVSize = D3DXVECTOR2(1.0f / m_apSeparateTexInfoList[nCnt].m_UVCnt.x, 1.0f / m_apSeparateTexInfoList[nCnt].m_UVCnt.y);
		}
		else
		{
			m_apSeparateTexInfoList[nCnt].m_UVSize = ZeroVector2;
			//警告文
			MessageBox(hwnd, "0除算をしようとしました　値を確認してください", m_aSeparateTexFileNameList[nCnt].data(), MB_OK | MB_ICONHAND);
		}
	}
}

//-----------------------------------------------------------------------------
//テクスチャ一括破棄
//-----------------------------------------------------------------------------
void CTexture::TexUnload()
{
	//サイズ分
	for (size_t nCnt = 0; nCnt < m_apTextureList.size(); nCnt++)
	{
		//nullcheck
		if (m_apTextureList[nCnt])
		{
			//テクスチャの開放
			m_apTextureList[nCnt]->Release();
			m_apTextureList[nCnt] = nullptr;
		}
	}

	//サイズ分
	for (size_t nCnt = 0; nCnt < m_apSeparateTexInfoList.size(); nCnt++)
	{
		//nullcheck
		if (m_apSeparateTexInfoList[nCnt].m_apSeparateTexture)
		{
			//テクスチャの開放
			m_apSeparateTexInfoList[nCnt].m_apSeparateTexture->Release();
			m_apSeparateTexInfoList[nCnt].m_apSeparateTexture = nullptr;
		}
	}

	//配列の開放
	m_apTextureList.clear();
	m_apSeparateTexInfoList.clear();
}

//-----------------------------------------------------------------------------
//テクスチャ取得　通常
//----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEX_TYPE textype)
{
	//配列より小さい値か
	if (textype < (int)m_apTextureList.size())
	{
		//nullcheck
		if (m_apTextureList[textype])
		{
			return m_apTextureList[textype];
		}
	}
	return nullptr;
}

//-----------------------------------------------------------------------------
//テクスチャ取得　分割されてるやつ
//----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetSeparateTexture(SEPARATE_TEX_TYPE textype)
{
	//配列より小さい値か
	if (textype < (int)m_apSeparateTexInfoList.size())
	{
		//nullcheck
		if (m_apSeparateTexInfoList[textype].m_apSeparateTexture)
		{
			return m_apSeparateTexInfoList[textype].m_apSeparateTexture;
		}
	}
	return nullptr;
}
//-----------------------------------------------------------------------------
//テクスチャのUVサイズ取得
//----------------------------------------------------------------------------
D3DXVECTOR2 CTexture::GetSparateTex_UVSize(SEPARATE_TEX_TYPE textype)
{
	//配列より小さい値か
	//配列外のモノを指定されていた場合はアクセス違反なるので注意
	if (textype < (int)m_apSeparateTexInfoList.size())
	{
		return m_apSeparateTexInfoList[textype].m_UVSize;
	}
	return ZeroVector2;
}

//-----------------------------------------------------------------------------
//テクスチャのUV枚数取得
//----------------------------------------------------------------------------
D3DXVECTOR2 CTexture::GetSparateTex_UVCnt(SEPARATE_TEX_TYPE textype)
{
	//配列より小さい値か
	//配列外のモノを指定されていた場合はアクセス違反なるので注意
	if (textype < (int)m_apSeparateTexInfoList.size())
	{
		return m_apSeparateTexInfoList[textype].m_UVCnt;
	}
	return ZeroVector2;
}
