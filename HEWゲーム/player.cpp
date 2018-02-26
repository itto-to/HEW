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
#include "game.h"
#include "item.h"
#include "mesh.h"
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
#define HALF_PLAYER_DEPTH	(0.0f)

#define PLAYER_BB_MAX		(D3DXVECTOR3(HALF_PLAYER_WIDTH, HALF_PLAYER_HEIGHT, HALF_PLAYER_DEPTH))// �v���C���[�o�E���f�B���O�{�b�N�X��max���_���W
#define PLAYER_BB_MIN		(-PLAYER_BB_MAX)			// �v���C���[�o�E���f�B���O�{�b�N�X��min���_���W

#define	PLAYER_PADDING		(-100.0f)					// �v���C���[���m�̊Ԋu
#define	VALUE_MOVE_PLAYER	(0.155f)					// �ړ����x
#define	RATE_MOVE_PLAYER	(0.025f)					// �ړ������W��
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.025f)			// ��]���x
#define	RATE_ROTATE_PLAYER	(0.10f)						// ��]�����W��
#define	VALUE_MOVE_BULLET	(7.5f)						// �e�̈ړ����x
#define PLAYER_JUMP_SPEED	(15.f)						// �v���C���[�̃W�����v����
#define PLAYER_BIG_JUMP_SPEED (20.f)					// �v���C���[�̑�W�����v����
#define GRAVITY_ACCELARATION (-0.5f)					// �d�͉����x
#define MAX_LIFE			(3)							// �ő�̗�
#define INVINCIBLE_FRAME	(100)						// �G�Ɠ���������̖��G�t���[��
#define SLIDING_HIT_BOX		{D3DXVECTOR3(-HALF_PLAYER_WIDTH, -HALF_PLAYER_HEIGHT, -HALF_PLAYER_DEPTH), D3DXVECTOR3(HALF_PLAYER_WIDTH, 0.0f, HALF_PLAYER_DEPTH)}


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice, PLAYER *player);

void EnterPlayerOnGround(PLAYER *player);
void EnterPlayerJump(PLAYER *player);
void EnterPlayerDead(PLAYER *player);

void ExitPlayerOnGround(PLAYER *player);
void ExitPlayerJump(PLAYER *player);
void ExitPlayerDead(PLAYER *player);

