//------------------------------------------------------------------------------
//
//キャラクター用のモデル処理  [modelcharacter.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODELCHARACTER_H_
#define _MODELCHARACTER_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "sceneX.h"
#include "motion.h"
//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CModelParts;
class CCharacter;

//キャラクターの形になるモデルの情報
class CModelCharacterInfo
{
public:
	std::vector<std::shared_ptr<CModelInfo>> pModelInfoList;	//モデルの描画に必要な情報
	std::vector<int> nWeaponID;									//武器番号
};

//キャラクターのモデル
class CModelCharacter
{
public:
	CModelCharacter();
	~CModelCharacter();

	//モデルの種類
	enum CHARACTER_TYPE
	{
		CHARACTER_NONE = -1,	//
		CHARACTER_PLAYER,		//プレイヤー
		CHARACTER_ENEMY,		//敵
		CHARACTER_MAX
	};

	HRESULT Init();																//初期化
	void Uninit();																//終了
	void Update();																//更新
	void Draw();																//描画
	static std::unique_ptr<CModelCharacter> Create(CHARACTER_TYPE const modeltype);	//生成処理
	static HRESULT Load();														//モデル読み込み
	static void UnLoad();														//モデル開放

	void ShowModelCharacterInfo();												//モデルの情報表示

	//Set関数
	void SetCharacterMtx(D3DXMATRIX *mtx);										//親マトリックス設定関数
	void SetMotion(CMotion::MOTION_TYPE motiontype);							//モーションセット
	void SetDefaultMotion();													//デフォルトのモーションセット

	void SetFrame(int nFrame)		{ m_nFrame = nFrame; };						//フレーム数
	void SetKey(int nKey)			{ m_nKey = nKey; };							//キー数
	void ResetMotion()				{ m_nFrame = 0; m_nKey = 0; };				//モーションの情報の初期化
	void SetMotionStop(bool bStop)	{ m_bMotionStop = bStop; };					//モーションストップ設定

	//Get関数
	int &GetFrame()							{ return m_nFrame; };				//フレーム数
	int &GetKey()							{ return m_nKey; };					//キー数
	CHARACTER_TYPE &GetType()				{ return m_type; };					//モデルの種類取得
	CMotion::MOTION_TYPE &GetMotion()		{ return m_motion; };				//現在のモーション情報取得
	bool &GetMotionStop()					{ return m_bMotionStop; };			//モーションストップ取得

	void KeyCheck();															//キーチェック
	void FrameCheck();															//フレームチェック
	void ForcedUpdate(CMotion::MOTION_TYPE motiontype, int nKey);				//強制的にモーションチェンジ
	bool CheckMoveKey();														//移動するキーか確認
	float GetWaistHeight();														//腰の高さ取得

	HRESULT SaveModelOffset();													//オフセット情報保存

	static int GetModelNum(CHARACTER_TYPE const modeltype)							{return  m_pModelCharacterList[modeltype]->pModelInfoList.size();};							//モデル数取得

	CModelParts *GetPartsPtr(int nIndex);
protected:
private:
	static std::vector<std::unique_ptr<CModelCharacterInfo>> m_pModelCharacterList;	//全てのモデルキャラクタの情報を格納
	std::vector<std::unique_ptr<CModelParts>> m_pModelPartsList;				//モデルのパーツのポインタ

	CHARACTER_TYPE m_type;															//モデルの種類
	D3DXMATRIX *m_CharacterMtx;													//キャラクターのマトリックス
	CMotion::MOTION_TYPE m_motion;												//現在のモーション
	CMotion::MOTION_TYPE m_motionOld;											//1F前のモーション
	int m_nFrame;																//現在のフレーム
	int m_nKey;																	//現在のキー
	bool m_bMotionStop;															//モーションストップ

	HRESULT LoadModelRelativeValue(CHARACTER_TYPE const modeltype);				//モデルの相対座標読み込み
	static HRESULT LoadModelName(CHARACTER_TYPE const modeltype);				//モデル名読み込み

	static char *m_aFileName[CHARACTER_MAX];
};
#endif