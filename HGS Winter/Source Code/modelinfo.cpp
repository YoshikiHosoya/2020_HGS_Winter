//-----------------------------------------------------------------------------
//
//モデル情報管理  [modelinfo.cpp]
// Author : Yoshiki Hosoya
//
//-----------------------------------------------------------------------------
#include "modelinfo.h"
#include "manager.h"
#include "renderer.h"
//-----------------------------------------------------------------------------
//静的メンバ変数の初期化
//-----------------------------------------------------------------------------
std::vector<std::shared_ptr<CModelInfo>> CModelInfo::m_apModelList = {};

//テクスチャ名の設定
std::vector<CModelInfo::MODELNAME> CModelInfo::m_aModelFileNameList =
{
	{ "data/MODEL/Map/Wall.x" ,				MODEL_MAPOBJECT_WALL },					//壁
	{ "data/MODEL/Map/Block.x" ,			MODEL_MAPOBJECT_BLOCK },				//壁
	{ "data/MODEL/Map/Sphere.x" ,			MODEL_MAPOBJECT_SPHERE },				//壁

	{ "data/MODEL/Woman/00_waist.x" ,		MODEL_PARTS_PLAYER_WAIST },				//プレイヤー　腰
	{ "data/MODEL/Woman/01_body.x" ,		MODEL_PARTS_PLAYER_BODY },				//プレイヤー　体
	{ "data/MODEL/Woman/02_head.x" ,		MODEL_PARTS_PLAYER_HEAD },				//プレイヤー　頭
	{ "data/MODEL/Woman/03_R_Shoulder.x" ,	MODEL_PARTS_PLAYER_R_SHOULDER },		//プレイヤー　右腕
	{ "data/MODEL/Woman/04_R_Arm.x" ,		MODEL_PARTS_PLAYER_R_ARM },				//プレイヤー　右肘から先
	{ "data/MODEL/Woman/05_R_Hand.x" ,		MODEL_PARTS_PLAYER_R_HAND },			//プレイヤー　右手
	{ "data/MODEL/Woman/06_L_Shoulder.x" ,	MODEL_PARTS_PLAYER_L_SHOULDER },		//プレイヤー　左腕
	{ "data/MODEL/Woman/07_L_Arm.x" ,		MODEL_PARTS_PLAYER_L_ARM },				//プレイヤー　左肘から先
	{ "data/MODEL/Woman/08_L_Hand.x" ,		MODEL_PARTS_PLAYER_L_HAND },			//プレイヤー　左手
	{ "data/MODEL/Woman/09_R_Knee.x",		MODEL_PARTS_PLAYER_R_KNEE },			//プレイヤー　右腿
	{ "data/MODEL/Woman/10_R_Leg.x" ,		MODEL_PARTS_PLAYER_R_LEG },				//プレイヤー　右脚
	{ "data/MODEL/Woman/11_R_Foot.x" ,		MODEL_PARTS_PLAYER_R_FOOT },			//プレイヤー　右足
	{ "data/MODEL/Woman/12_L_Knee.x" ,		MODEL_PARTS_PLAYER_L_KNEE },			//プレイヤー　左腿
	{ "data/MODEL/Woman/13_L_Leg.x" ,		MODEL_PARTS_PLAYER_L_LEG },				//プレイヤー　左脚
	{ "data/MODEL/Woman/14_L_Foot.x" ,		MODEL_PARTS_PLAYER_L_FOOT },			//プレイヤー　左足

	{ "data/MODEL/Armor/00_waist.x" ,		MODEL_PARTS_ENEMY_WAIST },				//敵　腰
	{ "data/MODEL/Armor/01_body.x" ,		MODEL_PARTS_ENEMY_BODY },				//敵　体
	{ "data/MODEL/Armor/02_head.x" ,		MODEL_PARTS_ENEMY_HEAD },				//敵　頭
	{ "data/MODEL/Armor/03_R_Shoulder.x" ,	MODEL_PARTS_ENEMY_R_SHOULDER },			//敵　右腕
	{ "data/MODEL/Armor/04_R_Arm.x" ,		MODEL_PARTS_ENEMY_R_ARM },				//敵　右肘から先
	{ "data/MODEL/Armor/05_R_Hand.x" ,		MODEL_PARTS_ENEMY_R_HAND },				//敵　右手
	{ "data/MODEL/Armor/06_L_Shoulder.x" ,	MODEL_PARTS_ENEMY_L_SHOULDER },			//敵　左腕
	{ "data/MODEL/Armor/07_L_Arm.x" ,		MODEL_PARTS_ENEMY_L_ARM },				//敵　左肘から先
	{ "data/MODEL/Armor/08_L_Hand.x" ,		MODEL_PARTS_ENEMY_L_HAND },				//敵　左手
	{ "data/MODEL/Armor/09_R_Knee.x",		MODEL_PARTS_ENEMY_R_KNEE },				//敵　右腿
	{ "data/MODEL/Armor/10_R_Leg.x" ,		MODEL_PARTS_ENEMY_R_LEG },				//敵　右脚
	{ "data/MODEL/Armor/11_R_Foot.x" ,		MODEL_PARTS_ENEMY_R_FOOT },				//敵　右足
	{ "data/MODEL/Armor/12_L_Knee.x" ,		MODEL_PARTS_ENEMY_L_KNEE },				//敵　左腿
	{ "data/MODEL/Armor/13_L_Leg.x" ,		MODEL_PARTS_ENEMY_L_LEG },				//敵　左脚
	{ "data/MODEL/Armor/14_L_Foot.x" ,		MODEL_PARTS_ENEMY_L_FOOT },				//敵　左足

	{ "data/MODEL/Woman/15_weapon.x" ,	MODEL_WEAPONPARTS_LONG_SWORD },				//武器
	{ "data/MODEL/Armor/15_weapon.x" ,	MODEL_WEAPONPARTS_SHORT_SWORD },			//武器
};

