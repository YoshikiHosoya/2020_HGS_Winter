//------------------------------------------------------------------------------
//
//道  [way.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "way.h"
#include "manager.h"
#include "game.h"
#include "game_2D.h"
#include "renderer.h"
#include "sound.h"
#include "particle.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CWay::CWay()
{

}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CWay::~CWay()
{

}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CWay::Init()
{
	CScene2D::Init();

	//成功
	return S_OK;
}
//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CWay::Uninit()
{
	CScene2D::Uninit();
}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CWay::Update()
{
	CScene2D::Update();

	CGame_2D *pGame2D = (CGame_2D*)CManager::GetGame();

	//通常時以外は終わる
	if (pGame2D->GetGamestate() != CGame::STATE_NORMAL)
	{
		return;
	}

	switch (pGame2D->GetDirection())
	{
	case DIRECTION::UP:
		CScene2D::GetPos().y += pGame2D->GetSpeed();

		break;
	case DIRECTION::LEFT:
		CScene2D::GetPos().x += 100.0f;

		break;
	case DIRECTION::RIGHT:
		CScene2D::GetPos().x -= 100.0f;

		break;
	default:
		break;
	}
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CWay::Draw()
{
	CScene2D::Draw();


}

//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CWay::ShowDebugInfo()
{
#ifdef _DEBUG
	//プレイヤー情報情報
	if (ImGui::TreeNode("Way"))
	{

		ImGui::TreePop();
	}
#endif //DEBUG
}


//------------------------------------------------------------------------------
//生成処理
//------------------------------------------------------------------------------
std::shared_ptr<CWay> CWay::Create(D3DXVECTOR3 pos, WAY_TYPE waytype)
{
	//変数宣言
	std::shared_ptr<CWay> pWay = std::make_shared<CWay>();


	if (pWay)
	{
		pWay->SetPos(pos);
		pWay->SetSize(D3DXVECTOR3(400.0f, 400.0f, 0.0f));
		//初期化
		pWay->Init();

		pWay->m_waytype = waytype;

		pWay->BindTexture(CTexture::GetTexture(((CTexture::TEX_TYPE)(int)(CTexture::TEX_WAY_UP + (CTexture::TEX_TYPE)(pWay->m_waytype)))));

		pWay->SetObjType(OBJTYPE_WAY);


		pWay->AddSharedList(pWay);

		return pWay;
	}

	//生成した情報
	return nullptr;
}

//------------------------------------------------------------------------------
//当たり判定処理
//------------------------------------------------------------------------------
bool CWay::Collision(D3DXVECTOR3 pos)
{
	// X Yの範囲
	if (this->GetPos().y + this->GetSize().y * 0.5f >=	pos.y &&
		this->GetPos().y - this->GetSize().y * 0.5f <=	pos.y)
	{
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
//当たり判定処理
//------------------------------------------------------------------------------
bool CWay::CollisionPlayerHit(D3DXVECTOR3 pos)
{
	switch (m_waytype)
	{
	case CWay::UP:
		// X Yの範囲
		if (this->GetPos().x + this->GetSize().x * 0.5f <= pos.x ||
			this->GetPos().x - this->GetSize().x * 0.5f >= pos.x)
		{
			return true;
		}
		break;
	case CWay::RIGHT_01:
	case CWay::LEFT_02:
		if(this->GetPos().x - this->GetSize().x * 0.5f >= pos.x)
		{
			return true;
		}
		break;
	case CWay::LEFT_01:
	case CWay::RIGHT_02:
		if (this->GetPos().x + this->GetSize().x * 0.5f <= pos.x)
		{
			return true;
		}
		break;
	}

	return false;
}

//------------------------------------------------------------------------------
//当たり判定　タイマー追加
//------------------------------------------------------------------------------
bool CWay::CollisionPlayerAddTimer(D3DXVECTOR3 pos)
{
	float fDistance = 400.0f;

	fDistance = fabsf(this->GetPos().y - pos.y);

	CGame_2D *pGame2D = (CGame_2D*)CManager::GetGame();

	//音再生
	CManager::GetSound()->Play(CSound::LABEL_SE_ADDTIME);


	if (fDistance >= 110)
	{
		pGame2D->AddTimer(2);
		printf("AddTimer 2\n");
		//音再生
		CManager::GetSound()->Play(CSound::LABEL_SE_PERFECT);

		CParticle::CreateFromText(pos, ZeroVector3, CParticleParam::EFFECT_CROSS, TAG::NONE,0,CHossoLibrary::RandomColor());
		CParticle::CreateFromText(pos, ZeroVector3, CParticleParam::EFFECT_CIRCLE, TAG::NONE, 0, CHossoLibrary::RandomColor());
		CParticle::CreateFromText(pos, ZeroVector3, CParticleParam::EFFECT_STAR, TAG::NONE, 0, CHossoLibrary::RandomColor());
		CParticle::CreateFromText(pos, ZeroVector3, CParticleParam::EFFECT_SQUARE, TAG::NONE, 0, CHossoLibrary::RandomColor());
		CParticle::CreateFromText(pos, ZeroVector3, CParticleParam::EFFECT_TRIANGLE, TAG::NONE, 0, CHossoLibrary::RandomColor());

		return true;
	}
	else if (fDistance >= 80)
	{
		pGame2D->AddTimer(1);
		printf("AddTimer 1\n");
		//音再生
		CManager::GetSound()->Play(CSound::LABEL_SE_GREAT);

		return true;

	}
	else
	{
		//音再生
		CManager::GetSound()->Play(CSound::LABEL_SE_GOOD);

	}
	return false;

}
