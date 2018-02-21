//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

enum PLAYER_STATE {
	PLAYER_ONGROUND,	// 着地
	PLAYER_JUMP,		// ジャンプ中
	PLAYER_STATEMAX,
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 texture; // テクスチャ読み込み場所
	LPDIRECT3DVERTEXBUFFER9 vtx;
	D3DXVECTOR3 pos;			// 現在の位置
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 rot;			// 現在の向き
	D3DXVECTOR3 rotDest;		// 目的の向き
	float ground;				// 地面の高さ
	float distance;				// 進んだ距離
	PLAYER_STATE state;			// プレイヤーの状態
} PLAYER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);

D3DXVECTOR3 GetPositionPlayer(void);
D3DXVECTOR3 GetRotationPlayer(void);
D3DXVECTOR3 GetRotationDestPlayer(void);
D3DXVECTOR3 GetMovePlayer(void);

#endif
