//------------------------------------------------------------------------------
//
//モデル情報管理  [modelinfo.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _MODELINFO_H_
#define _MODELINFO_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
//前方宣言
class CModelInfo
{
public:
	CModelInfo();
	~CModelInfo();

	enum MODEL_TYPE
	{
		MODEL_NONE = -1,

		//マップオブジェクト 0番台
		MODEL_MAPOBJECT_WALL,					//オブジェクト　壁
		MODEL_MAPOBJECT_BLOCK,					//オブジェクト　ブロック
		MODEL_MAPOBJECT_SPHERE,					//オブジェクト　スフィア
		//プレイヤーパーツ 100番台
		MODEL_PARTS_PLAYER_WAIST = 100,			//プレイヤー　腰
		MODEL_PARTS_PLAYER_BODY,				//プレイヤー　体
		MODEL_PARTS_PLAYER_HEAD,				//プレイヤー　頭
		MODEL_PARTS_PLAYER_R_SHOULDER,			//プレイヤー　右腕
		MODEL_PARTS_PLAYER_R_ARM,				//プレイヤー　右肘から
		MODEL_PARTS_PLAYER_R_HAND,				//プレイヤー　右手
		MODEL_PARTS_PLAYER_L_SHOULDER,			//プレイヤー　左腕
		MODEL_PARTS_PLAYER_L_ARM,				//プレイヤー　左肘から先
		MODEL_PARTS_PLAYER_L_HAND,				//プレイヤー　左手
		MODEL_PARTS_PLAYER_R_KNEE,				//プレイヤー　右腿
		MODEL_PARTS_PLAYER_R_LEG,				//プレイヤー　右脚
		MODEL_PARTS_PLAYER_R_FOOT,				//プレイヤー　右足
		MODEL_PARTS_PLAYER_L_KNEE,				//プレイヤー　左腿
		MODEL_PARTS_PLAYER_L_LEG,				//プレイヤー　左脚
		MODEL_PARTS_PLAYER_L_FOOT,				//プレイヤー　左足

		//敵のパーツ 200番台
		MODEL_PARTS_ENEMY_WAIST = 200,			//敵　腰
		MODEL_PARTS_ENEMY_BODY,					//敵　体
		MODEL_PARTS_ENEMY_HEAD,					//敵　頭
		MODEL_PARTS_ENEMY_R_SHOULDER,			//敵　右腕
		MODEL_PARTS_ENEMY_R_ARM,				//敵　右肘から先
		MODEL_PARTS_ENEMY_R_HAND,				//敵　右手
		MODEL_PARTS_ENEMY_L_SHOULDER,			//敵　左腕
		MODEL_PARTS_ENEMY_L_ARM,				//敵　左肘から先
		MODEL_PARTS_ENEMY_L_HAND,				//敵　左手
		MODEL_PARTS_ENEMY_R_KNEE,				//敵　右腿
		MODEL_PARTS_ENEMY_R_LEG,				//敵　右脚
		MODEL_PARTS_ENEMY_R_FOOT,				//敵　右足
		MODEL_PARTS_ENEMY_L_KNEE,				//敵　左腿
		MODEL_PARTS_ENEMY_L_LEG,				//敵　左脚
		MODEL_PARTS_ENEMY_L_FOOT,				//敵　左足

		//武器 900番台
		MODEL_WEAPONPARTS_LONG_SWORD = 900,		//長剣　プレイヤー用
		MODEL_WEAPONPARTS_SHORT_SWORD,			//ちょっと短い剣　敵用
	};


	typedef struct MODELNAME
	{
		std::string modelname;
		MODEL_TYPE modeltype;
	}MODELNAME;

	static void ModelLoad(HWND hwnd);			//モデル生成
	static void ModelUnload();					//モデル生成

	static std::shared_ptr<CModelInfo> GetModelInfo(CModelInfo::MODEL_TYPE modeltype);			//モデル情報取得

	//Get関数
	std::string GetName() { return m_aName; };				//モデル名
	LPD3DXMESH GetMesh() { return m_mesh; };				//メッシュ情報
	LPD3DXBUFFER GetMatBuff() { return m_matBuff; };		//頂点情報
	DWORD GetMatNum() { return m_matNum; };					//マテリアル数
	MODEL_TYPE GetModelType() { return m_modeltype; };		//モデルタイプ

private:
	static std::vector<std::shared_ptr<CModelInfo>> m_apModelList;		//モデルへのポインタのリスト
	static std::vector<MODELNAME> m_aModelFileNameList;				//モデルのファイル名

	MODEL_TYPE		m_modeltype;		//モデルの種類
	std::string		m_aName;			//モデル名
	LPD3DXMESH		m_mesh;				//メッシュ
	LPD3DXBUFFER	m_matBuff;			//頂点情報
	DWORD			m_matNum;			//マテリアル数

};

#endif


