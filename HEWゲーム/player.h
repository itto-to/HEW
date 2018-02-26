//=============================================================================
//
// �v���C���[���� [player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "collision.h"
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PLAYER			(4)							// �v���C���[�̐�


//*****************************************************************************
// �񋓑̒�`
//*****************************************************************************
enum PLAYER_STATE {
	PLAYER_ONGROUND,	// ���n
	PLAYER_JUMP,		// �W�����v��
	PLAYER_DEAD,		// ���S
	PLAYER_STATEMAX,
};



//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	int id;				// �v���C���[�ԍ�
	bool kengen;				// �X�L���𔭓����錠���������Ă��邩�ǂ���
	LPDIRECT3DTEXTURE9 texture; // �e�N�X�`���ǂݍ��ݏꏊ
	LPDIRECT3DVERTEXBUFFER9 vtx;
	D3DXVECTOR3 pos;			// ���݂̈ʒu
	D3DXVECTOR3 move;			// �ړ���
	D3DXVECTOR3 rot;			// ���݂̌���
	D3DXVECTOR3 rotDest;		// �ړI�̌���
	D3DXVECTOR3 scl;			// �X�P�[��
	float ground;				// �n�ʂ̍���
	float distance;				// �i�񂾋���
	int life;					// �v���C���[�̗̑�
	PLAYER_STATE state;			// �v���C���[�̏��
	float state_counter;		// ��Ԃ̃^�C�}�[
	bool is_invincible;			// ���G��Ԃ��ǂ���
	int invincible_counter;		// ���G���Ԃ̃J�E���g
	BOUNDING_BOX hitBox;		// �v���C���[�̓����蔻��
	BOUNDING_BOX screenBox;		// ��ʊO����p�{�b�N�X
} PLAYER;

//*****************************************************************************
// �v���g�^�C�v�錾
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
