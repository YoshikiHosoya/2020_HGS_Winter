//------------------------------------------------------------------------------
//
//ライブラリ処理  [HossoLibrary.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "hossolibrary.h"
#include "manager.h"
#include "keyboard.h"
#include "renderer.h"
#include "light.h"
#include "mouse.h"
#include "sound.h"
#include "scene.h"
#include "scene2D.h"
#include "particle.h"
#include "Pad_XInput.h"
//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define STICK_ON	(0.8f)
#define JOY_MAX_RANGE  (32768.0f)

//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
CKeyboard *CHossoLibrary::m_pKeyboard = nullptr;
CPad_XInput *CHossoLibrary::m_pXInput = nullptr;

bool CHossoLibrary::m_bStickX = false;
bool CHossoLibrary::m_bStickY = false;
float CHossoLibrary::m_fLeftStickX = false;
float CHossoLibrary::m_fLeftStickY = false;
bool CHossoLibrary::m_WireFrame = false;
bool CHossoLibrary::m_Lighting = true;
bool CHossoLibrary::m_bDebugPlayer = false;
int CHossoLibrary::m_Culling = false;

//------------------------------------------------------------------------------
//移動用のチェック関数
//------------------------------------------------------------------------------
bool CHossoLibrary::CheckMove(DIRECTION direction)
{
	switch (direction)
	{
	case DIRECTION::UP:
		//↑
		if (m_pKeyboard->GetPress(DIK_W) || m_pKeyboard->GetPress(DIK_UP) || m_pXInput->GetPress(CPad_XInput::JOYPADKEY_UP))
		{
			return true;
		}
		break;
	case DIRECTION::DOWN:
		//↓
		if (m_pKeyboard->GetPress(DIK_S) || m_pKeyboard->GetPress(DIK_DOWN) || m_pXInput->GetPress(CPad_XInput::JOYPADKEY_DOWN))
		{
			return true;
		}
		break;
	case DIRECTION::LEFT:
		//←
		if (m_pKeyboard->GetPress(DIK_A) || m_pKeyboard->GetPress(DIK_LEFT) || m_pXInput->GetPress(CPad_XInput::JOYPADKEY_LEFT))
		{
			return true;
		}
		break;
	case DIRECTION::RIGHT:

		//→
		if (m_pKeyboard->GetPress(DIK_D) || m_pKeyboard->GetPress(DIK_RIGHT) || m_pXInput->GetPress(CPad_XInput::JOYPADKEY_RIGHT))
		{
			return true;
		}
		break;
	}
	return false;
}

