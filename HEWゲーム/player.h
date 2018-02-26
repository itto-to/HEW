//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "collision.h"
#include "main.h"

enum PLAYER_STATE {
	PLAYER_ONGROUND,	// 着地
	PLAYER_JUMP,		// ジャンプ中
	PLAYER_DEAD,		// 死亡
	PLAYER_STATEMAX,
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	// スキル利用の為に追加
	int skillpoint;


	
	bool kengen;				// スキルを発動する権限を持っているかどうか
	LPDIRECT3DTEXTURE9 texture; // テクスチャ読み込み場所
	LPDIRECT3DVERTEXBUFFER9 vtx;
	D3DXVECTOR3 pos;			// 現在の位置
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 rot;			// 現在の向き
	D3DXVECTOR3 rotDest;		// 目的の向き
	D3DXVECTOR3 scl;			// スケール
	float ground;				// 地面の高さ
	float distance;				// 進んだ距離
	int life;					// プレイヤーの体力
	PLAYER_STATE state;			// プレイヤーの状態
	bool is_invincible;			// 無敵状態かどうか
	int invincible_counter;		// 無敵時間のカウント
	BOUNDING_BOX hitBox;		// プレイヤーの当たり判定
	BOUNDING_BOX screenBox;		// 画面外判定用ボックス
} PLAYER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(int no);

D3DXVECTOR3 GetPositionPlayer(void);
D3DXVECTOR3 GetRotationPlayer(void);
D3DXVECTOR3 GetRotationDestPlayer(void);
D3DXVECTOR3 GetMovePlayer(void);
int NumPlayer(void);

#endif
