//------------------------------------------------------------------------------
//
//モーション処理  [motion.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MOTION_H_
#define _MOTION_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include <vector>

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModelCharacter;

class CMotion
{
public:
	//キー
	struct KEY
	{
		D3DXVECTOR3 rotDest;	//rotの行先

		//コンストラクタ
		KEY()
		{
			//初期化
			rotDest = ZeroVector3;
		}
	};
	//キー情報
	struct KEY_INFO
	{
		std::vector<std::unique_ptr<KEY>> pKeyList;		//keyのポインタ
		int nFrame;										//フレーム数
		float fOriginHeight;							//原点の高さ

		//コンストラクタ
		KEY_INFO()
		{
			//初期化
			fOriginHeight = 0.0f;
			nFrame = 30;
		}
	};
	//攻撃関係の情報
	struct ATTACK_INFO
	{
		int nAttack;			//攻撃力
		int nBlueAttack;		//青攻撃力
		D3DXCOLOR orbitcol;		//軌跡の色
		float fLength;			//コリジョンの判定の長さ
		float fRadius;			//コリジョンの半径

		int nStartCollisonKey;	//コリジョン開始のキー
		int nEndCollisionKey;	//コリジョン終了のキー

		int nMoveKey;			//移動する時のキー
		float fMoveFront;		//向いてる方向に対する移動量
		float fMoveUp;			//上下の移動量

		float fKnockBack_Front;	//前後のノックバック
		float fKnockBack_Up;	//上下のノックバック

		//コンストラクタ
		ATTACK_INFO()
		{
			//初期化
			nAttack = 0;
			nBlueAttack = 0;
			orbitcol = WhiteColor;
			fLength = 0.0f;
			fRadius = 0.0f;
			nStartCollisonKey = 0;
			nEndCollisionKey = 0;
			nMoveKey = 0;
			fMoveFront = 0.0f;
			fMoveUp = 0.0f;
			fKnockBack_Front = 0.0f;
			fKnockBack_Up = 0.0f;
		}
	};
	//モーションの情報
	struct MOTION_INFO
	{
		std::vector<std::unique_ptr<KEY_INFO>> pKeyInfoList;	//keyInfoへのポインタ
		int nNumKey;											//キー数
		bool bLoop;												//ループするかどうか
		bool bAttack;											//攻撃かどうか
		std::unique_ptr<ATTACK_INFO> pAttackInfo;				//攻撃に関する情報

		//コンストラクタ
		MOTION_INFO()
		{
			//初期化
			nNumKey = 0;
			bLoop = false;
			bAttack = false;
		}
	};

	//モーション
	enum MOTION_TYPE
	{
		MOTION_NONE = -1,				//無し

		PLAYER_NEUTRAL,					//プレイヤー　ニュートラル
		PLAYER_WALK,					//プレイヤー　歩く
		PLAYER_DASH,					//プレイヤー　走る
		PLAYER_JUMP,					//プレイヤー　ジャンプ
		PLAYER_HIT,						//プレイヤー　被弾
		PLAYER_DEATH,					//プレイヤー　死亡
		MOTION_MAX						//最大値
	};

	CMotion();
	~CMotion();

	static HRESULT Load();			//モーション情報読み込み
	static void UnLoad();			//モーション情報破棄
	static void ShowDebugInfo();	//デバッグの情報

	static void AddKeyInfo(MOTION_TYPE NowMotion, int model);		//キー数追加
	static void PopbackKeyInfo(MOTION_TYPE NowMotion);					//キー数減少

	//Get関数
	static D3DXVECTOR3 GetRotDest(MOTION_TYPE NowMotion,int nKey,int nIndex)	{ return m_pMotionInfoList[NowMotion]->pKeyInfoList[nKey]->pKeyList[nIndex]->rotDest; };		//モーションの回転のゴール取得
	static float GetOriginHeight(MOTION_TYPE NowMotion, int nKey)				{ return m_pMotionInfoList[NowMotion]->pKeyInfoList[nKey]->fOriginHeight; };					//モーションの腰の高さ取得
	static int GetFrame(MOTION_TYPE NowMotion, int nKey)						{ return m_pMotionInfoList[NowMotion]->pKeyInfoList[nKey]->nFrame; };							//モーションのフレーム数取得
	static int GetNumKey(MOTION_TYPE NowMotion)									{ return m_pMotionInfoList[NowMotion]->nNumKey; };												//モーションのキー数取得
	static bool CheckLoop(MOTION_TYPE NowMotion)								{ return m_pMotionInfoList[NowMotion]->bLoop; };												//モーションのループするかどうか取得
	static bool CheckAttack(MOTION_TYPE NowMotion)								{ return m_pMotionInfoList[NowMotion]->bAttack; };												//モーションが攻撃かどうかだけ返す
	static ATTACK_INFO *GetAttackInfo(MOTION_TYPE NowMotion)					{ return m_pMotionInfoList[NowMotion]->pAttackInfo.get(); };									//攻撃情報読み込み
	static MOTION_INFO *GetMotion(MOTION_TYPE NowMotion)						{ return m_pMotionInfoList[NowMotion].get(); };													//モーション情報取得
	static std::vector<std::string> &GetFileName()								{ return m_apFileName; };																		//ファイル名取得

	static bool CheckAttackKey(MOTION_TYPE NowMotion, int nNowKey);			//攻撃かどうか

	static HRESULT SaveMotion(MOTION_TYPE motiontype);													//モーション保存
	static void MotionCopy(MOTION_TYPE Nowmotion, int NowKey, MOTION_TYPE CopyMotion, int CopyKey);		//モーションコピー
	static void LoadMotionFileName();																	//モーションのファイル名読み込み

private:
	static std::vector<std::unique_ptr<MOTION_INFO>> m_pMotionInfoList;		//モーションの情報
	static HRESULT LoadMotion(MOTION_TYPE motiontype);						//モーション読み込み

	static std::vector<std::string> m_apFileName;							//ファイル名
};
#endif