//------------------------------------------------------------------------------
//選択肢用チェック関数
//------------------------------------------------------------------------------
DIRECTION CHossoLibrary::CheckSelect()
{
	DIRECTION direction = DIRECTION::NONE;

	//↑
	if ((m_pKeyboard->GetTrigger(DIK_W) || m_pKeyboard->GetTrigger(DIK_UP) || m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_UP,1) || (m_fLeftStickY / JOY_MAX_RANGE > STICK_ON && m_bStickY == false)))
	{
		direction = DIRECTION::UP;
	}
	//↓
	else if ((m_pKeyboard->GetTrigger(DIK_S) || m_pKeyboard->GetTrigger(DIK_DOWN) || m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_DOWN, 1) || (m_fLeftStickY / JOY_MAX_RANGE < -STICK_ON && m_bStickY == false)))
	{
		direction = DIRECTION::DOWN;
	}
	//→
	else if ((m_pKeyboard->GetTrigger(DIK_D) || m_pKeyboard->GetTrigger(DIK_RIGHT) || m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_RIGHT, 1) || (m_fLeftStickX / JOY_MAX_RANGE > STICK_ON && m_bStickX == false)))
	{
		direction = DIRECTION::RIGHT;
	}
	//←
	else if ((m_pKeyboard->GetTrigger(DIK_A) || m_pKeyboard->GetTrigger(DIK_LEFT) || m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_LEFT, 1)  || (m_fLeftStickX / JOY_MAX_RANGE < -STICK_ON && m_bStickX == false)))
	{
		direction = DIRECTION::LEFT;
	}

	return direction;
}
//------------------------------------------------------------------------------
//Lstick入力確認
//------------------------------------------------------------------------------
bool CHossoLibrary::CheckLStick()
{
	//変数宣言
	float joypadX, joypadY;

	//ジョイパッドの移動処理
	m_pXInput->GetStickLeft(&joypadX, &joypadY);

	if (joypadX != 0 || joypadY != 0)
	{
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
//ジャンプ
//------------------------------------------------------------------------------
bool CHossoLibrary::CheckJump(INPUTTYPE type)
{
	switch (type)
	{
	case CHossoLibrary::PRESS:
		if (m_pKeyboard->GetPress(DIK_SPACE) || m_pXInput->GetPress(CPad_XInput::JOYPADKEY_A))
		{
			return true;
		}
		break;
	case CHossoLibrary::TRIGGER:
		if (m_pKeyboard->GetTrigger(DIK_SPACE) || m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_A, 1))
		{
			return true;
		}
		break;
	}
	return false;
}
//------------------------------------------------------------------------------
//決定 トリガーのみ
//------------------------------------------------------------------------------
bool CHossoLibrary::CheckDash(INPUTTYPE type)
{
	switch (type)
	{
	case CHossoLibrary::PRESS:
		if (m_pKeyboard->GetPress(DIK_LSHIFT) || m_pXInput->GetPress(CPad_XInput::JOYPADKEY_R2))
		{
			return true;
		}
		break;
	case CHossoLibrary::TRIGGER:
		if (m_pKeyboard->GetTrigger(DIK_LSHIFT) || m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_R2, 5))
		{
			return true;
		}
		break;
	}

	return false;
}
//------------------------------------------------------------------------------
//カメラリセット
//------------------------------------------------------------------------------
bool CHossoLibrary::CheckCameraReset(INPUTTYPE type)
{
	switch (type)
	{
	case CHossoLibrary::PRESS:
		if (m_pKeyboard->GetPress(DIK_V) || m_pXInput->GetPress(CPad_XInput::JOYPADKEY_L2))
		{
			return true;
		}
		break;
	case CHossoLibrary::TRIGGER:
		if (m_pKeyboard->GetTrigger(DIK_V) || m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_L2, 1))
		{
			return true;
		}
		break;
	}

	return false;
}
//------------------------------------------------------------------------------
//決定 トリガーのみ
//------------------------------------------------------------------------------
bool CHossoLibrary::CheckDecision()
{
	if (m_pKeyboard->GetTrigger(DIK_RETURN) || m_pKeyboard->GetTrigger(DIK_SPACE) ||
		m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_A, 1))
	{
		CManager::GetSound()->Play(CSound::LABEL_SE_DECISION);
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
//チュートリアルスキップ
//------------------------------------------------------------------------------
bool CHossoLibrary::CheckSkipTutorial()
{
	if (m_pKeyboard->GetTrigger(DIK_RETURN) ||
		m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_START, 1))
	{
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
//キャンセル トリガーのみ
//------------------------------------------------------------------------------
bool CHossoLibrary::CheckCancel()
{
	if (m_pKeyboard->GetTrigger(DIK_BACKSPACE) || m_pKeyboard->GetTrigger(DIK_DELETE) ||
		m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_BACK, 1) || m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_B, 1))
	{
		//CManager::GetSound()->Play(CSound::LABEL_SE_CANCEL);

		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
//ポーズ トリガーのみ
//------------------------------------------------------------------------------
bool CHossoLibrary::CheckPause()
{
	if (m_pKeyboard->GetTrigger(DIK_P) || m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_START, 1))
	{
		//CManager::GetSound()->Play(CSound::LABEL_SE_PAUSE);
		return true;

	}
	return false;
}

//------------------------------------------------------------------------------
//何かボタン押して下さい用　トリガー
//------------------------------------------------------------------------------
bool CHossoLibrary::CheckAnyButton()
{
	if (m_pKeyboard->GetTrigger(DIK_RETURN)						|| m_pKeyboard->GetTrigger(DIK_SPACE) ||
		m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_START, 1)	|| m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_BACK, 1) ||
		m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_A, 1)		|| m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_B, 1) ||
		m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_X, 1)		|| m_pXInput->GetTrigger(CPad_XInput::JOYPADKEY_Y, 1))
	{
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
//スティックの倒れてるかの情報をリセット
//------------------------------------------------------------------------------
void CHossoLibrary::ResetStickInfo()
{
	//1F前のスティック情報を元に倒れてるかチェック
	fabsf(m_fLeftStickX / JOY_MAX_RANGE) > STICK_ON ? m_bStickX = true : m_bStickX = false;
	fabsf(m_fLeftStickY / JOY_MAX_RANGE) > STICK_ON ? m_bStickY = true : m_bStickY = false;

	//今フレームのスティック情報取得
	m_pXInput->GetStickLeft(&m_fLeftStickX, &m_fLeftStickY);
}

D3DXCOLOR CHossoLibrary::RandomColor()
{
	return D3DXCOLOR(CHossoLibrary::Random(1), CHossoLibrary::Random(1), CHossoLibrary::Random(1), 1.0f);
}

void CHossoLibrary::StartVibration(int nCntVibration)
{
	m_pXInput->StartVibration(nCntVibration);
}

//------------------------------------------------------------------------------
//3D上での移動処理
//------------------------------------------------------------------------------
bool CHossoLibrary::Check3DMoveStick(D3DXVECTOR3 & Move, float const fMove, float const fCameraRot, float & fRotGoal)
{

	D3DXVECTOR3	Pad = ZeroVector3;

	//パッドの値取得
	m_pXInput->GetStickLeft(&Pad.x, &Pad.y);

	//joypadの値を出す
	Pad.x /= JOY_MAX_RANGE;
	Pad.y /= JOY_MAX_RANGE;

	float fAngle = atan2f((float)Pad.x, (float)Pad.y);				//角度
	float fLength = sqrtf(Pad.x * Pad.x + Pad.y * Pad.y);			//距離

	//一定以下の値の場合は判定しない
	if (fabsf(fLength) < 0.2f)
	{
		fLength = 0;
		return false;
	}

	//回転の計算
	fRotGoal = D3DX_PI + fAngle + fCameraRot;

	CHossoLibrary::CalcRotation(fRotGoal);

	//プレイヤーのmoveに加算
	Move.x += sinf(fAngle + fCameraRot) * fMove;
	Move.z += cosf(fAngle + fCameraRot) * fMove;


	return true;

}
//------------------------------------------------------------------------------
//Rスティックによるカメラ回転
//------------------------------------------------------------------------------
bool CHossoLibrary::Check3DCameraStick(D3DXVECTOR3 & Rot, float fHolizonMove, float fVerticalMove)
{
	D3DXVECTOR3 Pad = ZeroVector3;

	//パッドの値取得
	m_pXInput->GetStickRight(&Pad.x, &Pad.y);

	//joypadの値を出す
	Pad.x /= JOY_MAX_RANGE;
	Pad.y /= JOY_MAX_RANGE;

	float fAngle = atan2f((float)Pad.x, (float)Pad.y);				//角度
	float fLength = sqrtf(Pad.x * Pad.x + Pad.y * Pad.y);			//距離

	//一定以下の値の場合は判定しない
	if (fabsf(fLength) < 0.2f)
	{
		fLength = 0;
		return false;
	}

	//カメラの回転を加算
	Rot.y += sinf(fAngle) * (fHolizonMove * fLength);
	Rot.x += cosf(fAngle) * (fVerticalMove * fLength);

	return true;
}
//------------------------------------------------------------------------------
//2Dの外積計算
//------------------------------------------------------------------------------
float CHossoLibrary::Vec2Cross(D3DXVECTOR2 const &rVecA, D3DXVECTOR2 const &rVecB)
{
	return rVecA.x * rVecB.y - rVecB.x * rVecA.y;
}

//------------------------------------------------------------------------------
//マトリックス計算
//------------------------------------------------------------------------------
void CHossoLibrary::CalcMatrix(D3DXMATRIX *pMtx, D3DXVECTOR3 const &rPos, D3DXVECTOR3 const &rRot)
{
	D3DXMATRIX	mtxRot, mtxTrans;			//計算用

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(pMtx);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rRot.y, rRot.x, rRot.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, rPos.x, rPos.y, rPos.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxTrans);
}
//------------------------------------------------------------------------------
//マトリックス計算
//------------------------------------------------------------------------------
void CHossoLibrary::CalcShadowMatrix(D3DXMATRIX & rShadowMtx, D3DXVECTOR3 const & rPos, D3DXVECTOR3 const & rNor)
{
	D3DXPLANE planeField;
	D3DXVECTOR3 LightVec = CManager::GetRenderer()->GetLight()->GetLightDir(0);
	D3DXVECTOR4 ShadowLightVec = ZeroVector4;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&rShadowMtx);

	//影用のライトの設定
	ShadowLightVec = D3DXVECTOR4(-LightVec.x, -LightVec.y, -LightVec.z, 0);

	//影描画用のプレーン生成
	D3DXPlaneFromPointNormal(&planeField, &rPos, &rNor);

	//影のマトリックス設定
	D3DXMatrixShadow(&rShadowMtx, &ShadowLightVec, &planeField);
}

