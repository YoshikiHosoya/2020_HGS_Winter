//------------------------------------------------------------------------------
//
//モーション処理  [motion.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "motion.h"
#include "renderer.h"
#include "manager.h"
#include "modelcharacter.h"

//------------------------------------------------------------------------------
// マクロ
//------------------------------------------------------------------------------
#define MOTION_FILE_NAME ("data/SAVEDATA/MotionFIleName.txt")
//------------------------------------------------------------------------------
//静的メンバ変数の初期化
//------------------------------------------------------------------------------
std::vector<std::unique_ptr<CMotion::MOTION_INFO>> CMotion::m_pMotionInfoList = {};
std::vector<std::string> CMotion::m_apFileName = {};
//------------------------------------------------------------------------------
//コンストラクタ
//------------------------------------------------------------------------------
CMotion::CMotion()
{
}

//------------------------------------------------------------------------------
//デストラクタ
//------------------------------------------------------------------------------
CMotion::~CMotion()
{
}

//------------------------------------------------------------------------------
//モーション読み込み
//------------------------------------------------------------------------------
HRESULT CMotion::Load()
{
	//モーションのファイル名取得
	LoadMotionFileName();

	//リスト分繰り返す
	for (size_t nCnt = 0; nCnt < m_apFileName.size(); nCnt++)
	{
		//メモリ確保
		std::unique_ptr<MOTION_INFO> pMotion(new MOTION_INFO);

		//配列に追加
		m_pMotionInfoList.emplace_back(std::move(pMotion));

		//debug
		std::cout << "new MOTION_INFO : " << nCnt << "-";

		//モデル読み込み
		if (FAILED(LoadMotion((CMotion::MOTION_TYPE)nCnt)))
		{
			//失敗
			std::cout << "LoadMotion() [FAILED] : " << NEWLINE;		//成功
			return E_FAIL;
		}
		else
		{
			std::cout << "LoadMotion() [Success] : " << NEWLINE;		//成功
		}
		//改行
		std::cout << NEWLINE;
	}
	//成功
	return S_OK;
}
//------------------------------------------------------------------------------
//モーション情報破棄
//------------------------------------------------------------------------------
void CMotion::UnLoad()
{

}

//------------------------------------------------------------------------------
//デバッグ情報出力
//------------------------------------------------------------------------------
void CMotion::ShowDebugInfo()
{
#ifdef _DEBUG



#endif //_DEBUG
}

//------------------------------------------------------------------------------
//キー追加
//------------------------------------------------------------------------------
void CMotion::AddKeyInfo(MOTION_TYPE NowMotion, int model)
{
	std::unique_ptr<KEY> pKey;						//仮のポインタ
	std::unique_ptr<KEY_INFO> pKeyInfo;				//仮のポインタ

	//メモリ確保
	pKeyInfo.reset(new KEY_INFO);

	//配列に追加
	m_pMotionInfoList[NowMotion]->pKeyInfoList.emplace_back(std::move(pKeyInfo));

	//キー分も追加
	for (int nCnt = 0; nCnt < CModelCharacter::GetModelNum((CModelCharacter::CHARACTER_TYPE) model) ; nCnt++)
	{
		//メモリ確保
		pKey.reset(new KEY);

		//配列に追加
		m_pMotionInfoList[NowMotion]->pKeyInfoList[m_pMotionInfoList[NowMotion]->nNumKey - 1]->pKeyList.emplace_back(std::move(pKey));
	}
}
//------------------------------------------------------------------------------
//キー消去
//------------------------------------------------------------------------------
void CMotion::PopbackKeyInfo(MOTION_TYPE NowMotion)
{
	m_pMotionInfoList[NowMotion]->pKeyInfoList.pop_back();
}