void UpdatePlayerOnGround(PLAYER *player);
void UpdatePlayerJump(PLAYER *player);
void UpdatePlayerDead(PLAYER *player);



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureKnight;		// �e�N�X�`���ǂݍ��ݏꏊ
int					g_num_player;				// �v���C���[�̐l��
PLAYER				g_playerWk[MAX_PLAYER];		// �v���C���[���[�N

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

	// �v���C�l���ݒ�
	g_num_player = MAX_PLAYER;

	for(int no = 0; no < MAX_PLAYER; no++)
	{
		g_playerWk[no].texture = NULL;

		g_playerWk[no].id      = no;
		g_playerWk[no].pos     = D3DXVECTOR3(100.0f, LANE_Y(no), LANE_Z(no));
		g_playerWk[no].move    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_playerWk[no].rot     = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_playerWk[no].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_playerWk[no].scl     = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_playerWk[no].ground  = g_playerWk[no].pos.y;
		g_playerWk[no].life    = MAX_LIFE;
		g_playerWk[no].state   = PLAYER_ONGROUND;
		
		// �����蔻�菉����
		g_playerWk[no].hitBox.max = PLAYER_BB_MAX;
		g_playerWk[no].hitBox.min = PLAYER_BB_MIN;

		// ��ʊO����p�o�E���f�B���O�{�b�N�X������
		// ��ʊO���菉����
		InitBoundingBox(&g_playerWk[no].screenBox, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_WIDTH, PLAYER_HEIGHT, 0.0f);

		// ���_�쐬
		MakeVertex(pDevice, &g_playerWk[no].vtx, PLAYER_WIDTH, PLAYER_HEIGHT);
		//MakeVertexPlayer(pDevice, &g_playerWk[no]);

	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_KNIGHT,			// �t�@�C���̖��O
		&g_playerWk[0].texture);		// �ǂݍ��ރ������[
	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_WIZARD,			// �t�@�C���̖��O
		&g_playerWk[1].texture);		// �ǂݍ��ރ������[
	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_THIEF,			// �t�@�C���̖��O
		&g_playerWk[2].texture);		// �ǂݍ��ރ������[
	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_COOK,			// �t�@�C���̖��O
		&g_playerWk[3].texture);		// �ǂݍ��ރ������[

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
		SAFE_RELEASE(g_playerWk[no].texture);

		// ���b�V���̊J��
		SAFE_RELEASE(g_playerWk[no].vtx);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{

	for(int no = 0; no < MAX_PLAYER; no++)
	{
		switch (g_playerWk[no].state)
		{
		case PLAYER_ONGROUND:
		//	UpdatePlayerOnGround(&g_playerWk[no]);
		//	break;

		//case PLAYER_JUMP:
		//	UpdatePlayerJump(&g_playerWk[no]);
		//	break;

		//case PLAYER_DEAD:
		//	UpdatePlayerDead(&g_playerWk[no]);
		//	break;

		default:
			break;
		}

		// ���S����
		if (g_playerWk[no].life <= 0) {
			g_playerWk[no].state = PLAYER_DEAD;
		}

		if(g_playerWk[no].state == PLAYER_JUMP)
		{
			g_playerWk[no].pos += g_playerWk[no].move;
			g_playerWk[no].move.y += GRAVITY_ACCELARATION;
		}

		// �W�����v����
		if(GetKeyboardTrigger(DIK_Z) && g_playerWk[no].state != PLAYER_JUMP)
		{
			g_playerWk[no].move.y = PLAYER_JUMP_SPEED;
			g_playerWk[no].state = PLAYER_JUMP;
		}
		else if (GetKeyboardTrigger(DIK_X) && g_playerWk[no].state != PLAYER_JUMP)
		{
			g_playerWk[no].move.y = PLAYER_BIG_JUMP_SPEED;
			g_playerWk[no].state = PLAYER_JUMP;
		}

		// �X���C�f�B���O����
		if (GetKeyboardTrigger(DIK_DOWN)) 
		{
			// �����蔻�������������
			g_playerWk[no].hitBox = SLIDING_HIT_BOX;
		}

		// ���n����
		if(g_playerWk[no].pos.y < g_playerWk[no].ground)
		{
			g_playerWk[no].pos.y = g_playerWk[no].ground;
			g_playerWk[no].state = PLAYER_ONGROUND;	// �v���C���[�̏�Ԃ𒅒n����
		}

		// ���G�J�E���g�����ȏ�Ȃ猳�̏�Ԃ�
		if (g_playerWk[no].is_invincible)
		{
			g_playerWk[no].invincible_counter++;
			if (g_playerWk[no].invincible_counter > INVINCIBLE_FRAME)
			{
				g_playerWk[no].is_invincible = false;
			}
		}


		//float fDiffRotY;

		//// �ړI�̊p�x�܂ł̍���
		//fDiffRotY = g_playerWk[no].rotDest.y - g_playerWk[no].rot.y;
		//if(fDiffRotY > D3DX_PI)
		//{
		//	fDiffRotY -= D3DX_PI * 2.0f;
		//}
		//if(fDiffRotY < -D3DX_PI)
		//{
		//	fDiffRotY += D3DX_PI * 2.0f;
		//}

		//// �ړI�̊p�x�܂Ŋ�����������
		//g_playerWk[no].rot.y += fDiffRotY * RATE_ROTATE_PLAYER;
		//if(g_playerWk[no].rot.y > D3DX_PI)
		//{
		//	g_playerWk[no].rot.y -= D3DX_PI * 2.0f;
		//}
		//if(g_playerWk[no].rot.y < -D3DX_PI)
		//{
		//	g_playerWk[no].rot.y += D3DX_PI * 2.0f;
		//}

		///// �ʒu�ړ�
		//g_playerWk[no].pos.x += g_playerWk[no].move.x;
		//g_playerWk[no].pos.y += g_playerWk[no].move.y;
		//g_playerWk[no].pos.z += g_playerWk[no].move.z;

		//if(g_playerWk[no].pos.x < -630.0f)
		//{
		//	g_playerWk[no].pos.x = -630.0f;
		//}
		//if(g_playerWk[no].pos.x > 630.0f)
		//{
		//	g_playerWk[no].pos.x = 630.0f;
		//}
		//if(g_playerWk[no].pos.y < 10.0f)
		//{
		//	g_playerWk[no].pos.y = 10.0f;
		//}
		//if(g_playerWk[no].pos.y > 150.0f)
		//{
		//	g_playerWk[no].pos.y = 150.0f;
		//}
		//if(g_playerWk[no].pos.z > 630.0f)
		//{
		//	g_playerWk[no].pos.z = 630.0f;
		//}
		//if(g_playerWk[no].pos.z < -630.0f)
		//{
		//	g_playerWk[no].pos.z = -630.0f;
		//}

		//// �ړ��ʂɊ�����������
		//g_playerWk[no].move.x += (0.0f - g_playerWk[no].move.x) * RATE_MOVE_PLAYER;
		//g_playerWk[no].move.y += (0.0f - g_playerWk[no].move.y) * RATE_MOVE_PLAYER;
		//g_playerWk[no].move.z += (0.0f - g_playerWk[no].move.z) * RATE_MOVE_PLAYER;



		//if((g_playerWk[no].move.x * g_playerWk[no].move.x
		//+ g_playerWk[no].move.y * g_playerWk[no].move.y
		//+ g_playerWk[no].move.z * g_playerWk[no].move.z) > 1.0f)
		//{
		//	D3DXVECTOR3 pos;

		//	pos.x = g_playerWk[no].pos.x + sinf(g_playerWk[no].rot.y) * 10.0f;
		//	pos.y = g_playerWk[no].pos.y + 2.0f;
		//	pos.z = g_playerWk[no].pos.z + cosf(g_playerWk[no].rot.y) * 10.0f;

		//	// �G�t�F�N�g�̐ݒ�
		//	SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		//						D3DXCOLOR(0.85f, 0.05f, 0.65f, 0.50f), 14.0f, 14.0f, 20);
		//	SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		//						D3DXCOLOR(0.65f, 0.85f, 0.05f, 0.30f), 10.0f, 10.0f, 20);
		//	SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		//						D3DXCOLOR(0.45f, 0.45f, 0.05f, 0.15f), 5.0f, 5.0f, 20);
		//}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxWorld;

	for (int no = 0; no < MAX_PLAYER; no++)
	{

		// �`��
		if (!g_playerWk[no].is_invincible || ((g_playerWk[no].invincible_counter % 10) > 4))
		{
			DrawMesh(g_playerWk[no].vtx, g_playerWk[no].texture, g_playerWk[no].pos, g_playerWk[no].rot, g_playerWk[no].scl);
		}

		//// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		//pDevice->SetStreamSource(0, g_playerWk[no].vtx, 0, sizeof(VERTEX_3D));

		//// ���_�t�H�[�}�b�g�̐ݒ�
		//pDevice->SetFVF(FVF_VERTEX_3D);

		//// ���[���h�}�g���b�N�X�̏�����
		//D3DXMatrixIdentity(&mtxWorld);

		//// ��]�𔽉f
		//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_playerWk[no].rot.y, g_playerWk[no].rot.x, g_playerWk[no].rot.z);
		//D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		//// �ړ��𔽉f
		//D3DXMatrixTranslation(&mtxTranslate, g_playerWk[no].pos.x, g_playerWk[no].pos.y, g_playerWk[no].pos.z);
		//D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		//// ���[���h�}�g���b�N�X�̐ݒ�
		//pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		//// �e�N�X�`���̐ݒ�
		//pDevice->SetTexture(0, g_pD3DTextureKnight);

		//// �`��
		//if (!g_playerWk[no].is_invincible || ((g_playerWk[no].invincible_counter % 10) > 4))
		//{
		//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		//}

#ifdef _DEBUG
		// �o�E���f�B���O�{�b�N�X��`��
		BOUNDING_BOX worldBox = ToWorldBoundingBox(g_playerWk[no].hitBox, g_playerWk[no].pos);
		DrawDebugBoundingBox(worldBox);

		// �f�o�b�O����\��
		D3DXVECTOR3 pos = g_playerWk[no].pos;
		PrintDebugProc("***�v���C���[%d��***\n", no);
		PrintDebugProc("���W X�F%f Y:%f Z:%f\n", pos.x, pos.y, pos.z);
		PrintDebugProc("���C�t : %d\n", g_playerWk[no].life);

		switch (g_playerWk[no].state)
		{
		case PLAYER_ONGROUND:
			PrintDebugProc("��ԁF%s", "���n\n");
			break;

		case PLAYER_JUMP:
			PrintDebugProc("��ԁF%s", "�W�����v\n");
			break;

		case PLAYER_DEAD:
			PrintDebugProc("��ԁF%s", "���S\n");
			break;
		}

		if (g_playerWk[no].is_invincible) {
			PrintDebugProc("���G\n");
		}
		else {
			PrintDebugProc("���G����Ȃ�\n");
		}

#endif
	}

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
PLAYER *GetPlayer(int no)
{
	return &g_playerWk[no];
}

//=============================================================================
// �ʒu�擾
//=============================================================================
D3DXVECTOR3 GetPositionPlayer(void)
{
	return g_playerWk[0].pos;
}

//=============================================================================
// �����擾
//=============================================================================
D3DXVECTOR3 GetRotationPlayer(void)
{
	return g_playerWk[0].rot;
}

//=============================================================================
// �ړI�̌����擾
//=============================================================================
D3DXVECTOR3 GetRotationDestPlayer(void)
{
	return g_playerWk[0].rotDest;
}

//=============================================================================
// �ړ��ʎ擾
//=============================================================================
D3DXVECTOR3 GetMovePlayer(void)
{
	return g_playerWk[0].move;
}

//=============================================================================
// ���݂̃v���C���[�l���擾
//=============================================================================
int NumPlayer(void)
{
	return g_num_player;
}