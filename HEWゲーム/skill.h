/***************************************************************
* ファイル名:	skill.h
* 制作者:		HAL東京ゲーム学科 山本 壮将
* 制作日:		2月23日
* 説明:			スキル関連のヘッダー
****************************************************************/
#ifndef _SKILL_H_
#define _SKILL_H_

//**************************************************************
// インクルードファイル
//**************************************************************
#include "main.h"


//**************************************************************
// マクロ定義
//**************************************************************

//**************************************************************
// 構造体定義
//**************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9			Texture_bar;
	LPDIRECT3DTEXTURE9			Texture_waku;
	LPDIRECT3DVERTEXBUFFER9		Buff_bar;
	LPDIRECT3DVERTEXBUFFER9		Buff_waku;
	float					gage;					// 今のゲージの値
	float					gage_lvup;				// レベルアップに必要な値
	int						lv;						// スキルのレベル
	bool					kengen;					// 権限は既に割りふてられている？
}SKILL;
//**************************************************************
// プロトタイプ宣言
//**************************************************************
HRESULT InitSkill(void);
void UninitSkill(void);
void UpdateSkill(float gageup);
void DrawSkill(void);
HRESULT MakeVertexSkill(LPDIRECT3DDEVICE9 pDevice);
void SetColorSkill(void);
void GetSkill(int no);
void SkillReset(int no);

#endif