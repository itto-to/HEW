//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"

#include "debugproc.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "item.h"
#include "score.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_PLAYER_KNIGHT		"data/TEXTURE/player_knight.png"	// �R�m�e�N�X�`����
#define TEXTURE_PLAYER_THIEF		"data/TEXTURE/player_thief.png"		// �����e�N�X�`����
#define TEXTURE_PLAYER_COOK			"data/TEXTURE/player_cook.png"		// �����l�e�N�X�`����
#define TEXTURE_PLAYER_WIZARD		"data/TEXTURE/player_wizard.png"	// ���@�g���e�N�X�`����

#define PLAYER_WIDTH		(100)
#define PLAYER_HEIGHT		(100)
#define HALF_PLAYER_WIDTH	(PLAYER_WIDTH / 2)
#define HALF_PLAYER_HEIGHT	(PLAYER_HEIGHT / 2)

#define	PLAYER_RADIUS		(10.0f)						// ���a
#define	VALUE_MOVE_PLAYER	(0.155f)					// �ړ����x
#define	RATE_MOVE_PLAYER	(0.025f)					// �ړ������W��
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.025f)			// ��]���x
#define	RATE_ROTATE_PLAYER	(0.10f)						// ��]�����W��
#define	VALUE_MOVE_BULLET	(7.5f)						// �e�̈ړ����x
#define MAX_PLAYER			(4)							// �v���C���[�̐�
#define PLAYER_JUMP_SPEED	(15.f)						// �v���C���[�̃W�����v�X�s�[�h
#define GRAVITY_ACCELARATION (-0.5f)						// �d�͉����x


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice, PLAYER *player);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureKnight;		// �e�N�X�`���ǂݍ��ݏꏊ

PLAYER				g_playerWk[MAX_PLAYER];					// �v���C���[���[�N

