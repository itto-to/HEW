//=============================================================================
//
// �X�e�[�W���� [stage.h]
// Author : 
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "collision.h"
#include "game.h"
#include "main.h"


//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct {
	LPDIRECT3DTEXTURE9 texture;		// �e�N�X�`���ǂݍ��ݏꏊ
	LPDIRECT3DVERTEXBUFFER9 vtx;	// ���_�o�b�t�@
	BOUNDING_BOX hitBox;			// �����蔻��p�o�E���f�B���O�{�b�N�X
	BOUNDING_BOX screenBox;			// ��ʊO����p�o�E���f�B���O�{�b�N�X
	D3DXVECTOR3 pos;				// ���݂̈ʒu
	D3DXVECTOR3 move;				// �ړ���
	D3DXVECTOR3 rot;				// ���݂̌���
	D3DXVECTOR3 rotDest;			// �ړI�̌���
} OBSTACLE;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitStage(void);
void UninitStage(void);
void UpdateStage(void);
void DrawStage(void);
OBSTACLE *GetObstacle(void);

#endif