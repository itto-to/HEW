//=============================================================================
//
// �X�e�[�W���� [stage.cpp]
// Author : 
//
//=============================================================================
#include "stage.h"

#include "mesh.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_OBSTACLE "data/TEXTURE/obstacle.png"
#define OBSTACLE_WIDTH	(100)
#define OBSTACLE_HEIGHT	(100)

//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct {
	LPDIRECT3DTEXTURE9 texture; // �e�N�X�`���ǂݍ��ݏꏊ
	LPDIRECT3DVERTEXBUFFER9 vtx;// ���_�o�b�t�@
	D3DXVECTOR3 pos;			// ���݂̈ʒu
	D3DXVECTOR3 move;			// �ړ���
	D3DXVECTOR3 rot;			// ���݂̌���
	D3DXVECTOR3 rotDest;		// �ړI�̌���
} OBSTACLE;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_textureObstacle;
OBSTACLE g_obstacle;


HRESULT InitStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_obstacle.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_obstacle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���_�쐬
	MakeVertex(pDevice, g_obstacle.vtx, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_OBSTACLE,		// �t�@�C���̖��O
		&g_textureObstacle);	// �ǂݍ��ރ������[


	return S_OK;
}

void UninitStage(void)
{
	SAFE_RELEASE(g_obstacle.vtx);

	SAFE_RELEASE(g_textureObstacle);
}

void UpdateStage(void)
{

}

void DrawStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxWorld;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_player.vtx, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureKnight);

	// �`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

}