char *player_textureFileName[MAX_PLAYER] =
{
	TEXTURE_PLAYER_KNIGHT,
	TEXTURE_PLAYER_THIEF,
	TEXTURE_PLAYER_COOK,
	TEXTURE_PLAYER_WIZARD,

};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int no = 0; no < MAX_PLAYER; no++)
	{
		g_playerWk[no].texture = NULL;

		g_playerWk[MAX_PLAYER].pos = D3DXVECTOR3(0.0f, 0.0f + (no * PLAYER_RADIUS), 0.0f);
		g_playerWk[MAX_PLAYER].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_playerWk[MAX_PLAYER].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_playerWk[MAX_PLAYER].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_playerWk[MAX_PLAYER].ground = g_playerWk[MAX_PLAYER].pos.y;
		g_playerWk[MAX_PLAYER].state = PLAYER_ONGROUND;

		// ���_�쐬
		MakeVertexPlayer(pDevice, &g_playerWk[MAX_PLAYER]);


		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_PLAYER_KNIGHT,		// �t�@�C���̖��O
			&g_pD3DTextureKnight);	// �ǂݍ��ރ������[

	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	for(int no = 0; no < MAX_PLAYER; no++)
	{
		// �e�N�X�`���̊J��
		SAFE_RELEASE(g_playerWk[MAX_PLAYER].texture);

		// ���b�V���̊J��
		SAFE_RELEASE(g_playerWk[MAX_PLAYER].vtx);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	if (g_playerWk[MAX_PLAYER].state == PLAYER_JUMP) {
		g_playerWk[MAX_PLAYER].pos += g_playerWk[MAX_PLAYER].move;
		g_playerWk[MAX_PLAYER].move.y += GRAVITY_ACCELARATION;
	}

	// �W�����v����
	if (GetKeyboardTrigger(DIK_Z) && g_playerWk[MAX_PLAYER].state != PLAYER_JUMP) {
		g_playerWk[MAX_PLAYER].move.y = PLAYER_JUMP_SPEED;
		g_playerWk[MAX_PLAYER].state = PLAYER_JUMP;
	}

	// ���n����
	if (g_playerWk[MAX_PLAYER].pos.y < g_playerWk[MAX_PLAYER].ground) {
		g_playerWk[MAX_PLAYER].pos.y = g_playerWk[MAX_PLAYER].ground;
		g_playerWk[MAX_PLAYER].state = PLAYER_ONGROUND;	// �v���C���[�̏�Ԃ𒅒n����
	}

	//float fDiffRotY;

	//// �ړI�̊p�x�܂ł̍���
	//fDiffRotY = g_playerWk[MAX_PLAYER].rotDest.y - g_playerWk[MAX_PLAYER].rot.y;
	//if(fDiffRotY > D3DX_PI)
	//{
	//	fDiffRotY -= D3DX_PI * 2.0f;
	//}
	//if(fDiffRotY < -D3DX_PI)
	//{
	//	fDiffRotY += D3DX_PI * 2.0f;
	//}

	//// �ړI�̊p�x�܂Ŋ�����������
	//g_playerWk[MAX_PLAYER].rot.y += fDiffRotY * RATE_ROTATE_PLAYER;
	//if(g_playerWk[MAX_PLAYER].rot.y > D3DX_PI)
	//{
	//	g_playerWk[MAX_PLAYER].rot.y -= D3DX_PI * 2.0f;
	//}
	//if(g_playerWk[MAX_PLAYER].rot.y < -D3DX_PI)
	//{
	//	g_playerWk[MAX_PLAYER].rot.y += D3DX_PI * 2.0f;
	//}

	///// �ʒu�ړ�
	//g_playerWk[MAX_PLAYER].pos.x += g_playerWk[MAX_PLAYER].move.x;
	//g_playerWk[MAX_PLAYER].pos.y += g_playerWk[MAX_PLAYER].move.y;
	//g_playerWk[MAX_PLAYER].pos.z += g_playerWk[MAX_PLAYER].move.z;

	//if(g_playerWk[MAX_PLAYER].pos.x < -630.0f)
	//{
	//	g_playerWk[MAX_PLAYER].pos.x = -630.0f;
	//}
	//if(g_playerWk[MAX_PLAYER].pos.x > 630.0f)
	//{
	//	g_playerWk[MAX_PLAYER].pos.x = 630.0f;
	//}
	//if(g_playerWk[MAX_PLAYER].pos.y < 10.0f)
	//{
	//	g_playerWk[MAX_PLAYER].pos.y = 10.0f;
	//}
	//if(g_playerWk[MAX_PLAYER].pos.y > 150.0f)
	//{
	//	g_playerWk[MAX_PLAYER].pos.y = 150.0f;
	//}
	//if(g_playerWk[MAX_PLAYER].pos.z > 630.0f)
	//{
	//	g_playerWk[MAX_PLAYER].pos.z = 630.0f;
	//}
	//if(g_playerWk[MAX_PLAYER].pos.z < -630.0f)
	//{
	//	g_playerWk[MAX_PLAYER].pos.z = -630.0f;
	//}

	//// �ړ��ʂɊ�����������
	//g_playerWk[MAX_PLAYER].move.x += (0.0f - g_playerWk[MAX_PLAYER].move.x) * RATE_MOVE_PLAYER;
	//g_playerWk[MAX_PLAYER].move.y += (0.0f - g_playerWk[MAX_PLAYER].move.y) * RATE_MOVE_PLAYER;
	//g_playerWk[MAX_PLAYER].move.z += (0.0f - g_playerWk[MAX_PLAYER].move.z) * RATE_MOVE_PLAYER;



	//if((g_playerWk[MAX_PLAYER].move.x * g_playerWk[MAX_PLAYER].move.x
	//+ g_playerWk[MAX_PLAYER].move.y * g_playerWk[MAX_PLAYER].move.y
	//+ g_playerWk[MAX_PLAYER].move.z * g_playerWk[MAX_PLAYER].move.z) > 1.0f)
	//{
	//	D3DXVECTOR3 pos;

	//	pos.x = g_playerWk[MAX_PLAYER].pos.x + sinf(g_playerWk[MAX_PLAYER].rot.y) * 10.0f;
	//	pos.y = g_playerWk[MAX_PLAYER].pos.y + 2.0f;
	//	pos.z = g_playerWk[MAX_PLAYER].pos.z + cosf(g_playerWk[MAX_PLAYER].rot.y) * 10.0f;

	//	// �G�t�F�N�g�̐ݒ�
	//	SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	//						D3DXCOLOR(0.85f, 0.05f, 0.65f, 0.50f), 14.0f, 14.0f, 20);
	//	SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	//						D3DXCOLOR(0.65f, 0.85f, 0.05f, 0.30f), 10.0f, 10.0f, 20);
	//	SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	//						D3DXCOLOR(0.45f, 0.45f, 0.05f, 0.15f), 5.0f, 5.0f, 20);
	//}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxWorld;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_playerWk[MAX_PLAYER].vtx, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_playerWk[MAX_PLAYER].rot.y, g_playerWk[MAX_PLAYER].rot.x, g_playerWk[MAX_PLAYER].rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_playerWk[MAX_PLAYER].pos.x, g_playerWk[MAX_PLAYER].pos.y, g_playerWk[MAX_PLAYER].pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureKnight);

	// �`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}


//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice, PLAYER *player)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&player->vtx,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		player->vtx->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-HALF_PLAYER_WIDTH,  HALF_PLAYER_HEIGHT, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3( HALF_PLAYER_WIDTH,  HALF_PLAYER_HEIGHT, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-HALF_PLAYER_WIDTH, -HALF_PLAYER_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3( HALF_PLAYER_WIDTH, -HALF_PLAYER_HEIGHT, 0.0f);

		// �@���̐ݒ�
		pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		player->vtx->Unlock();
	}

	return S_OK;
}


//=============================================================================
// �v���C���[���擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_playerWk[MAX_PLAYER];
}

//=============================================================================
// �ʒu�擾
//=============================================================================
D3DXVECTOR3 GetPositionPlayer(void)
{
	return g_playerWk[MAX_PLAYER].pos;
}

//=============================================================================
// �����擾
//=============================================================================
D3DXVECTOR3 GetRotationPlayer(void)
{
	return g_playerWk[MAX_PLAYER].rot;
}

//=============================================================================
// �ړI�̌����擾
//=============================================================================
D3DXVECTOR3 GetRotationDestPlayer(void)
{
	return g_playerWk[MAX_PLAYER].rotDest;
}

//=============================================================================
// �ړ��ʎ擾
//=============================================================================
D3DXVECTOR3 GetMovePlayer(void)
{
	return g_playerWk[MAX_PLAYER].move;
}