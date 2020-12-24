//------------------------------------------------------------------------------
//
//キャラクター用モデル処理  [modelcharacter.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "modelcharacter.h"
#include "modelparts.h"
#include "manager.h"
#include "keyboard.h"
#include "game.h"
#include "sceneX.h"
#include "game.h"
#include "modelinfo.h"
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
std::vector<std::unique_ptr<CModelCharacterInfo>> CModelCharacter::m_pModelCharacterList;
char *CModelCharacter::m_aFileName[CHARACTER_MAX]
{
	{ "data/SAVEDATA/Woman/Woman.txt" },
	{ "data/SAVEDATA/Armor/Armor.txt" },

};
//------------------------------------------------------------------------------
// マクロ
//------------------------------------------------------------------------------
#define OFFSET_SAVE_TEXT ("data/SAVEDATA/Offset.txt")
//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CModelCharacter::CModelCharacter()
{
	m_pModelPartsList.clear();
	m_CharacterMtx = nullptr;
	m_nFrame = 0;
	m_nKey = 0;
	m_type = CHARACTER_TYPE::CHARACTER_PLAYER;
	m_motion = CMotion::MOTION_NONE;
	m_motionOld = CMotion::MOTION_NONE;
	m_bMotionStop = false;
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CModelCharacter::~CModelCharacter()
{
	m_pModelPartsList.clear();
}
//------------------------------------------------------------------------------
//初期化処理
//------------------------------------------------------------------------------
HRESULT CModelCharacter::Init()
{
	//モデル数分繰り返す
	for (size_t nCnt = 0; nCnt < m_pModelCharacterList[m_type]->pModelInfoList.size(); nCnt++)
	{
		//メモリ確保+配列に追加
		m_pModelPartsList.emplace_back(std::move(CModelParts::Create(m_pModelCharacterList[m_type]->pModelInfoList[nCnt])));
	}

	//パーツの相対座標取得
	LoadModelRelativeValue(m_type);

	//初期モーションセット
	SetDefaultMotion();

	//強制的にモーションの所まで回転
	ForcedUpdate(m_motion, m_nKey);

	return S_OK;
}
//------------------------------------------------------------------------------
//終了処理
//------------------------------------------------------------------------------
void CModelCharacter::Uninit()
{
}
//------------------------------------------------------------------------------
//更新処理
//------------------------------------------------------------------------------
void CModelCharacter::Update()
{
	//1F前のモーション保存
	m_motionOld = m_motion;

	//フレーム加算
	m_nFrame++;

	//フレームが最大いったかどうか
	FrameCheck();

	if (!m_bMotionStop)
	{
		//nullcheck
		if (!m_pModelPartsList.empty())
		{
			//モデル数分繰り返す
			for (size_t nCnt = 0; nCnt < m_pModelPartsList.size(); nCnt++)
			{
				//更新
				m_pModelPartsList[nCnt]->Update();
			}
		}
	}
}
//------------------------------------------------------------------------------
//描画処理
//------------------------------------------------------------------------------
void CModelCharacter::Draw()
{
	//nullcheck
	if (!m_pModelPartsList.empty())
	{
		//モデル数分繰り返す
		for (size_t nCnt = 0; nCnt < m_pModelPartsList.size(); nCnt++)
		{
			//描画
			m_pModelPartsList[nCnt]->Draw();
		}
	}
}
//------------------------------------------------------------------------------
//生成
//------------------------------------------------------------------------------
std::unique_ptr<CModelCharacter> CModelCharacter::Create(CHARACTER_TYPE const modeltype)
{
	//メモリ確保
	std::unique_ptr<CModelCharacter>  pModelCharacter(new CModelCharacter);

	//タイプ
	pModelCharacter->m_type = modeltype;

	//初期化
	pModelCharacter->Init();

	//return
	return pModelCharacter;
}

//------------------------------------------------------------------------------
//読み込み
//------------------------------------------------------------------------------
HRESULT CModelCharacter::Load()
{
	for (int nCntModel = 0; nCntModel < CHARACTER_MAX; nCntModel++)
	{
		//モデルキャラクタ用のメモリ確保
		std::unique_ptr<CModelCharacterInfo> pModelCharacter(new CModelCharacterInfo);

		m_pModelCharacterList.emplace_back(std::move(pModelCharacter));

		//体のパーツ分
		for (int nCntParts = 0; nCntParts < 15; nCntParts++)
		{
			//Chara * 100 プレイヤーは100番台　とかに合わす為
			m_pModelCharacterList[nCntModel]->pModelInfoList.emplace_back(CModelInfo::GetModelInfo(CModelInfo::MODEL_TYPE(((nCntModel + 1) * 100) + nCntParts)));
		}

		//武器 900番台
		m_pModelCharacterList[nCntModel]->pModelInfoList.emplace_back(CModelInfo::GetModelInfo(CModelInfo::MODEL_TYPE(900 + nCntModel)));

		std::cout << "new CHARACTERCHARACTER : " << nCntModel << NEWLINE;

		std::cout << NEWLINE;
	}

	return S_OK;
}
//------------------------------------------------------------------------------
//開放
//------------------------------------------------------------------------------
void CModelCharacter::UnLoad()
{
	if (m_pModelCharacterList.empty())
	{
		m_pModelCharacterList.clear();
	}
}
//------------------------------------------------------------------------------
//キャラクターのマトリックス設定
//------------------------------------------------------------------------------
void CModelCharacter::SetCharacterMtx(D3DXMATRIX *mtx)
{
	//モデル総数分
	for (size_t nCnt = 0; nCnt < m_pModelPartsList.size(); nCnt++)
	{
		//親がいない時
		if (m_pModelPartsList[nCnt]->GetParent() == -1)
		{
			//大元のマトリックス設定
			m_pModelPartsList[nCnt]->SetParentMtx(mtx);
		}
		else
		{
			//親パーツのマトリックス設定
			m_pModelPartsList[nCnt]->SetParentMtx(m_pModelPartsList[m_pModelPartsList[nCnt]->GetParent()]->GetMtxWorldPtr());
		}
	}
}

//------------------------------------------------------------------------------
//モーションセット
//------------------------------------------------------------------------------
void CModelCharacter::SetMotion(CMotion::MOTION_TYPE motiontype)
{
	//nullcheck
	if (m_pModelCharacterList[m_type])
	{
		//モーション再生
		SetMotionStop(false);

		//モーションの切り替え
		m_motion = motiontype;

		//nullcheck
		if (!m_pModelPartsList.empty())
		{
			m_pModelPartsList[0]->SetMotionPosDest(motiontype, m_nKey);

			//モデル数分繰り返す
			for (size_t nCnt = 0; nCnt < m_pModelPartsList.size(); nCnt++)
			{
				//モーションの回転の決定先取得
				m_pModelPartsList[nCnt]->SetMotionRotDest(motiontype, m_nKey);
			}
		}
	}
}
//------------------------------------------------------------------------------
//デフォルトのモーションセット
//------------------------------------------------------------------------------
void CModelCharacter::SetDefaultMotion()
{
	switch (m_type)
	{
	case CHARACTER_TYPE::CHARACTER_PLAYER:
		//ニュートラルに戻す
		m_motion = CMotion::PLAYER_NEUTRAL;
		break;

	default:
		break;
	}
}

//------------------------------------------------------------------------------
//imgui用
//------------------------------------------------------------------------------
void CModelCharacter::ShowModelCharacterInfo()
{
#ifdef _DEBUG

	//キャラクター情報情報
	if (ImGui::TreeNode("ModelCharacterInfo"))
	{
		ImGui::Text("Motion ; %d", m_motion);
		ImGui::Text("Key ; %d", m_nKey);
		ImGui::Text("Frame ; %d", m_nFrame);

		ImGui::TreePop();
	}
#endif //DEBUG
}

//------------------------------------------------------------------------------
// パーツ情報読み込み
//------------------------------------------------------------------------------
HRESULT CModelCharacter::LoadModelRelativeValue(CHARACTER_TYPE type)
{
	//変数宣言
	FILE *pFile;			//ファイルのポインタ
	char cReadText[MAX_TEXT];
	char cHeadText[MAX_TEXT];
	char cDieText[MAX_TEXT];
	int nCntParts = 0;			//パーツ数
	int nCntWeaponParts = 0;	//武器パーツ数
	D3DXVECTOR3 pos = ZeroVector3;
	D3DXVECTOR3 rot = ZeroVector3;
	int nParent = 0;
	int nIndex = 0;

	//ファイルを開く
	pFile = fopen(m_aFileName[type], "r");

	//ファイルがあった場合
	if (pFile)
	{
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
				else if (strcmp(cHeadText, "CHARACTERSET") == 0)
				{
					//エンドキャラクターセットがくるまでループ
					while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);
						//改行
						if (strcmp(cHeadText, "\n") == 0)
						{
						}
						//パーツセットだったら
						else if (strcmp(cHeadText, "PARTSSET") == 0)
						{
							//エンドパーツセットがくるまでループ
							while (strcmp(cHeadText, "END_PARTSSET") != 0)
							{
								fgets(cReadText, sizeof(cReadText), pFile);
								sscanf(cReadText, "%s", &cHeadText);

								//インデックス
								if (strcmp(cHeadText, "INDEX") == 0)
								{
									sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nIndex);
								}
								//親番号
								if (strcmp(cHeadText, "PARENT") == 0)
								{
									sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nParent);
								}
								//ポジション
								if (strcmp(cHeadText, "POS") == 0)
								{
									sscanf(cReadText, "%s %s %f %f %f", &cDieText, &cDieText, &pos.x, &pos.y, &pos.z);
								}
								//回転量
								if (strcmp(cHeadText, "ROT") == 0)
								{
									sscanf(cReadText, "%s %s %f %f %f", &cDieText, &cDieText, &rot.x, &rot.y, &rot.z);
								}

								if (strcmp(cHeadText, "END_PARTSSET") == 0)
								{
									m_pModelPartsList[nCntParts]->SetPartsInfo(nIndex, nParent, pos, rot);

									//カウントアップ
									nCntParts++;
								}
							}
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
		std::cout << "テキスト読み込み失敗" << "LoadModelRelativeValue()" << NEWLINE;
		return E_FAIL;
	}
	return S_OK;
}
//------------------------------------------------------------------------------
// モデル名読み込み
//------------------------------------------------------------------------------
HRESULT CModelCharacter::LoadModelName(CHARACTER_TYPE type)
{
	//変数宣言
	FILE *pFile;					//ファイルのポインタ
	char cReadText[MAX_TEXT];
	char cHeadText[MAX_TEXT];
	char cDieText[MAX_TEXT];
	int nBool = 0;
	int nNumModel = 0;		//モデル数
	int nCntModel = 0;		//モデル数のカウント
	int nWeaponID = 0;		//武器番号

							//ファイルを開く
	pFile = fopen(m_aFileName[type], "r");

	//ファイルがあった場合
	if (pFile != NULL)
	{
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
				//モデル番号だったら
				if (strcmp(cHeadText, "WEAPON_ID") == 0)
				{
					//武器番号取得
					sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nWeaponID);

					//配列に追加
					m_pModelCharacterList[type]->nWeaponID.emplace_back(nWeaponID);

					//debug モデル名出力
					std::cout << "WeaponID >> " << nWeaponID << NEWLINE;

				}

			}
		}
		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		//失敗
		std::cout << "テキスト読み込み失敗" << "LoadModelName()" << NEWLINE;

		return E_FAIL;
	}

	return S_OK;
}

