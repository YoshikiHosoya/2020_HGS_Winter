//------------------------------------------------------------------------------
//
//ゲーム処理  [game.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "game_2D.h"
#include "manager.h"
#include "renderer.h"
#include "player_2D.h"
#include "fade.h"
#include "keyboard.h"
#include "camera.h"
#include "Way.h"
#include "ParticleManager.h"
#include "multinumber.h"
#include "bg.h"
#include "sound.h"
#include "particle.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define DEFAULT_CREATE_POS (2000.0f)
#define WAY_SIZE (400.0f)
#define BENDING_TIME (4)
#define DEFAULT_TIME (20)
#define COUNTDOWN	(3)
//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CGame_2D::CGame_2D()
{
	m_nSpeed = 15;
	m_direction = DIRECTION::UP;
	m_nCnt = 0;
	m_nScoreDistance = 0;
	SetScore(0);
	m_bBendingFlag = false;
	m_nTime = DEFAULT_TIME;
	m_pWayList = {};
	m_nBendingCountDown = COUNTDOWN;
}
//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CGame_2D::~CGame_2D()
{

}

//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CGame_2D::Init(HWND hWnd)
{
	//カメラ座標設定
	CManager::GetRenderer()->GetCamera()->SetState(CCamera::CAMERA_DEBUG);

	CGame::SetPlayerPtr(CPlayer_2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 650.0f, 0.0f)));

	//パーティクルのマネージャ
	CParticleManager::Create();

	// 背景の生成
	CBg::Create(65);

	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		m_pWayList.emplace_back(CWay::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, -nCnt * 400.0f + DEFAULT_CREATE_POS, 0.0f), CWay::UP));
	}

	m_pScore = CScene2D::Create_Shared(D3DXVECTOR3(950.0f, 100.0f, 0.0f), D3DXVECTOR3(250.0f, 100.0f, 0.0f), CScene::OBJTYPE_UI);
	m_pScore->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_RANKING_SCORE));


	m_pScoreNumber = CMultiNumber::Create(D3DXVECTOR3(1100.0f, 100.0f, 0.0f), D3DXVECTOR3(50.0f, 75.0f, 0.0f),0,7, CScene::OBJTYPE_UI);

	m_pTimeNumber = CMultiNumber::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 50.0f, 0.0f), D3DXVECTOR3(50.0f, 75.0f, 0.0f), m_nTime, 2, CScene::OBJTYPE_UI);

	m_pNextBending = CScene2D::Create_Shared(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 300.0f, 0.0f), D3DXVECTOR3(150.0f, 150.0f, 0.0f), CScene::OBJTYPE_UI);
	m_pNextBending->SetDisp(false);

	m_pReadyGo = CScene2D::Create_Shared(SCREEN_CENTER_POS, D3DXVECTOR3(500.0f, 200.0f, 0.0f), CScene::OBJTYPE_UI);
	m_pReadyGo->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_GAME_LADY));

	//最初のカーブ
	Bending();

	//ゲームステート初期化
	SetGamestate(CGame::STATE_READY);

	//音再生
	CManager::GetSound()->Play(CSound::LABEL_SE_READY);


	return S_OK;
}

