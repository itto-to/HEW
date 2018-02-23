//=============================================================================
//
// �w�i���� [background.cpp]
// Author : 
//
//=============================================================================
#include "background.h"

#include "mesh.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_BACKGROUND		"data/TEXTURE/background.png"


//*****************************************************************************
// �\���̐錾
//*****************************************************************************
typedef struct {
	LPDIRECT3DVERTEXBUFFER9 vtx;	// ���_�o�b�t�@
	LPDIRECT3DTEXTURE9 texture;		// �e�N�X�`���̃|�C���^
	D3DXVECTOR3 pos;				// ���W
} BACKGROUND;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
BACKGROUND g_background;


HRESULT InitBackground()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_background.pos = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 100.0f);

	// ���_�쐬
	MakeVertex(pDevice, g_background.vtx, SCREEN_WIDTH, SCREEN_HEIGHT);

	// �e�N�X�`���쐬
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BACKGROUND, &g_background.texture);

	

	return S_OK;
}

void UninitBackground()
{
	SAFE_RELEASE(g_background.vtx);
	SAFE_RELEASE(g_background.texture);
}

void UpdateBackground()
{
}

void DrawBackground()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxWorld, mtxRot, mtxTranslate;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_background.vtx, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_background.pos.x, g_background.pos.y, g_background.pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_background.texture);

	// �`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}