//------------------------------------------------------------------------------
// オフセット情報保存
//------------------------------------------------------------------------------
HRESULT CModelCharacter::SaveModelOffset()
{
	FILE *pFile;

	int nRotNum = 0;
	char cHeadtext[128];
	char cWriteText[128];

	//ファイル読み込み
	pFile = fopen(OFFSET_SAVE_TEXT, "w");

	//nullcheck
	if (pFile != nullptr)
	{
		//ブロックコメント
		fputs(COMMENT02, pFile);
		fputs(COMMENT01, pFile);

		strcpy(cHeadtext, "//Offset\n");
		strcpy(cHeadtext, "//テキストにコピペしてください\n");

		fputs(cHeadtext, pFile);

		strcpy(cHeadtext, "//Author:Yoshiki Hosoya\n");
		fputs(cHeadtext, pFile);

		fputs(COMMENT01, pFile);
		fputs(COMMENT02, pFile);
		fputs(NEWLINE, pFile);

		//ブロックコメント終了//

			//キャラクターセット
			fputs("CHARACTERSET", pFile);
			fputs(NEWLINE, pFile);

			//nullcheck
			if (!m_pModelPartsList.empty())
			{
				for (size_t nCnt = 0; nCnt < m_pModelPartsList.size(); nCnt++)
				{

					//パーツセット
					fputs("	PARTSSET", pFile);
					fputs(NEWLINE, pFile);

					//インデックス
					sprintf(cWriteText, "		%s %s %d", "INDEX", &EQUAL, m_pModelPartsList[nCnt]->GetIndex());
					fputs(cWriteText, pFile);
					fputs(NEWLINE, pFile);

					//親番号
					sprintf(cWriteText, "		%s %s %d", "PARENT", &EQUAL, m_pModelPartsList[nCnt]->GetParent());
					fputs(cWriteText, pFile);
					fputs(NEWLINE, pFile);

					//座標
					sprintf(cWriteText, "		%s %s %.1f %.1f %.1f", "POS", &EQUAL,
						m_pModelPartsList[nCnt]->GetPos().x,
						m_pModelPartsList[nCnt]->GetPos().y,
						m_pModelPartsList[nCnt]->GetPos().z);
					fputs(cWriteText, pFile);
					fputs(NEWLINE, pFile);

					//回転
					sprintf(cWriteText, "		%s %s %.1f %.1f %.1f", "ROT", &EQUAL,
						m_pModelPartsList[nCnt]->GetRot().x,
						m_pModelPartsList[nCnt]->GetRot().y,
						m_pModelPartsList[nCnt]->GetRot().z);
					fputs(cWriteText, pFile);
					fputs(NEWLINE, pFile);

					//パーツセット終了
					fputs("	END_PARTSSET", pFile);
					fputs(NEWLINE, pFile);
					fputs(NEWLINE, pFile);

				}
			}
			//アタックセット終了
			fputs("END_CHARACTERSET", pFile);
			fputs(NEWLINE, pFile);
			fputs(NEWLINE, pFile);


		std::cout << "Save Succsess!!  SaveModelOffset()" << NEWLINE;

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		//ファイルが開けませんでした
		std::cout << "Save FAILED!!  Can't Open File. SaveModelOffset()" << NEWLINE;
		return E_FAIL;
	}
	return S_OK;
}
//------------------------------------------------------------------------------
// フレームが最後まで行ったか確認
//------------------------------------------------------------------------------
void CModelCharacter::FrameCheck()
{
	//フレームが一定値まで来た時
	if (m_nFrame >= CMotion::GetFrame(m_motion, m_nKey))
	{
		//フレーム初期化
		m_nFrame = 0;

		//キー加算
		m_nKey++;

		//キーが最後まで行ったか
		KeyCheck();

		//次のモーション情報をセット
		SetMotion(m_motion);
	}
}

