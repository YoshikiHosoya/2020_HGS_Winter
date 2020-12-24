//------------------------------------------------------------------------------
//
//キャラクター処理  [character.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "character.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "game.h"
#include "modelcharacter.h"
#include "collision.h"
#include "motion.h"
#include "camera.h"
#include "meshfield.h"
#include "model_object.h"
#include "Map.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
std::vector<std::unique_ptr<CDefaultParam>> CCharacter::m_DefaultParam = {};
std::vector<std::string> CCharacter::m_aParamFileName =
{
	{ "data/SAVEDATA/CHaracterParam/PlayerParam.txt" },
};

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------
#define INERTIA	(0.2f)			//摩擦
#define AIR_INERTIA (0.05f)		//空中時の摩擦
#define GRAVITY (0.7f)			//重力
//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CCharacter::CCharacter()
{
	m_pos = ZeroVector3;
	m_posOld = ZeroVector3;
	m_move = ZeroVector3;
	m_rot = ZeroVector3;
	m_nLife = 1;
	m_bAttack = false;
	m_bJump = false;
	m_bInvincible = false;
	m_bGravity = true;
	m_bAlive = true;
	m_nCntState = 0;
	m_State = STATE_NORMAL;
	m_Param = PARAM_PLAYER;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CCharacter::~CCharacter()
{
	if (m_pCollision)
	{
		m_pCollision.reset();
	}
}

//------------------------------------------------------------------------------
//デバッグ情報表記
//------------------------------------------------------------------------------
void CCharacter::ShowDebugInfo()
{
#ifdef _DEBUG

#endif //DEBUG
}

//------------------------------------------------------------------------------
//ダメージ処理
//------------------------------------------------------------------------------
bool CCharacter::ApplyDamage(int nDamage, int nBlueDamage)
{
	return false;
}

//------------------------------------------------------------------------------
//ステート設定
//------------------------------------------------------------------------------
void CCharacter::SetState(STATE nextstate)
{
	m_State = nextstate;
}

//------------------------------------------------------------------------------
//パラメータ設定
//------------------------------------------------------------------------------
void CCharacter::SetParam(PARAM param)
{
	//パラメータ設定
	m_Param = param;

	//最大ＨＰ設定
	m_nLife = m_DefaultParam[param]->GetMaxLife();
}

//------------------------------------------------------------------------------
//キャラクター情報表示
//------------------------------------------------------------------------------
void CCharacter::ShowCharacterInfo()
{
#ifdef _DEBUG
	//キャラクター情報情報
	if (ImGui::TreeNode("CharacterInfo"))
	{
		ImGui::Text("pos %.2f,%.2f,,%.2f", m_pos.x, m_pos.y, m_pos.z);
		ImGui::Text("move %.2f,%.2f,,%.2f", m_move.x, m_move.y, m_move.z);
		ImGui::Text("rot %.2f,%.2f,,%.2f", m_rot.x, m_rot.y, m_rot.z);


		ImGui::Text("bJump [%d] bInvincible [%d]", m_bJump, m_bInvincible);
		ImGui::Text("State [%d] nCntState [%d]", m_State, m_nCntState);


		ImGui::TreePop();
	}
#endif //DEBUG
}


//------------------------------------------------------------------------------
//パラメータ取得
//------------------------------------------------------------------------------
HRESULT CCharacter::LoadDefaultParam()
{
	//変数宣言
	FILE *pFile;			//ファイルのポインタ
	char cReadText[MAX_TEXT];
	char cHeadText[MAX_TEXT];
	char cDieText[MAX_TEXT];
	int nLife = 0;
	float fMoveSpeed = 0.0f;
	float fJumpSpeed = 0.0f;
	float fDashSpeed = 0.0f;
	float fAirSpeed = 0.0f;
	int nInvincinbleTime = 0;

	for (size_t nCnt = 0; nCnt < m_aParamFileName.size(); nCnt++)
	{
		//ファイルを開く
		pFile = fopen(m_aParamFileName[nCnt].data(), "r");

		//ファイルがあった場合
		if (pFile)
		{

			std::unique_ptr<CDefaultParam> pParam(new CDefaultParam);

			//パラメータの配列追加
			m_DefaultParam.emplace_back(std::move(pParam));

			std::cout << "new DefaultParam - " << nCnt << NEWLINE;

			//スクリプトがくるまで
			while (strcmp(cHeadText, "SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);
			}
			//スクリプトだったら
			if (strcmp(cHeadText, "SCRIPT") == 0)
			{
				//エンドスクリプトが来る前ループ
				while (strcmp(cHeadText, "END_SCRIPT") != 0)
				{
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s", &cHeadText);
					//改行
					if (strcmp(cHeadText, "\n") == 0)
					{
					}
					//キャラクターセットだったら
					else if (strcmp(cHeadText, "PARAMSET") == 0)
					{
						//エンドキャラクターセットがくるまでループ
						while (strcmp(cHeadText, "END_PARAMSET") != 0)
						{
							fgets(cReadText, sizeof(cReadText), pFile);
							sscanf(cReadText, "%s", &cHeadText);
							//最大HP
							if (strcmp(cHeadText, "MAX_LIFE") == 0)
							{
								sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nLife);
								std::cout << "m_DefaultParam - " << nCnt  << " MAX_LIFE >> " << nLife << NEWLINE;
								m_DefaultParam[nCnt]->SetMaxLife(nLife);
							}
							//移動速度
							if (strcmp(cHeadText, "MOVE_SPEED") == 0)
							{
								sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &fMoveSpeed);
								std::cout << "m_DefaultParam - " << nCnt << " MOVE_SPEED >> " << fMoveSpeed << NEWLINE;
								m_DefaultParam[nCnt]->SetMoveSpeed(fMoveSpeed);
							}
							//ダッシュ速度
							if (strcmp(cHeadText, "DASH_SPEED") == 0)
							{
								sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &fDashSpeed);
								std::cout << "m_DefaultParam - " << nCnt << " DASH_SPEED >> " << fDashSpeed << NEWLINE;
								m_DefaultParam[nCnt]->SetDashSpeed(fDashSpeed);
							}
							//ジャンプ力
							if (strcmp(cHeadText, "JUMP_SPEED") == 0)
							{
								sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &fJumpSpeed);
								std::cout << "m_DefaultParam - " << nCnt << " JUMP_SPEED >> " << fJumpSpeed << NEWLINE;
								m_DefaultParam[nCnt]->SetJumpSpeed(fJumpSpeed);
							}
							//空中の移動速度
							if (strcmp(cHeadText, "AIR_SPEED") == 0)
							{
								sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &fAirSpeed);
								std::cout << "m_DefaultParam - " << nCnt << " AIR_SPEED >> " << fAirSpeed << NEWLINE;
								m_DefaultParam[nCnt]->SetAirSpeed(fAirSpeed);
							}
							//無敵時間
							if (strcmp(cHeadText, "INVINCIBLE_TIME") == 0)
							{
								sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nInvincinbleTime);
								std::cout << "m_DefaultParam - " << nCnt << " INVINCIBLE_TIME >> " << nInvincinbleTime << NEWLINE;
								m_DefaultParam[nCnt]->SetInvincinbleTime(nInvincinbleTime);
							}
							//終了
							if (strcmp(cHeadText, "END_PARAMSET") == 0)
							{
								std::cout << "LoadSuccess!" << "LoadDefaultParam() - " << nCnt << NEWLINE;
								std::cout << NEWLINE;
								break;
							}
						}
					}
				}
			}
			fclose(pFile);
		}

		//開けなかった時
		else
		{
			std::cout << "LoadFailed!!  Can't Open File." << "LoadDefaultParam() - " << nCnt << NEWLINE;

		}
	}
	return S_OK;
}