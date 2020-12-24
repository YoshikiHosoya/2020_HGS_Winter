//------------------------------------------------------------------------------
//
//背景の処理  [bg.h]
//Author:Yoshiki Hosoya
//
//------------------------------------------------------------------------------
#ifndef _BG_H_
#define _BG_H_
//------------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//------------------------------------------------------------------------------
//マクロ
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//クラス定義
//------------------------------------------------------------------------------
class CScene2D;

class CBg : public CScene
{
public:
	enum class BG_FIGURE
	{
		BG_NONE = -1, 
		BG_Circle,			// まる
		BG_Triangle,		// 三角
		BG_Square,			// 四角
		BG_Star,			// 星
		BG_Cross,			// ばつ
		BG_MAX,
	};

	enum class BG_FIGURE_SIZE
	{
		NONE = -1,
		SMALL,			// 小
		MEDIUM,			// 中
		BIG,			// 大
		FIGURE_SIZE_MAX,
	};

	CBg();
	~CBg();

	HRESULT Init();						//初期化
	void Update();						//更新
	void Draw();						//描画
	void ShowDebugInfo() {};			//デバッグ情報表記

	static std::unique_ptr<CBg> Create(int DropNum);			//生成処理

private:
	void	RandomFigure();						// ランダム図形の生成

	std::vector<std::shared_ptr<CScene2D>>		m_apScene2D;						// 背景
	std::shared_ptr<CScene2D>					m_pGridLine;						// グリッド線

	D3DXVECTOR3			m_FigureSize;			// 図形のサイズ
	D3DXVECTOR3			m_FigureMove;			// 図形の移動量
	BG_FIGURE_SIZE		m_FigureSizeType;		// 図形のサイズの種類
	BG_FIGURE			m_FigureType;			// 図形の種類
	int					m_nCntScroll;			// スクロールカウント
	int					m_DropRangeNum;			// 落ちる許容範囲
};

#endif