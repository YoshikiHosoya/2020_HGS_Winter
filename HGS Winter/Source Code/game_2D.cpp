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
#include "ParticleManager.h"
#include "multinumber.h"
#include "bg.h"
#include "sound.h"
#include "particle.h"
#include "enemy_2D.h"
#include "bomb.h"
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

	//m_pScore = CScene2D::Create_Shared(D3DXVECTOR3(950.0f, 100.0f, 0.0f), D3DXVECTOR3(250.0f, 100.0f, 0.0f), CScene::OBJTYPE_UI);
	//m_pScore->BindTexture(CTexture::GetTexture(CTexture::TEX_UI_RANKING_SCORE));


	//m_pScoreNumber = CMultiNumber::Create(D3DXVECTOR3(1100.0f, 100.0f, 0.0f), D3DXVECTOR3(50.0f, 75.0f, 0.0f),0,7, CScene::OBJTYPE_UI);

	//m_pTimeNumber = CMultiNumber::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 50.0f, 0.0f), D3DXVECTOR3(50.0f, 75.0f, 0.0f), m_nTime, 2, CScene::OBJTYPE_UI);

	//ゲームステート初期化
	SetGamestate(CGame::STATE_READY);

	//音再生
	CManager::GetSound()->Play(CSound::LABEL_SE_READY);

	CEnemy_2D::Create(D3DXVECTOR3(100.0f, 100.0f, 0.0f), CEnemy_2D::BLUE);

	return S_OK;
}

//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CGame_2D::Update()
{
	EnemySpawn();

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
//敵の出現
//------------------------------------------------------------------------------
void CGame_2D::EnemySpawn()
{
	m_nCnt++;

	CDebugProc::Print(CDebugProc::PLACE_LEFT, "EnemuNum >> %d\n", CEnemy_2D::GetEnemyNum());


	if (m_nCnt % 60 == 0)
	{
		CreateEnemyGroup(SCREEN_CENTER_POS + D3DXVECTOR3(CHossoLibrary::Random(500.0f), CHossoLibrary::Random(250.0f), 0.0f));

		if (m_nCnt % 120 == 0)
		{
			CBomb::Create(SCREEN_CENTER_POS + D3DXVECTOR3(CHossoLibrary::Random(500.0f), CHossoLibrary::Random(250.0f), 0.0f));
		}
	}

}

//------------------------------------------------------------------------------
//敵の集団生成
//------------------------------------------------------------------------------
void CGame_2D::CreateEnemyGroup(D3DXVECTOR3 posOrigin)
{
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		CEnemy_2D::Create(posOrigin + D3DXVECTOR3(CHossoLibrary::Random(30.0f), CHossoLibrary::Random(30.0f), 0.0f), CEnemy_2D::BLUE);
	}
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
