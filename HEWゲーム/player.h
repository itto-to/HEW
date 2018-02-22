//=============================================================================
//
// �v���C���[���� [player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

enum PLAYER_STATE {
	PLAYER_ONGROUND,	// ���n
	PLAYER_JUMP,		// �W�����v��
	PLAYER_STATEMAX,
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	bool kengen;				// �X�L���𔭓����錠���������Ă��邩�ǂ���
	LPDIRECT3DTEXTURE9 texture; // �e�N�X�`���ǂݍ��ݏꏊ
	LPDIRECT3DVERTEXBUFFER9 vtx;
	D3DXVECTOR3 pos;			// ���݂̈ʒu
	D3DXVECTOR3 move;			// �ړ���
	D3DXVECTOR3 rot;			// ���݂̌���
	D3DXVECTOR3 rotDest;		// �ړI�̌���
	float ground;				// �n�ʂ̍���
	float distance;				// �i�񂾋���
	PLAYER_STATE state;			// �v���C���[�̏��
} PLAYER;

//*****************************************************************************
// �v���g�^�C�v�錾
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
