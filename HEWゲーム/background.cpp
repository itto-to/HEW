//=============================================================================
//
// 背景処理 [background.cpp]
// Author : 
//
//=============================================================================
#include "background.h"

#include "mesh.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_BACKGROUND		"data/TEXTURE/background.png"


//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct {
	LPDIRECT3DVERTEXBUFFER9 vtx;	// 頂点バッファ
	LPDIRECT3DTEXTURE9 texture;		// テクスチャのポインタ
	D3DXVECTOR3 pos;				// 座標
} BACKGROUND;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************



//*****************************************************************************
// グローバル変数
//*****************************************************************************
BACKGROUND g_background;


HRESULT InitBackground()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_background.pos = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 100.0f);

	// 頂点作成
	MakeVertex(pDevice, g_background.vtx, SCREEN_WIDTH, SCREEN_HEIGHT);

	// テクスチャ作成
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

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_background.vtx, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_background.pos.x, g_background.pos.y, g_background.pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// テクスチャの設定
	pDevice->SetTexture(0, g_background.texture);

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}