//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CGame_2D::Uninit()
{
	//終了処理
	CScene::ReleaseAll();
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CGame_2D::Update()
{
	if (GetGamestate() == CGame::STATE_READY)
	{
		m_nCnt++;

		if (m_nCnt >= 180)
		{
			//テクスチャ差し替え
			m_pReadyGo->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_GAME_GO));

			//ゲームステート通常
			SetGamestate(CGame::STATE_NORMAL);

			CParticle::CreateFromText(SCREEN_CENTER_POS + CHossoLibrary::RandomVector3(500.0f), ZeroVector3, CParticleParam::EFFECT_CROSS, TAG::NONE, 0, CHossoLibrary::RandomColor());
			CParticle::CreateFromText(SCREEN_CENTER_POS + CHossoLibrary::RandomVector3(500.0f), ZeroVector3, CParticleParam::EFFECT_CIRCLE, TAG::NONE, 0, CHossoLibrary::RandomColor());
			CParticle::CreateFromText(SCREEN_CENTER_POS + CHossoLibrary::RandomVector3(500.0f), ZeroVector3, CParticleParam::EFFECT_STAR, TAG::NONE, 0, CHossoLibrary::RandomColor());
			CParticle::CreateFromText(SCREEN_CENTER_POS + CHossoLibrary::RandomVector3(500.0f), ZeroVector3, CParticleParam::EFFECT_SQUARE, TAG::NONE, 0, CHossoLibrary::RandomColor());
			CParticle::CreateFromText(SCREEN_CENTER_POS + CHossoLibrary::RandomVector3(500.0f), ZeroVector3, CParticleParam::EFFECT_TRIANGLE, TAG::NONE, 0, CHossoLibrary::RandomColor());
			CParticle::CreateFromText(SCREEN_CENTER_POS + CHossoLibrary::RandomVector3(500.0f), ZeroVector3, CParticleParam::EFFECT_CROSS, TAG::NONE, 0, CHossoLibrary::RandomColor());
			CParticle::CreateFromText(SCREEN_CENTER_POS + CHossoLibrary::RandomVector3(500.0f), ZeroVector3, CParticleParam::EFFECT_CIRCLE, TAG::NONE, 0, CHossoLibrary::RandomColor());
			CParticle::CreateFromText(SCREEN_CENTER_POS + CHossoLibrary::RandomVector3(500.0f), ZeroVector3, CParticleParam::EFFECT_STAR, TAG::NONE, 0, CHossoLibrary::RandomColor());
			CParticle::CreateFromText(SCREEN_CENTER_POS + CHossoLibrary::RandomVector3(500.0f), ZeroVector3, CParticleParam::EFFECT_SQUARE, TAG::NONE, 0, CHossoLibrary::RandomColor());
			CParticle::CreateFromText(SCREEN_CENTER_POS + CHossoLibrary::RandomVector3(500.0f), ZeroVector3, CParticleParam::EFFECT_TRIANGLE, TAG::NONE, 0, CHossoLibrary::RandomColor());
			CParticle::CreateFromText(SCREEN_CENTER_POS + CHossoLibrary::RandomVector3(500.0f), ZeroVector3, CParticleParam::EFFECT_CROSS, TAG::NONE, 0, CHossoLibrary::RandomColor());
			CParticle::CreateFromText(SCREEN_CENTER_POS + CHossoLibrary::RandomVector3(500.0f), ZeroVector3, CParticleParam::EFFECT_CIRCLE, TAG::NONE, 0, CHossoLibrary::RandomColor());
			CParticle::CreateFromText(SCREEN_CENTER_POS + CHossoLibrary::RandomVector3(500.0f), ZeroVector3, CParticleParam::EFFECT_STAR, TAG::NONE, 0, CHossoLibrary::RandomColor());
			CParticle::CreateFromText(SCREEN_CENTER_POS + CHossoLibrary::RandomVector3(500.0f), ZeroVector3, CParticleParam::EFFECT_SQUARE, TAG::NONE, 0, CHossoLibrary::RandomColor());
			CParticle::CreateFromText(SCREEN_CENTER_POS + CHossoLibrary::RandomVector3(500.0f), ZeroVector3, CParticleParam::EFFECT_TRIANGLE, TAG::NONE, 0, CHossoLibrary::RandomColor());


		}
	}

	else if (GetGamestate() == CGame::STATE_NORMAL)
	{
		if (m_pReadyGo)
		{
			m_pReadyGo->SetColor(m_pReadyGo->GetColor() -= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.02f));
			if (m_pReadyGo->GetColor().a <= 0)
			{
				m_pReadyGo->Release();
				m_pReadyGo.reset();
			}
		}

		m_nCnt++;

		if (m_nCnt % 60 == 0)
		{
			AddTimer(-1);

			if (m_nTime <= 0)
			{
				SetGamestate(CGame::STATE_GAMEOVER);
				//音再生
				CManager::GetSound()->Play(CSound::LABEL_SE_TIMEUP);
			}
		}

		m_nScoreDistance += m_nSpeed;
		m_pScoreNumber->SetMultiNumber(m_nScoreDistance);

		m_nBendingTime--;

		if (m_nBendingTime < 0)
		{
			m_direction = DIRECTION::UP;
		}

		//次の曲がり角までの差分
		float fNextBendingDistance = m_fNextBendingPoint - m_nScoreDistance;
		CDebugProc::Print(CDebugProc::PLACE_LEFT, "fNextBendingDistance >> %.2f\n", fNextBendingDistance + DEFAULT_CREATE_POS);

		float fPlayerToBendingWayDistance = 99999999.9f;

		if (m_pNextBendingWayPos)
		{
			fPlayerToBendingWayDistance = m_pNextBendingWayPos->y - GetPlayer()->GetPlayerPos().y;

			CDebugProc::Print(CDebugProc::PLACE_LEFT, "m_pNextBendingWayPos >>>> %.2f\n", m_pNextBendingWayPos->y);

			CDebugProc::Print(CDebugProc::PLACE_LEFT, "fPlayerToBendingWayDistance >>>>>> %.2f\n", fPlayerToBendingWayDistance);

			if (fabsf(fPlayerToBendingWayDistance) + 150 < 400 * m_nBendingCountDown)
			{
				m_pNextBending->SetDisp(true);
				m_nBendingCountDown--;

				printf("CountDown %d\n", m_nBendingCountDown);
				printf("fNextBendingDistance %.2f\n", fNextBendingDistance + DEFAULT_CREATE_POS + 170);

				CParticle::CreateFromText(m_pNextBending->GetPos(), ZeroVector3, CParticleParam::EFFECT_COUNTDOWN);

				//音再生
				CManager::GetSound()->Play(CSound::LABEL_SE_COUNTDOWN);
			}
		}

		if (m_pWayList[m_pWayList.size() - 1]->GetPos().y >= -600.0f)
		{
			if (fNextBendingDistance <= DEFAULT_CREATE_POS && m_bBendingFlag)
			{
				switch (m_NextBendingDirection)
				{
				case DIRECTION::LEFT:
					m_pWayList.emplace_back(CWay::Create(m_pWayList[m_pWayList.size() - 1]->GetPos() + D3DXVECTOR3(0.0f, -WAY_SIZE, 0.0f), CWay::LEFT_01));
					m_pWayList.emplace_back(CWay::Create(m_pWayList[m_pWayList.size() - 1]->GetPos() + D3DXVECTOR3(-WAY_SIZE, 0.0f, 0.0f), CWay::LEFT_02));
					m_pNextBendingWayPos = m_pWayList[m_pWayList.size() - 1]->GetPosPtr();

					m_bBendingFlag = false;
					break;

				case DIRECTION::RIGHT:
					m_pWayList.emplace_back(CWay::Create(m_pWayList[m_pWayList.size() - 1]->GetPos() + D3DXVECTOR3(0.0f, -WAY_SIZE, 0.0f), CWay::RIGHT_01));
					m_pWayList.emplace_back(CWay::Create(m_pWayList[m_pWayList.size() - 1]->GetPos() + D3DXVECTOR3(WAY_SIZE, 0.0f, 0.0f), CWay::RIGHT_02));
					m_pNextBendingWayPos = m_pWayList[m_pWayList.size() - 1]->GetPosPtr();

					m_bBendingFlag = false;
					break;

				default:
					break;
				}

			}
			else
			{
				m_pWayList.emplace_back(CWay::Create(m_pWayList[m_pWayList.size() - 1]->GetPos() + D3DXVECTOR3(0.0f, -WAY_SIZE, 0.0f), CWay::UP));
			}
		}

		for (size_t nCnt = 0; nCnt < m_pWayList.size(); nCnt++)
		{
			if (m_pWayList[nCnt]->GetPos().y >= 1200.0f)
			{
				m_pWayList[nCnt]->Release();
				m_pWayList.erase(m_pWayList.begin() + nCnt);
			}
		}

	}
	else if (GetGamestate() == CGame::STATE_GAMEOVER)
	{
		//カウントダウン
		m_nCnt--;

		//カウント0
		if (m_nCnt <= 0)
		{
			//ゲーム終了
			GameEnd();
		}

	}

}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CGame_2D::Draw()
{

}
//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CGame_2D::ShowDebugInfo()
{
#ifdef _DEBUG
	//キーボードのポインタ
	CKeyboard *pKeyboard = CManager::GetKeyboard();


	CDebugProc::Print(CDebugProc::PLACE_LEFT, "m_nScoreDistance >> %d\n", m_nScoreDistance);
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "m_fNextBendingPoint >> %.2f\n", m_fNextBendingPoint);
	CDebugProc::Print(CDebugProc::PLACE_LEFT, "NextDirection >> %d\n", m_NextBendingDirection);


