//=============================================================================
//
// プレイヤーアクション処理 [player_behavior.h]
// Author : 
//
//=============================================================================
#include "player_behavior.h"

#include "debugproc.h"
#include "input.h"
#include "player.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************



void UpdatePlayerOnGround(PLAYER *player)
{
	// ジャンプ処理
	if (GetKeyboardTrigger(DIK_Z) && player->state != PLAYER_JUMP)
	{
		player->move.y = PLAYER_JUMP_SPEED;
		player->state = PLAYER_JUMP;
	}
	else if (GetKeyboardTrigger(DIK_X) && player->state != PLAYER_JUMP)
	{
		player->move.y = PLAYER_BIG_JUMP_SPEED;
		player->state = PLAYER_JUMP;
	}
	else if (GetKeyboardTrigger(DIK_DOWN))	// スライディング処理
	{


	}
}

void UpdatePlayerJump(PLAYER *player)
{
	player->pos += player->move * player->speed_factor;
	player->move.y += GRAVITY_ACCELARATION * player->speed_factor;					// 重力加速度
}

void UpdatePlayerSliding(PLAYER *player)
{

}

void UpdatePlayerDead(PLAYER *player)
{

}

void EnterPlayerOnGround(PLAYER *player)
{

}

void EnterPlayerJump(PLAYER *player)
{

}

void EnterPlayerSliding(PLAYER *player)
{
	// 当たり判定を小さくする
	player->hitBox = SLIDING_HIT_BOX;
}

void EnterPlayerDead(PLAYER *player)
{

}

void ExitPlayerOnGround(PLAYER *player)
{

}

void ExitPlayerJump(PLAYER *player)
{

}

void ExitPlayerSliding(PLAYER *player)
{
	// 当たり判定を元に戻す
	player->hitBox.max = PLAYER_BB_MAX;
	player->hitBox.min = PLAYER_BB_MIN;
}

void ExitPlayerDead(PLAYER *player)
{

}
