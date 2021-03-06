//------------------------------------------------------------------------------
//
//テクスチャ管理  [texture.cpp]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------

#ifndef _TEXTURE_H_
#define _TEXTURE_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
//前方宣言
class CTexture
{
public:
	CTexture() {};
	~CTexture() {};

	//通常のテクスチャ
	enum TEX_TYPE
	{
		TEX_NONE = 0,

		TEX_EFFECT_PARTICLE,					//パーティクル

		//キャラクター
		TEX_PLAYER,								//プレイヤー
		TEX_ENEMY_BLUE,							//敵
		TEX_ENEMY_RED,							//敵
		TEX_ENEMY_PURPLE,						//敵

		TEX_ITEM_BOMB,							//ボム
		TEX_ITEM_MITOKONDORIA,					//ミトコンドリア

		//BG
		TEX_UI_BG_GRIDLINE,						// グリッド線

		//タイトル
		TEX_UI_TITLE_NAME,						// タイトルロゴ

		//ランキング
		TEX_UI_RANKING_NAME,					// ランキングロゴ
		TEX_UI_RANKING_SCORE,					// ランキングスコア
		TEX_UI_RANKING_1st,						// 1位
		TEX_UI_RANKING_2nd,						// 2位
		TEX_UI_RANKING_3rd,						// 3位
		TEX_UI_RANKING_4th,						// 4位
		TEX_UI_RANKING_5th,						// 5位
		TEX_UI_RANKING_GO_TITLE,				// タイトルへ

		TEX_UI_ENTER,							// Enter
		TEX_UI_NUM,								// ナンバー
		TEX_UI_SCORE,							// スコア
		TEX_UI_TIME,							// タイム

		// ゲームUI
		TEX_UI_GAME_CROSS,						// バツ ( かける )
		TEX_UI_GAME_HIGHSCORE,					// ハイスコア
		TEX_UI_GAME_READY,						// Ready
		TEX_UI_GAME_GO,						// Ready

		// リザルトUI
		TEX_UI_RESULT_KILL,						// キル


		TEX_MAX									//MAX
	};

	//アニメーション用のテクスチャ
	//分割されているテクスチャ
	enum SEPARATE_TEX_TYPE
	{
		SEPARATE_TEX_NONE = 0,

		//エフェクト
		SEPARATE_TEX_EFFECT_EXPLOSION00,			//爆発
		SEPARATE_TEX_IMPACT_00,						//インパクト
		SEPARATE_TEX_IMPACT_01,						//インパクト
		SEPARATE_TEX_BOMB_APPEAR,					//爆弾出現


		SEPARATE_TEX_UI_NUMBER,						// ナンバー

		SEPARATE_TEX_MAX							//MAX
	};

	typedef struct
	{
		LPDIRECT3DTEXTURE9 m_apSeparateTexture;		//分割テクスチャへのポインタ
		SEPARATE_TEX_TYPE type;						//テクスチャの種類
		D3DXVECTOR2 m_UVCnt;						//UVの枚数
		D3DXVECTOR2 m_UVSize;						//UVの大きさ

	}SEPARATE_TEX_INFO;

	static void TexLoad(HWND hwnd);						//テクスチャ生成
	static void TexUnload();							//テクスチャ破棄

	static LPDIRECT3DTEXTURE9 GetTexture(TEX_TYPE textype);							//通常テクスチャの取得

	static std::vector<std::string> &GetTexFileName() { return m_aTexFileName; };						//テクスチャ名取得
	static std::vector<std::string> &GetSeparateFileName() { return m_aSeparateTexFileNameList; };		//分割テクスチャ名取得

	static LPDIRECT3DTEXTURE9 GetSeparateTexture(SEPARATE_TEX_TYPE textype);		//分割テクスチャ取得
	static D3DXVECTOR2 GetSparateTex_UVSize(SEPARATE_TEX_TYPE textype);				//分割テクスチャ　UVのサイズ取得
	static D3DXVECTOR2 GetSparateTex_UVCnt(SEPARATE_TEX_TYPE textype);				//分割テクスチャ　UVの枚数取得


private:
	static std::vector<LPDIRECT3DTEXTURE9> m_apTextureList;							//通常テクスチャへのポインタのリスト
	static std::vector<std::string> m_aTexFileName;											//通常テクスチャのファイル名


	static std::vector<std::string> m_aSeparateTexFileNameList;								//分割テクスチャのファイル名
	static std::vector<SEPARATE_TEX_INFO> m_apSeparateTexInfoList;					//分割テクスチャに関する情報のポインタ

};

#endif