#endif //_DEBUG
}

//------------------------------------------------------------------------------
//プレイヤーの曲がる処理
//------------------------------------------------------------------------------
void CGame_2D::PlayerBending(DIRECTION Direction)
{
	m_direction = Direction;
	m_nBendingTime = BENDING_TIME;;

	Bending();
}

//------------------------------------------------------------------------------
//ゲーム終了
//------------------------------------------------------------------------------
void CGame_2D::GameEnd()
{
	CGame::SetScore(m_nScoreDistance);
	CManager::GetRenderer()->GetFade()->SetModeFade(CManager::MODE_RANKING);
	SetGamestate(CGame::STATE_NONE);

}
//------------------------------------------------------------------------------
//タイマー
//------------------------------------------------------------------------------
void CGame_2D::AddTimer(int nAddTime)
{
	m_nTime += nAddTime;
	m_pTimeNumber->SetMultiNumber(m_nTime);

}

//------------------------------------------------------------------------------
//曲がる処理
//------------------------------------------------------------------------------
void CGame_2D::Bending()
{
	m_NextBendingDirection = (DIRECTION)(rand() % 2);
	m_fNextBendingPoint = (float)(m_nScoreDistance + (m_nSpeed * 60) + rand() % (m_nSpeed * 90));
	//m_fNextBendingPoint = m_nScoreDistance + (m_nSpeed * 60);
	//m_fNextBendingPoint = m_nScoreDistance + 2500.0f;
	m_pNextBendingWayPos = nullptr;
	m_nBendingCountDown = COUNTDOWN;
	m_pNextBending->SetDisp(false);


	if (m_NextBendingDirection == DIRECTION::LEFT)
	{
		m_pNextBending->BindTexture(CTexture::GetTexture(CTexture::TEX_ARROW_LEFT));
	}
	if (m_NextBendingDirection == DIRECTION::RIGHT)
	{
		m_pNextBending->BindTexture(CTexture::GetTexture(CTexture::TEX_ARROW_RIGHT));
	}

	m_nSpeed += 3;



	m_bBendingFlag = true;

}

//------------------------------------------------------------------------------
//ゲームステート設定
//------------------------------------------------------------------------------
void CGame_2D::SetGamestate(STATE gamestate)
{
	if (GetGamestate() != gamestate)
	{
		CGame::SetGamestate(gamestate);
		//ステートが進んでいる場合

		if (gamestate == CGame::STATE_GAMEOVER)
		{
			m_nCnt = 60;
		}
	}
}
