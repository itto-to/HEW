//=============================================================================
//
// �v���C���[�A�N�V�������� [player_behavior.h]
// Author : 
//
//=============================================================================
#include "player_behavior.h"

#include "debugproc.h"
#include "input.h"
#include "player.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************



void UpdatePlayerOnGround(PLAYER *player)
{
	// �W�����v����
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
	else if (GetKeyboardTrigger(DIK_DOWN))	// �X���C�f�B���O����
	{


	}
}

void UpdatePlayerJump(PLAYER *player)
{
	player->pos += player->move * player->speed_factor;
	player->move.y += GRAVITY_ACCELARATION * player->speed_factor;					// �d�͉����x
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
	// �����蔻�������������
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
	// �����蔻������ɖ߂�
	player->hitBox.max = PLAYER_BB_MAX;
	player->hitBox.min = PLAYER_BB_MIN;
}

void ExitPlayerDead(PLAYER *player)
{

}