//------------------------------------------------------------------------------
//モーション読み込み
//------------------------------------------------------------------------------
HRESULT CMotion::LoadMotion(MOTION_TYPE motiontype)
{
	//変数宣言
	FILE *pFile;					//ファイルのポインタ
	char cReadText[MAX_TEXT] = {};
	char cHeadText[MAX_TEXT] = {};
	char cDieText[MAX_TEXT] = {};
	int nLoop = 0;						//ループするか
	int nCntkeyInfo = 0;				//キー情報
	int nCntKey = 0;					//キー
	int boolAttack = 0;					//攻撃するか

	std::unique_ptr<KEY> pKey;						//仮のポインタ
	std::unique_ptr<KEY_INFO> pKeyInfo;				//仮のポインタ

	//ファイルを開く
	pFile = fopen(m_apFileName[motiontype].data(), "r");

	//debug モデル名出力
	std::cout << "Name >> " << m_apFileName[motiontype].data() << NEWLINE;

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

				//ループするか
				if (strcmp(cHeadText, "LOOP") == 0)
				{
					sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nLoop);		//一旦int型に格納
					m_pMotionInfoList[motiontype]->bLoop = nLoop ? true : false;				//0か1か判断
				}
				//キー数
				if (strcmp(cHeadText, "NUM_KEY") == 0)
				{
					sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &m_pMotionInfoList[motiontype]->nNumKey);
				}
				//攻撃するか
				if (strcmp(cHeadText, "ATTACK") == 0)
				{
					sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &boolAttack);

					//攻撃するかどうか設定
					m_pMotionInfoList[motiontype]->bAttack = boolAttack ? true : false;				//0か1か判断
				}

				//攻撃情報の設定
				else if (strcmp(cHeadText, "ATTACKSET") == 0)
				{
					//メモリ確保
					m_pMotionInfoList[motiontype]->pAttackInfo.reset(new ATTACK_INFO);

					//エンドアタックセットがくるまでループ
					while (strcmp(cHeadText, "END_ATTACKSET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);

						//攻撃力
						if (strcmp(cHeadText, "ATTACK_VALUE") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &m_pMotionInfoList[motiontype]->pAttackInfo->nAttack);
							std::cout << "AttackInfo - " << motiontype << " - ATTACK_VALUE - " << m_pMotionInfoList[motiontype]->pAttackInfo->nAttack << NEWLINE;
						}
						//青攻撃力
						if (strcmp(cHeadText, "BLUEATTACK_VALUE") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &m_pMotionInfoList[motiontype]->pAttackInfo->nBlueAttack);
							std::cout << "AttackInfo - " << motiontype << " - BLUEATTACK_VALUE - " << m_pMotionInfoList[motiontype]->pAttackInfo->nBlueAttack << NEWLINE;
						}
						//判定の長さ
						if (strcmp(cHeadText, "COLLISION_LENGTH") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_pMotionInfoList[motiontype]->pAttackInfo->fLength);
							std::cout << "AttackInfo - " << motiontype << " - COLLISION_LENGTH - " << m_pMotionInfoList[motiontype]->pAttackInfo->fLength << NEWLINE;
						}
						//判定の半径
						if (strcmp(cHeadText, "COLLISION_RADIUS") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_pMotionInfoList[motiontype]->pAttackInfo->fRadius);
							std::cout << "AttackInfo - " << motiontype << " - COLLISION_RADIUS - " << m_pMotionInfoList[motiontype]->pAttackInfo->fRadius << NEWLINE;
						}
						//判定開始
						if (strcmp(cHeadText, "COLLISION_START") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &m_pMotionInfoList[motiontype]->pAttackInfo->nStartCollisonKey);
							std::cout << "AttackInfo - " << motiontype << " - COLLISION_START - " << m_pMotionInfoList[motiontype]->pAttackInfo->nStartCollisonKey << NEWLINE;
						}
						//判定終了
						if (strcmp(cHeadText, "COLLISION_END") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &m_pMotionInfoList[motiontype]->pAttackInfo->nEndCollisionKey);
							std::cout << "AttackInfo - " << motiontype << " - COLLISION_END - " << m_pMotionInfoList[motiontype]->pAttackInfo->nEndCollisionKey << NEWLINE;
						}
						//移動速度
						if (strcmp(cHeadText, "MOVE_KEY") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &m_pMotionInfoList[motiontype]->pAttackInfo->nMoveKey);
							std::cout << "AttackInfo - " << motiontype << " - MOVE_KEY - " << m_pMotionInfoList[motiontype]->pAttackInfo->nMoveKey << NEWLINE;
						}
						//移動速度
						if (strcmp(cHeadText, "MOVE_FRONT_VALUE") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_pMotionInfoList[motiontype]->pAttackInfo->fMoveFront);
							std::cout << "AttackInfo - " << motiontype << " - MOVE_FRONT_VALUE - " << m_pMotionInfoList[motiontype]->pAttackInfo->fMoveFront << NEWLINE;
						}
						//移動速度
						if (strcmp(cHeadText, "MOVE_UP_VALUE") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_pMotionInfoList[motiontype]->pAttackInfo->fMoveUp);
							std::cout << "AttackInfo - " << motiontype << " - MOVE_UP_VALUE - " << m_pMotionInfoList[motiontype]->pAttackInfo->fMoveUp << NEWLINE;
						}
						//ノックバックの移動量
						if (strcmp(cHeadText, "KNOCKBACK_FRONT_VALUE") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_pMotionInfoList[motiontype]->pAttackInfo->fKnockBack_Front);
							std::cout << "AttackInfo - " << motiontype << " - KNOCKBACK_FRONT_VALUE - " << m_pMotionInfoList[motiontype]->pAttackInfo->fKnockBack_Front << NEWLINE;
						}
						//ノックバックの移動量
						if (strcmp(cHeadText, "KNOCKBACK_UP_VALUE") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_pMotionInfoList[motiontype]->pAttackInfo->fKnockBack_Up);
							std::cout << "AttackInfo - " << motiontype << " - KNOCKBACK_UP_VALUE - " << m_pMotionInfoList[motiontype]->pAttackInfo->fKnockBack_Up << NEWLINE;
						}
						//軌跡の色
						if (strcmp(cHeadText, "ORBIT_COL") == 0)
						{
							sscanf(cReadText, "%s %s %f %f %f %f", &cDieText, &cDieText,
								&m_pMotionInfoList[motiontype]->pAttackInfo->orbitcol.r, &m_pMotionInfoList[motiontype]->pAttackInfo->orbitcol.g,
								&m_pMotionInfoList[motiontype]->pAttackInfo->orbitcol.b, &m_pMotionInfoList[motiontype]->pAttackInfo->orbitcol.a);
						}
					}
				}
				//モデル番号だったら
				else if (strcmp(cHeadText, "KEYSET") == 0)
				{
					//メモリ確保
					pKeyInfo.reset(new KEY_INFO);

					//配列に追加
					m_pMotionInfoList[motiontype]->pKeyInfoList.emplace_back(std::move(pKeyInfo));

					//カウントリセット
					nCntKey = 0;

					//エンドパーツセットがくるまでループ
					while (strcmp(cHeadText, "END_KEYSET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);

						//改行
						if (strcmp(cHeadText, "\n") == 0)
						{
						}

						//フレーム数
						else if (strcmp(cHeadText, "FRAME") == 0)
						{
							sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &m_pMotionInfoList[motiontype]->pKeyInfoList[nCntkeyInfo]->nFrame);
						}
						//フレーム数
						else if (strcmp(cHeadText, "HEIGHT") == 0)
						{
							sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_pMotionInfoList[motiontype]->pKeyInfoList[nCntkeyInfo]->fOriginHeight);
						}
						//キーだったら
						else if (strcmp(cHeadText, "KEY") == 0)
						{
							//メモリ確保
							pKey.reset(new KEY);

							//配列に追加
							m_pMotionInfoList[motiontype]->pKeyInfoList[nCntkeyInfo]->pKeyList.emplace_back(std::move(pKey));

							sscanf(cReadText, "%s %s %s %f %f %f", &cDieText, &cDieText, &cDieText,
								&m_pMotionInfoList[motiontype]->pKeyInfoList[nCntkeyInfo]->pKeyList[nCntKey]->rotDest.x,
								&m_pMotionInfoList[motiontype]->pKeyInfoList[nCntkeyInfo]->pKeyList[nCntKey]->rotDest.y,
								&m_pMotionInfoList[motiontype]->pKeyInfoList[nCntkeyInfo]->pKeyList[nCntKey]->rotDest.z);

							//キー加算
							nCntKey++;
						}
						//終了
						if (strcmp(cHeadText, "END_KEYSET") == 0)
						{
							//キー情報加算
							nCntkeyInfo++;
						}
					}
				}
			}
		}
		//保存成功
		std::cout << "Load Succsess!!  LoadMotion()" << NEWLINE;

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		//ファイルが開けませんでした
		std::cout << "Load FAILED!!  Can't Open File. LoadMotion()" << NEWLINE;
		return E_FAIL;
	}
	return S_OK;
}

