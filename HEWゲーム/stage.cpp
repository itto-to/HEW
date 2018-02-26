//=============================================================================
//
// �X�e�[�W���� [stage.cpp]
// Author : 
//
//=============================================================================
#include "stage.h"

#include "camera.h"
#include "collision.h"
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
	bool use;			// �v���C���[���Q�����Ă����true
	float speed_factor; // ���x�W�� �f�t�H���g��1.0f
} LANE;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LANE g_lane[MAX_PLAYER];
OBSTACLE g_obstacle;



HRESULT InitStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���[��������
	for (int no = 0; no < MAX_PLAYER; no++)
	{
		g_lane[no].use = true;
		g_lane[no].speed_factor = 1.0f;
	}

	g_obstacle.pos = D3DXVECTOR3(-SCREEN_WIDTH / 2.0f, LANE_Y(0), LANE_Z(0));
	g_obstacle.move = D3DXVECTOR3(10.f, 0.0f, 0.0f);
	g_obstacle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_obstacle.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �����蔻�菉����
	InitBoundingBox(&g_obstacle.hitBox, D3DXVECTOR3(0.0f, 0.0f, 0.0f), OBSTACLE_WIDTH, OBSTACLE_HEIGHT, 0.0f);

	// ��ʊO���菉����
	InitBoundingBox(&g_obstacle.screenBox, D3DXVECTOR3(0.0f, 0.0f, 0.0f), OBSTACLE_WIDTH, OBSTACLE_HEIGHT, 0.0f);

	// ���_�쐬
	MakeVertex(pDevice, &g_obstacle.vtx, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_OBSTACLE,		// �t�@�C���̖��O
		&g_obstacle.texture);	// �ǂݍ��ރ������[


	return S_OK;
}

void UninitStage(void)
{
	SAFE_RELEASE(g_obstacle.vtx);
	SAFE_RELEASE(g_obstacle.texture);
}

void UpdateStage(void)
{
	g_obstacle.pos += g_obstacle.move;

	BOUNDING_BOX worldBox = ToWorldBoundingBox(g_obstacle.hitBox, g_obstacle.pos);
	if (IsObjectOffscreen(worldBox))
	{
		g_obstacle.pos.x = -SCREEN_WIDTH / 2.0f;
	}
}

void DrawStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_obstacle.vtx, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixIdentity(&mtxScl);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_obstacle.rot.y, g_obstacle.rot.x, g_obstacle.rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_obstacle.pos.x, g_obstacle.pos.y, g_obstacle.pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_obstacle.texture);

	// �`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

#ifdef _DEBUG
	BOUNDING_BOX worldBox = ToWorldBoundingBox(g_obstacle.hitBox, g_obstacle.pos);
	DrawDebugBoundingBox(worldBox);
#endif
}

OBSTACLE *GetObstacle(void)
{
	return &g_obstacle;
}