//=============================================================================
//
// ステージ処理 [stage.h]
// Author : 
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "collision.h"
#include "game.h"
#include "main.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LANE_DATA	(512)

//*****************************************************************************
// 列挙体宣言
//*****************************************************************************
enum STAGE {
	STAGE_PLAIN   = 0,		// 平原ステージ
	STAGE_DESERT  = 1,		// 砂漠ステージ
	STAGE_VOLCANO = 2,		// 火山ステージ
	STAGE_SNOW    = 3,		// 雪山ステージ
	STAGE_MAX     = 4,
};

enum OBSTACLE_TYPE {
	OBSTACLE_JUMP,			// ジャンプ用障害物
	OBSTACLE_LARGE_JUMP,	// 大ジャンプ用障害物
	OBSTACLE_SLIDING,		// スライディング用障害物
	OBSTACLE_MAX,
};

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct {
	bool use;			// プレイヤーが参加していればtrue
	float speed_factor;	// 速度係数 デフォルトは1.0f
	float lane_length;	// レーン1週分の長さ
} LANE;

typedef struct {
	int length;
	char tile[MAX_LANE_DATA];
} LANE_DATA;

typedef struct {
	int lane_no;					// 所属するレーンの番号
	STAGE stage;					// 所属するステージ
	OBSTACLE_TYPE obstacle_type;	// 障害物のタイプ
	LPDIRECT3DTEXTURE9 texture;		// テクスチャ読み込み場所
	LPDIRECT3DVERTEXBUFFER9 vtx;	// 頂点バッファ
	BOUNDING_BOX hit_box;			// 当たり判定用バウンディングボックス
	BOUNDING_BOX screen_box;		// 画面外判定用バウンディングボックス
	D3DXVECTOR3 pos;				// 現在の位置
	D3DXVECTOR3 move;				// 移動量
	D3DXVECTOR3 rot;				// 現在の向き
	D3DXVECTOR3 rotDest;			// 目的の向き
} OBSTACLE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitStage(void);
void UninitStage(void);
void UpdateStage(void);
void DrawStage(void);
OBSTACLE *GetObstacle(void);
LANE *GetLane(int no);

#endif