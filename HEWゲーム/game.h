//=============================================================================
//
// ゲーム画面処理 [game.h]
// Author : 
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LANE_Y(n)	((100 - n * 200.0f))				// nレーンのY座標
#define LANE_Z(n)	((MAX_PLAYER - n + 1) * 10.0f)		// nレーンのZ座標


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

#endif