//------------------------------------------------------------------------------
//攻撃するか確認
//------------------------------------------------------------------------------
bool CMotion::CheckAttackKey(MOTION_TYPE NowMotion, int nNowKey)
{
	if (m_pMotionInfoList[NowMotion]->bAttack)
	{
		if (nNowKey >= m_pMotionInfoList[NowMotion]->pAttackInfo->nStartCollisonKey && nNowKey <= m_pMotionInfoList[NowMotion]->pAttackInfo->nEndCollisionKey)
		{
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------
//モーションの保存
//------------------------------------------------------------------------------
HRESULT CMotion::SaveMotion(MOTION_TYPE motiontype)
{
	FILE *pFile;

	int nRotNum = 0;
	char cHeadtext[128];
	char cWriteText[128];

	//ファイル読み込み
	pFile = fopen(m_apFileName[motiontype].data(), "w");

	//nullcheck
	if (pFile != nullptr)
	{
		//ブロックコメント
		fputs(COMMENT02, pFile);
		fputs(COMMENT01, pFile);

		strcpy(cHeadtext, "//Motion\n");
		fputs(cHeadtext, pFile);

		strcpy(cHeadtext, "//Author:Yoshiki Hosoya\n");
		fputs(cHeadtext, pFile);

		fputs(COMMENT01, pFile);
		fputs(COMMENT02, pFile);
		fputs(NEWLINE, pFile);

		//ブロックコメント終了//

		//スクリプト
		strcpy(cHeadtext, "SCRIPT");
		fputs(cHeadtext, pFile);
		fputs(NEWLINE, pFile);

		//改行
		fputs(NEWLINE, pFile);

		//ループ
		strcpy(cHeadtext, "LOOP");
		sprintf(cWriteText, "	%s %s %d		%s", "LOOP", &EQUAL, m_pMotionInfoList[motiontype]->bLoop,	"//ループするかどうか");
		fputs(cWriteText, pFile);
		fputs(NEWLINE, pFile);

		//キー数
		strcpy(cHeadtext, "NUM_KEY");
		sprintf(cWriteText, "	%s %s %d		%s", "NUM_KEY", &EQUAL, m_pMotionInfoList[motiontype]->nNumKey, "//キー数");
		fputs(cWriteText, pFile);
		fputs(NEWLINE, pFile);

		//攻撃するかどうか
		strcpy(cHeadtext, "ATTACK");
		sprintf(cWriteText, "	%s %s %d		%s","ATTACK", &EQUAL, m_pMotionInfoList[motiontype]->bAttack, "//攻撃するかどうか");
		fputs(cWriteText, pFile);
		fputs(NEWLINE, pFile);
		fputs(NEWLINE, pFile);

		//攻撃する場合
		if (m_pMotionInfoList[motiontype]->bAttack)
		{
			//アタックセット
			fputs("	ATTACKSET", pFile);
			fputs(NEWLINE, pFile);

			//攻撃力
			sprintf(cWriteText, "		%s %s %d		%s", "ATTACK_VALUE", &EQUAL, m_pMotionInfoList[motiontype]->pAttackInfo->nAttack, "//攻撃力");
			fputs(cWriteText, pFile);
			fputs(NEWLINE, pFile);

			//攻撃力（青）
			sprintf(cWriteText, "		%s %s %d		%s", "BLUEATTACK_VALUE", &EQUAL, m_pMotionInfoList[motiontype]->pAttackInfo->nBlueAttack, "//青攻撃力");
			fputs(cWriteText, pFile);
			fputs(NEWLINE, pFile);

			//判定の長さ
			sprintf(cWriteText, "		%s %s %.1f		%s", "COLLISION_LENGTH", &EQUAL, m_pMotionInfoList[motiontype]->pAttackInfo->fLength, "//判定の長さ");
			fputs(cWriteText, pFile);
			fputs(NEWLINE, pFile);

			//判定の半径
			sprintf(cWriteText, "		%s %s %.1f		%s", "COLLISION_RADIUS", &EQUAL, m_pMotionInfoList[motiontype]->pAttackInfo->fRadius, "//判定の半径");
			fputs(cWriteText, pFile);
			fputs(NEWLINE, pFile);

			//判定開始
			sprintf(cWriteText, "		%s %s %d		%s", "COLLISION_START", &EQUAL, m_pMotionInfoList[motiontype]->pAttackInfo->nStartCollisonKey, "//判定開始のキー");
			fputs(cWriteText, pFile);
			fputs(NEWLINE, pFile);

			//判定終了
			sprintf(cWriteText, "		%s %s %d		%s", "COLLISION_END", &EQUAL, m_pMotionInfoList[motiontype]->pAttackInfo->nEndCollisionKey, "//判定終了のキー");
			fputs(cWriteText, pFile);
			fputs(NEWLINE, pFile);

			//移動するキー
			sprintf(cWriteText, "		%s %s %d		%s", "MOVE_KEY", &EQUAL, m_pMotionInfoList[motiontype]->pAttackInfo->nMoveKey, "//移動するキー");
			fputs(cWriteText, pFile);
			fputs(NEWLINE, pFile);

			//移動量
			sprintf(cWriteText, "		%s %s %.1f		%s", "MOVE_FRONT_VALUE", &EQUAL, m_pMotionInfoList[motiontype]->pAttackInfo->fMoveFront, "//前方の移動量");
			fputs(cWriteText, pFile);
			fputs(NEWLINE, pFile);

			//移動量
			sprintf(cWriteText, "		%s %s %.1f		%s", "MOVE_UP_VALUE", &EQUAL, m_pMotionInfoList[motiontype]->pAttackInfo->fMoveUp, "//上下の移動量");
			fputs(cWriteText, pFile);
			fputs(NEWLINE, pFile);

			//ノックバック
			sprintf(cWriteText, "		%s %s %.1f		%s", "KNOCKBACK_FRONT_VALUE", &EQUAL, m_pMotionInfoList[motiontype]->pAttackInfo->fKnockBack_Front, "//ノックバックの移動量　前後");
			fputs(cWriteText, pFile);
			fputs(NEWLINE, pFile);

			//ノックバック
			sprintf(cWriteText, "		%s %s %.1f		%s", "KNOCKBACK_UP_VALUE", &EQUAL, m_pMotionInfoList[motiontype]->pAttackInfo->fKnockBack_Up, "//ノックバックの移動量上下");
			fputs(cWriteText, pFile);
			fputs(NEWLINE, pFile);

			//軌跡の色
			sprintf(cWriteText, "		%s %s %.1f %.1f %.1f %.1f		%s", "ORBIT_COL", &EQUAL, m_pMotionInfoList[motiontype]->pAttackInfo->orbitcol.r,
				m_pMotionInfoList[motiontype]->pAttackInfo->orbitcol.g,
				m_pMotionInfoList[motiontype]->pAttackInfo->orbitcol.b,
				m_pMotionInfoList[motiontype]->pAttackInfo->orbitcol.a, "//軌跡の色");
			fputs(cWriteText, pFile);
			fputs(NEWLINE, pFile);

			//アタックセット終了
			fputs("	END_ATTACKSET", pFile);
			fputs(NEWLINE, pFile);
			fputs(NEWLINE, pFile);


		}

		//nullcheck
		if (!m_pMotionInfoList[motiontype]->pKeyInfoList.empty())
		{
			for (int nCnt = 0; nCnt < m_pMotionInfoList[motiontype]->nNumKey; nCnt++)
			{
				//キーセット
				sprintf(cWriteText, "	KEYSET		//---< KEY : %d / %d >---", nCnt, m_pMotionInfoList[motiontype]->nNumKey);
				fputs(cWriteText, pFile);
				fputs(NEWLINE, pFile);

				//フレーム数
				sprintf(cWriteText, "		%s %s %d		%s", "FRAME", &EQUAL, m_pMotionInfoList[motiontype]->pKeyInfoList[nCnt]->nFrame, "//フレーム数");
				fputs(cWriteText, pFile);
				fputs(NEWLINE, pFile);

				//高さ
				sprintf(cWriteText, "		%s %s %.2f		%s", "HEIGHT", &EQUAL, m_pMotionInfoList[motiontype]->pKeyInfoList[nCnt]->fOriginHeight, "//原点の高さ");
				fputs(cWriteText, pFile);
				fputs(NEWLINE, pFile);

				fputs(NEWLINE, pFile);

				for (size_t nCntParts = 0; nCntParts < m_pMotionInfoList[motiontype]->pKeyInfoList[nCnt]->pKeyList.size(); nCntParts++)
				{
					//キー
					sprintf(cWriteText, "		KEY [%d] = %.3f %.3f %.3f", nCntParts, m_pMotionInfoList[motiontype]->pKeyInfoList[nCnt]->pKeyList[nCntParts]->rotDest.x,
					m_pMotionInfoList[motiontype]->pKeyInfoList[nCnt]->pKeyList[nCntParts]->rotDest.y, m_pMotionInfoList[motiontype]->pKeyInfoList[nCnt]->pKeyList[nCntParts]->rotDest.z);
					fputs(cWriteText, pFile);
					fputs(NEWLINE, pFile);

				}
			//キーセット終了
			fputs("	END_KEYSET", pFile);
			fputs(NEWLINE, pFile);
			fputs(NEWLINE, pFile);
			}

		}

		//キーセット終了
		fputs("END_SCRIPT", pFile);
		fputs(NEWLINE, pFile);

		//保存成功
		std::cout << "Motion Save Succsess!! >>" << m_apFileName[motiontype].data() << NEWLINE;

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		//ファイルが開けませんでした
		std::cout << "Motion Save FAILED!!  Can't Open File. SavePlaceData()" << NEWLINE;
		return E_FAIL;
	}
	return S_OK;
}
//------------------------------------------------------------------------------
//モーションコピー
//------------------------------------------------------------------------------
void CMotion::MotionCopy(MOTION_TYPE Nowmotion, int NowKey, MOTION_TYPE CopyMotion, int CopyKey)
{
	//範囲外じゃないかどうか
	if (CopyMotion != CMotion::MOTION_NONE && CopyKey != -1)
	{
		//コピー
		for (size_t nCnt = 0; nCnt < m_pMotionInfoList[Nowmotion]->pKeyInfoList[NowKey]->pKeyList.size(); nCnt++)
		{
			m_pMotionInfoList[Nowmotion]->pKeyInfoList[NowKey]->pKeyList[nCnt]->rotDest = m_pMotionInfoList[CopyMotion]->pKeyInfoList[CopyKey]->pKeyList[nCnt]->rotDest;
		}
	}
}
//------------------------------------------------------------------------------
//モーションのファイル名取得
//------------------------------------------------------------------------------
void CMotion::LoadMotionFileName()
{
	//変数宣言
	FILE *pFile;					//ファイルのポインタ
	char cReadText[MAX_TEXT] = {};
	char cHeadText[MAX_TEXT] = {};
	char cDieText[MAX_TEXT] = {};

	//ファイル読み込み
	pFile = fopen(MOTION_FILE_NAME, "r");

	//nullcheck
	if (pFile != nullptr)
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

				//ファイル名
				if (strcmp(cHeadText, "FILENAME") == 0)
				{
					//仮の文字列
					char aName[128] = {};

					//ファイル名代入
					sscanf(cReadText, "%s %s %s %s %s", &cDieText, &cDieText, &aName, &cDieText, &cDieText);

					//配列に追加
					m_apFileName.emplace_back(aName);

					//debug モデル名出力
					std::cout << "Name >> " << aName << NEWLINE;
				}
			}
		}

		//保存成功
		std::cout << "Load Succsess!!  LoadMotionFileName()" << NEWLINE;

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		//ファイルが開けませんでした
		std::cout << "Load FAILED!!  Can't Open File. LoadMotionFileName()" << NEWLINE;
	}
}