//------------------------------------------------------------------------------
// 強制的にモーション切り替え　０F目の回転のとこにもってく
//------------------------------------------------------------------------------
void CModelCharacter::ForcedUpdate(CMotion::MOTION_TYPE motiontype, int nKey)
{
	m_pModelPartsList[0]->SetForcedPos(motiontype, m_nKey);

	//モデル数分繰り返す
	for (size_t nCnt = 0; nCnt < m_pModelPartsList.size(); nCnt++)
	{
		//モーションの回転の決定先取得
		m_pModelPartsList[nCnt]->SetForcedRot(motiontype, m_nKey);
	}
}

//------------------------------------------------------------------------------
// 移動するキーか確認
//------------------------------------------------------------------------------
bool CModelCharacter::CheckMoveKey()
{
	//フレームが０の時
	if (m_nFrame == 0)
	{
		//nullcheck
		if (CMotion::GetAttackInfo(m_motion))
		{
			//キーが指定されたキーの時
			if (CMotion::GetAttackInfo(m_motion)->nMoveKey == m_nKey)
			{
				return true;
			}
		}
	}
	return false;
}

//------------------------------------------------------------------------------
//腰の高さ取得
//------------------------------------------------------------------------------
float CModelCharacter::GetWaistHeight()
{
	return m_pModelPartsList[0]->GetPos().y;
}

//------------------------------------------------------------------------------
// パーツのインデックス情報取得
//------------------------------------------------------------------------------
CModelParts * CModelCharacter::GetPartsPtr(int nIndex)
{
	//モデル総数分
	for (size_t nCnt = 0; nCnt < m_pModelPartsList.size(); nCnt++)
	{
		//インデックスがあってるか
		if (m_pModelPartsList[nCnt]->GetIndex() == nIndex)
		{
			//return
			return m_pModelPartsList[nCnt].get();
		}

	}

	return nullptr;
}

//------------------------------------------------------------------------------
// キーが最後までいったのを確認
//------------------------------------------------------------------------------
void CModelCharacter::KeyCheck()
{
	//キーが一定値まで来た時
	if (m_nKey >= CMotion::GetNumKey(m_motion))
	{
		//フレームとキー初期化
		ResetMotion();

		//ループしない時
		if (!CMotion::CheckLoop(m_motion))
		{
			//デフォルトモーションセット
			SetDefaultMotion();
		}
	}
}