//------------------------------------------------------------------------------
//Imgui初期化
//------------------------------------------------------------------------------
 HRESULT CHossoLibrary::InitImgui(HWND hWnd)
{
	//ゲームパッドとキーボードの情報取得
	//所有権は貰ってないから開放の必要はない
	m_pXInput = CManager::GetXInput();
	m_pKeyboard = CManager::GetKeyboard();

	//NULLチェック
	if (!m_pXInput || !m_pKeyboard)
	{
		return E_FAIL;
	}

	//create
	IMGUI_CHECKVERSION();

	//生成
	ImGui::CreateContext();

	//デフォルトカラー設定
	ImGui::StyleColorsDark();

	//初期化
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(CManager::GetRenderer()->GetDevice());

	return S_OK;
}
//------------------------------------------------------------------------------
//Imgui終了
//------------------------------------------------------------------------------
void CHossoLibrary::UninitImgui()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
//------------------------------------------------------------------------------
//Imgui デバッグ情報表示
//------------------------------------------------------------------------------
void CHossoLibrary::ShowDebugInfo()
{
#ifdef _DEBUG

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//開始
	if (ImGui::CollapsingHeader("Debug"))
	{
		//FPS情報
		if (ImGui::TreeNode("FPS"))
		{
			//FPS
			ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::TreePop();
		}
		//Sceneの数
		if (ImGui::TreeNode("NumInfo"))
		{
			ImGui::Text("NumAll (%d)", CScene::GetNumAll());
			ImGui::Text("NumScene2D (%d)", CScene2D::GetNumScene2D());
			ImGui::Text("ParticleAll (%d)", COneParticle::GetNumAll());
			ImGui::TreePop();
		}

		//情報
		if (ImGui::TreeNode("DebugCommand"))
		{
			//ワイヤーフレーム
			if (ImGui::Checkbox("WireFrame", &m_WireFrame))
			{
				CheckWireMode();
			}

			//ライティング
			if (ImGui::Checkbox("Lighting", &m_Lighting))
			{
				pDevice->SetRenderState(D3DRS_LIGHTING, m_Lighting);		// ライティングモード切り替え
			}

			//カリング
			if (ImGui::TreeNode("tree1", "CULLING"))
			{
				if (ImGui::RadioButton("D3DCULL_CCW", &m_Culling, 0))
				{
					//裏面カリング
					CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_CULLING_CCW);
				}
				ImGui::SameLine();
				if (ImGui::RadioButton("D3DCULL_CW", &m_Culling, 1))
				{
					//表面カリング
					CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_CULLING_CW);
				}
				ImGui::SameLine();
				if (ImGui::RadioButton("D3DCULL_NONE", &m_Culling, 2))
				{
					//カリングしない
					CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_CULLING_NONE);
				}
				ImGui::TreePop();
			}
			//プレイヤーデバッグ
			if (ImGui::Checkbox("DebugPlayer", &m_bDebugPlayer))
			{

			}
			ImGui::TreePop();
		}
	}

