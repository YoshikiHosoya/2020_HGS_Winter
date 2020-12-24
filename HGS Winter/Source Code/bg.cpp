//------------------------------------------------------------------------------
//
//背景の処理  [bg.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "bg.h"
#include "scene2D.h"
#include "game.h"
#include "game_2D.h"
#include "manager.h"
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define FIGURE_SIZE_SMALL		(D3DXVECTOR3(20.0f, 20.0f, 0.0f))
#define FIGURE_SIZE_MEDIUM		(D3DXVECTOR3(50.0f, 50.0f, 0.0f))
#define FIGURE_SIZE_BIG			(D3DXVECTOR3(100.0f, 100.0f, 0.0f))

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CBg::CBg()
{
	m_apScene2D.clear();
	m_pGridLine.reset();
	m_FigureSize = ZeroVector3;
	m_FigureMove = ZeroVector3;
	m_FigureSizeType = BG_FIGURE_SIZE::NONE;
	m_FigureType = BG_FIGURE::BG_NONE;
	m_nCntScroll = 0;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CBg::~CBg()
{
}

//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CBg::Init()
{
	if (CManager::GetMode() == CManager::MODE_2DGAME)
	{
		m_pGridLine = CScene2D::Create_Shared(SCREEN_CENTER_POS, SCREEN_SIZE, CScene::OBJTYPE_BACK);
		m_pGridLine->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_BG_GRIDLINE));

	}

	return S_OK;
}

//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CBg::Uninit()
{

}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CBg::Update()
{

	if (CManager::GetMode() == CManager::MODE_2DGAME)
	{
		if (CManager::GetGame()->GetGamestate() != CGame::STATE::STATE_NORMAL)
		{
			return;
		}
	}

	D3DXVECTOR3 move = ZeroVector3;
	// カウントアップ
	m_nCntScroll++;

	// 図形のランダム生成
	RandomFigure();

	for (int nCnt = 0; nCnt < (int)m_apScene2D.size(); nCnt++)
	{
		if (m_apScene2D[nCnt])
		{
			if (CManager::GetMode() == CManager::MODE_2DGAME)
			{
				CGame_2D *pGame2D = (CGame_2D*)CManager::GetGame();
				move.y = (float)pGame2D->GetSpeed();

				m_apScene2D[nCnt]->GetPos().y += move.y * ((float)m_apScene2D[nCnt]->GetSize().y / 50);
			}
			else
			{
				m_FigureMove.y = (float)m_apScene2D[nCnt]->GetSize().y / 20;

				m_apScene2D[nCnt]->GetPos().y += m_FigureMove.y;
			}

			if ((int)m_apScene2D[nCnt]->GetSize().y == 20)
			{
				m_apScene2D[nCnt]->GetRot().z += 0.05f;
			}
			else if ((int)m_apScene2D[nCnt]->GetSize().y == 50)
			{
				m_apScene2D[nCnt]->GetRot().z += 0.03f;
			}
			else if ((int)m_apScene2D[nCnt]->GetSize().y == 100)
			{
				m_apScene2D[nCnt]->GetRot().z += 0.015f;
			}

			// 範囲外
			if (m_apScene2D[nCnt]->GetPos().y >= 2000.0f)
			{
				m_apScene2D[nCnt]->Release();
				m_apScene2D[nCnt] = nullptr;
				m_apScene2D.erase(m_apScene2D.begin() + nCnt);
			}
		}
	}
	//m_nCntScroll
	if (m_pGridLine)
	{
		m_pGridLine->SetAnimation(D3DXVECTOR2(1.0f, 1.0f - ((float)m_nCntScroll / 50)), D3DXVECTOR2(1.0f, 1.0f));
	}
}

//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CBg::Draw()
{

}

//------------------------------------------------------------------------------
//背景の生成処理
//------------------------------------------------------------------------------
std::unique_ptr<CBg> CBg::Create(int DropNum)
{
	//メモリ確保
	std::unique_ptr<CBg> pBg(new CBg);

	pBg->Init();
	pBg->m_DropRangeNum = DropNum;
	pBg->SetObjType(OBJTYPE::OBJTYPE_BACK);
	pBg->AddUniqueList(std::move(pBg));

	//リターン
	return pBg;
}

//------------------------------------------------------------------------------
//ランダム図形の生成
//------------------------------------------------------------------------------
void CBg::RandomFigure()
{
	int nCntTime = 0;
	nCntTime = rand() % 100 + 1;

	// ランダムでサイズを決める
	m_FigureSizeType	= (BG_FIGURE_SIZE)(rand() % (int)BG_FIGURE_SIZE::FIGURE_SIZE_MAX);
	m_FigureType		= (BG_FIGURE)(rand() % (int)BG_FIGURE::BG_MAX);

	switch (m_FigureSizeType)
	{
	case CBg::BG_FIGURE_SIZE::SMALL:
		m_FigureSize = FIGURE_SIZE_SMALL;
		break;

	case CBg::BG_FIGURE_SIZE::MEDIUM:
		m_FigureSize = FIGURE_SIZE_MEDIUM;
		break;

	case CBg::BG_FIGURE_SIZE::BIG:
		m_FigureSize = FIGURE_SIZE_BIG;
		break;
	}

	// 図形の生成
	if (nCntTime > m_DropRangeNum)
	{
		m_apScene2D.emplace_back(CScene2D::Create_Shared(D3DXVECTOR3((float)(rand() % 1280), -100.0f, 0.0f), m_FigureSize, CScene::OBJTYPE_BACK));
		m_apScene2D[m_apScene2D.size() - 1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		switch (m_FigureType)
		{
		case CBg::BG_FIGURE::BG_Circle:
			m_apScene2D[m_apScene2D.size() - 1]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_BG_CIRCLE));
			break;

		case CBg::BG_FIGURE::BG_Triangle:
			m_apScene2D[m_apScene2D.size() - 1]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_BG_TRIANGLE));
			break;

		case CBg::BG_FIGURE::BG_Square:
			m_apScene2D[m_apScene2D.size() - 1]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_BG_SQUARE));
			break;

		case CBg::BG_FIGURE::BG_Star:
			m_apScene2D[m_apScene2D.size() - 1]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_BG_STAR));
			break;

		case CBg::BG_FIGURE::BG_Cross:
			m_apScene2D[m_apScene2D.size() - 1]->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_BG_CROSS));
			break;
		}
	}
}