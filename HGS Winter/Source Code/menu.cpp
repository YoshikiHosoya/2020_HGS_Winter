//------------------------------------------------------------------------------
//
//メニュー処理  [Menu.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "menu.h"
#include "scene2D.h"
#include "manager.h"
#include "sound.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CMenu::m_pTexture = nullptr;

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CMenu::CMenu()
{
	m_apScene2DList = {};

	m_nChoiseNum = 0;
	m_nSelectNum = 0;
	m_type = CMenu::MENU_VERTICAL;
}
//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CMenu::~CMenu()
{
	//配列の開放
	m_apScene2DList.clear();
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CMenu::Init()
{
	return S_OK;
}
//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CMenu::Uninit()
{

}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CMenu::Update()
{
	//nullcheck
	if (!m_apScene2DList.empty())
	{
		for (size_t nCnt = 0; nCnt < m_apScene2DList.size(); nCnt++)
		{
			//カーソルの合ったものは明るく
			if (nCnt == m_nSelectNum)
			{
				m_apScene2DList[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			//それ以外の項目は暗く
			else
			{
				m_apScene2DList[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
			}

			//更新処理
			m_apScene2DList[nCnt]->Update();
		}
	}
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CMenu::Draw()
{
	//nullcheck
	if (!m_apScene2DList.empty())
	{
		for (size_t nCnt = 0; nCnt < m_apScene2DList.size(); nCnt++)
		{
			//描画処理
			m_apScene2DList[nCnt]->Draw();
		}
	}
}
//------------------------------------------------------------------------------
//デバッグ情報表示
//------------------------------------------------------------------------------
void CMenu::ShowDebugInfo()
{
#ifdef _DEBUG

#endif // _DEBUG

}
//------------------------------------------------------------------------------
//生成
//------------------------------------------------------------------------------
std::unique_ptr<CMenu> CMenu::Create(D3DXVECTOR3 pos, D3DXVECTOR3 onesize, LPDIRECT3DTEXTURE9 pTex, D3DXVECTOR2 UVCnt, CMenu::TYPE type)
{
	//メモリ確保
	std::unique_ptr<CMenu> pMenu(new CMenu);

	//初期化
	pMenu->Init();

	//選択肢の数
	pMenu->m_nChoiseNum = (int)UVCnt.y;
	pMenu->m_type = type;

	//UV設定
	D3DXVECTOR2 UVSize = D3DXVECTOR2(1 / UVCnt.x, 1 / UVCnt.y);

	//生成
	for (int nCnt = 0; nCnt < pMenu->m_nChoiseNum; nCnt++)
	{
		switch (type)
		{
			//縦のメニュー
		case CMenu::MENU_VERTICAL:
			pMenu->m_apScene2DList.emplace_back(CScene2D::Create_SelfManagement(pos + D3DXVECTOR3(0.0f, -pMenu->m_nChoiseNum * onesize.y * 0.5f + onesize.y * nCnt, 0.0f), onesize));
			break;

			//横のメニュー
		case CMenu::MENU_HORIZON:
			pMenu->m_apScene2DList.emplace_back(CScene2D::Create_SelfManagement(pos + D3DXVECTOR3(-onesize.x * 0.5f + onesize.x * nCnt, 0.0f, 0.0f), onesize));
			break;
		}
		//テクスチャセット
		pMenu->m_apScene2DList[nCnt]->BindTexture(pTex);
		pMenu->m_apScene2DList[nCnt]->SetAnimation(D3DXVECTOR2(0.0f, UVSize.y * nCnt), UVSize);
	}


	//生成した情報
	return pMenu;
}
//------------------------------------------------------------------------------
//番号取得
//------------------------------------------------------------------------------
void CMenu::MenuSelect()
{
	switch (m_type)
	{
		//縦メニュー
	case CMenu::MENU_VERTICAL:
		CHossoLibrary::SelectVerticalMenu(m_nSelectNum, m_nChoiseNum);
		break;

		//横メニュー
	case CMenu::MENU_HORIZON:
		CHossoLibrary::SelectHorizonMenu(m_nSelectNum, m_nChoiseNum);

		break;
	}
}
//------------------------------------------------------------------------------
//番号取得
//------------------------------------------------------------------------------
int CMenu::GetSelectNum()
{
	return m_nSelectNum;
}