#endif //DEBUG
}
//------------------------------------------------------------------------------
//ワイヤーフレームかどうか確認
//Imguiのウィンドウだけワイヤーフレームにならない為に必要
//------------------------------------------------------------------------------
void CHossoLibrary::CheckWireMode()
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//trueかfalseかで決める
	m_WireFrame ?
		CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_WIRE_ON):
		CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_WIRE_OFF);
}
//------------------------------------------------------------------------------
//カリングの状態確認と設定
//一時的に処理を変えた後何に戻すべきか確認用
//------------------------------------------------------------------------------
void CHossoLibrary::CheckCulling()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	switch (m_Culling)
	{
	case 0:
		//裏面カリング
		CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_CULLING_CCW);
		break;
	case 1:
		//表面カリング
		CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_CULLING_CW);
		break;
	case 2:
		//カリングしない
		CManager::GetRenderer()->SetRendererCommand(CRenderer::RENDERER_CULLING_NONE);
		break;
	default:
		break;
	}
}
//------------------------------------------------------------------------------
//カリングの状態確認と設定
//一時的にライティングを切った後に戻すべきか確認用
//------------------------------------------------------------------------------
void CHossoLibrary::CheckLighting()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, m_Lighting);		// ライティングモード切り替え
}

//------------------------------------------------------------------------------
//回転の計算　360度以内にする
//------------------------------------------------------------------------------
void CHossoLibrary::CalcRotation(float &fRot)
{
	//差分が3.14を以上の時
	if (fRot > D3DX_PI)
	{
		//6.28引く
		fRot -= D3DX_PI * 2;
	}
	//差分が-3.14以下の時
	if (fRot < -D3DX_PI)
	{
		//6.28加算
		fRot += D3DX_PI * 2;
	}
}


