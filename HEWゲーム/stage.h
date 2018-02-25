//=============================================================================
//
// ステージ処理 [stage.h]
// Author : 
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "collision.h"
#include "main.h"

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct {
	LPDIRECT3DTEXTURE9 texture;		// テクスチャ読み込み場所
	LPDIRECT3DVERTEXBUFFER9 vtx;	// 頂点バッファ
	BOUNDING_BOX hitBox;			// 当たり判定用バウンディングボックス
	BOUNDING_BOX screenBox;			// 画面外判定用バウンディングボックス
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

#endif