//-----------------------------------------------------------------------------
//コンストラクタ
//-----------------------------------------------------------------------------
CModelInfo::CModelInfo()
{
	//初期化
	m_aName = {};
	m_matBuff = nullptr;
	m_mesh = nullptr;
	m_matNum = 0;
};

//-----------------------------------------------------------------------------
//デストラクタ
//-----------------------------------------------------------------------------
CModelInfo::~CModelInfo()
{
	//バッファ開放
	if (m_matBuff)
	{
		m_matBuff->Release();
		m_matBuff = nullptr;
	}
	//メッシュ開放
	if (m_mesh)
	{
		m_mesh->Release();
		m_mesh = nullptr;
	}
}

//-----------------------------------------------------------------------------
//モデル読み込み
//----------------------------------------------------------------------------
void CModelInfo::ModelLoad(HWND hwnd)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (size_t nCnt = 0; nCnt < m_aModelFileNameList.size(); nCnt++)
	{
		//debug
		std::cout << "Model Load - " << nCnt << m_aModelFileNameList[nCnt].modelname.data() << NEWLINE;

		m_apModelList.emplace_back(std::make_shared<CModelInfo>());
		m_apModelList[nCnt]->m_modeltype = m_aModelFileNameList[nCnt].modeltype;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(m_aModelFileNameList[nCnt].modelname.data(),
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_apModelList[nCnt]->m_matBuff,
			NULL,
			&m_apModelList[nCnt]->m_matNum,
			&m_apModelList[nCnt]->m_mesh)))
		{
			//失敗したとき
			std::cout << "失敗ンゴ" << NEWLINE;
		}
	}
}

//-----------------------------------------------------------------------------
//モデル破棄
//----------------------------------------------------------------------------
void CModelInfo::ModelUnload()
{

}

//-----------------------------------------------------------------------------
//モデル取得
//----------------------------------------------------------------------------
std::shared_ptr<CModelInfo> CModelInfo::GetModelInfo(CModelInfo::MODEL_TYPE modeltype)
{
	//サイズ分繰り返す
	for (size_t nCnt = 0; nCnt < m_apModelList.size(); nCnt++)
	{
		//nullcheck
		if (m_apModelList[nCnt])
		{
			//モデルタイプが一致しているかどうか
			if (m_apModelList[nCnt]->GetModelType() == modeltype)
			{
				return m_apModelList[nCnt];
			}
		}
	}
	return nullptr;
}