//------------------------------------------------------------------------------
//XYZ軸全部3.14以内に収める
//------------------------------------------------------------------------------
void CHossoLibrary::CalcRotation_XYZ(D3DXVECTOR3 & rot)
{
	CHossoLibrary::CalcRotation(rot.x);
	CHossoLibrary::CalcRotation(rot.y);
	CHossoLibrary::CalcRotation(rot.z);

}

//------------------------------------------------------------------------------
//ビルボード設定 XとZのみ
//------------------------------------------------------------------------------
void CHossoLibrary::SetBillboard_XZ_Only(D3DXMATRIX * pMtx)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//計算用変数
	D3DXMATRIX mtxView;

	//ビューマトリックス初期化
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ビルボード設定
	pMtx->_11 = mtxView._11;
	pMtx->_12 = mtxView._21;
	pMtx->_13 = mtxView._31;
	//pMtx->_21 = mtxView._12;
	//pMtx->_22 = mtxView._22;
	//pMtx->_23 = mtxView._32;
	pMtx->_31 = mtxView._13;
	pMtx->_32 = mtxView._23;
	pMtx->_33 = mtxView._33;
}
//------------------------------------------------------------------------------
//ビルボード設定
//------------------------------------------------------------------------------
void CHossoLibrary::SetBillboard(D3DXMATRIX * pMtx)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//計算用変数
	D3DXMATRIX mtxView;

	//ビューマトリックス初期化
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ビルボード設定
	pMtx->_11 = mtxView._11;
	pMtx->_12 = mtxView._21;
	pMtx->_13 = mtxView._31;
	pMtx->_21 = mtxView._12;
	pMtx->_22 = mtxView._22;
	pMtx->_23 = mtxView._32;
	pMtx->_31 = mtxView._13;
	pMtx->_32 = mtxView._23;
	pMtx->_33 = mtxView._33;
}
//------------------------------------------------------------------------------
//範囲内の値に修正
//------------------------------------------------------------------------------
bool CHossoLibrary::RangeLimit_Equal_Int(int & nValue, int nMin, int nMax)
{
	//最小値より小さい時
	if (nValue < nMin)
	{
		//最小値に合わす
		nValue = nMin;
		return true;
	}
	//最大値より大きい時
	if (nValue > nMax)
	{
		//最大値に合わす
		nValue = nMax;
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
//範囲内の値に修正
//------------------------------------------------------------------------------
bool CHossoLibrary::RangeLimit_Equal_Float(float & nValue, float nMin, float nMax)
{
	//最小値より小さい時
	if (nValue < nMin)
	{
		//最小値に合わす
		nValue = nMin;
		return true;
	}
	//最大値より大きい時
	if (nValue > nMax)
	{
		//最大値に合わす
		nValue = nMax;
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
//縦のメニュー選択
//------------------------------------------------------------------------------
void CHossoLibrary::SelectVerticalMenu(int & nSelectNum, int const & nMaxSelect)
{
	//↑
	if (CHossoLibrary::CheckSelect() == DIRECTION::UP)
	{
		nSelectNum--;
		//一番↑にいったら
		if (nSelectNum < 0)
		{
			//一番↑に合わせる
			nSelectNum = 0;
		}
		else
		{
			//CManager::GetSound()->Play(CSound::LABEL_SE_SELECT);
		}
	}
	//↓
	if (CHossoLibrary::CheckSelect() == DIRECTION::DOWN)
	{
		nSelectNum++;
		//一番↓にいったら
		if (nSelectNum > nMaxSelect - 1)
		{
			//一番↓に合わせる
			nSelectNum = nMaxSelect - 1;
		}
		else
		{
			//CManager::GetSound()->Play(CSound::LABEL_SE_SELECT);
		}
	}
}
//------------------------------------------------------------------------------
//横のメニュー選択
//------------------------------------------------------------------------------
void CHossoLibrary::SelectHorizonMenu(int & nSelectNum, int const & nMaxSelect)
{
	//←
	if (CHossoLibrary::CheckSelect() == DIRECTION::LEFT)
	{
		nSelectNum--;
		if (nSelectNum < 0)
		{
			nSelectNum = 0;
		}
		else
		{
			//CManager::GetSound()->Play(CSound::LABEL_SE_SELECT);
		}
	}
	//→
	if (CHossoLibrary::CheckSelect() == DIRECTION::RIGHT)
	{
		nSelectNum++;
		if (nSelectNum > nMaxSelect - 1)
		{
			nSelectNum = nMaxSelect - 1;
		}
		else
		{
			//CManager::GetSound()->Play(CSound::LABEL_SE_SELECT);
		}
	}
}
//------------------------------------------------------------------------------
//画面外のランダムの座標
//------------------------------------------------------------------------------
D3DXVECTOR3 CHossoLibrary::RandomScreenOutPos(D3DXVECTOR3 Value)
{
	DIRECTION dir = (DIRECTION)(rand() % (int)DIRECTION::MAX);
	D3DXVECTOR3 pos = ZeroVector3;

	switch (dir)
	{
	case DIRECTION::UP:
		pos = SCREEN_CENTER_POS + D3DXVECTOR3(Random(SCREEN_WIDTH * 0.6f), -SCREEN_HEIGHT * 0.6f, 0.0f);
		break;
	case DIRECTION::DOWN:
		pos = SCREEN_CENTER_POS + D3DXVECTOR3(Random(SCREEN_WIDTH * 0.6f), +SCREEN_HEIGHT * 0.6f, 0.0f);

		break;
	case DIRECTION::LEFT:
		pos = SCREEN_CENTER_POS + D3DXVECTOR3(-SCREEN_WIDTH * 0.6f , Random(SCREEN_HEIGHT * 0.6f), 0.0f);

		break;
	case DIRECTION::RIGHT:
		pos = SCREEN_CENTER_POS + D3DXVECTOR3(+SCREEN_WIDTH * 0.6f, Random(SCREEN_HEIGHT * 0.6f), 0.0f);

		break;
	}



	return pos;
}
//------------------------------------------------------------------------------
//-3.14〜3.14を返す
//------------------------------------------------------------------------------
float CHossoLibrary::Random_PI()
{
	return float(rand() % 314 / 100.0f) - float(rand() % 314) / 100.0f;
}

//------------------------------------------------------------------------------
//入力された値の+-ランダムな値で返す
//------------------------------------------------------------------------------
float CHossoLibrary::Random(float fInputValue)
{
	//0除算防止
	if (fInputValue <= 0.0f)
	{
		return 0.0f;
	}

	int nValue = int(fInputValue * 100);

	float fValue;

	rand() % 2 ?
		fValue = float(rand() % nValue / 100.0f) :
		fValue = -float(rand() % nValue / 100.0f);

	return fValue;
}

//------------------------------------------------------------------------------
//入力された値の+-ランダムな値で返す
//------------------------------------------------------------------------------
D3DXVECTOR3 CHossoLibrary::RandomVector3(float Max)
{
	D3DXVECTOR3 Value = ZeroVector3;

	Value.x = Random(Max);
	Value.y = Random(Max);
	Value.z = Random(Max);


	return Value;
}


//------------------------------------------------------------------------------
//ImGuiのコンボボックス
//------------------------------------------------------------------------------
bool CHossoLibrary::ImGui_Combobox(std::vector<std::string> aItemNameList, std::string aTitle, int & nValue)
{
	bool bChange = false;

#ifdef _DEBUG
	//combo開始
	if (ImGui::BeginCombo(aTitle.data(), aItemNameList[nValue].data()))
	{
		//要素分繰り返す
		for (size_t nCnt = 0; nCnt < aItemNameList.size(); nCnt++)
		{

			//選択番号があってるかどうか
			bool is_selected = (aItemNameList[nValue] == aItemNameList[nCnt]);

			//選択された時の処理
			if (ImGui::Selectable(aItemNameList[nCnt].data(), is_selected))
			{
				//現在の選択項目設定
				nValue = nCnt;
				bChange = true;
			}
		}
		//combo終了
		ImGui::EndCombo();

	}
#endif //DEBUG
	return bChange;
}
