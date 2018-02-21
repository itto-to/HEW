//=============================================================================
//
// ステージ処理 [stage.cpp]
// Author : 
//
//=============================================================================
#include "stage.h"

#include "mesh.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_OBSTACLE "data/TEXTURE/obstacle.png"
#define OBSTACLE_WIDTH	(100)
#define OBSTACLE_HEIGHT	(100)

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct {
	LPDIRECT3DTEXTURE9 texture; // テクスチャ読み込み場所
	LPDIRECT3DVERTEXBUFFER9 vtx;// 頂点バッファ
	D3DXVECTOR3 pos;			// 現在の位置
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 rot;			// 現在の向き
	D3DXVECTOR3 rotDest;		// 目的の向き
} OBSTACLE;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
//LPDIRECT3DTEXTURE9 g_obstacle.texture;
OBSTACLE g_obstacle;


HRESULT InitStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_obstacle.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_obstacle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 頂点作成
	MakeVertex(pDevice, g_obstacle.vtx, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_OBSTACLE,		// ファイルの名前
		&g_obstacle.texture);	// 読み込むメモリー


	return S_OK;
}

void UninitStage(void)
{
	SAFE_RELEASE(g_obstacle.vtx);

	SAFE_RELEASE(g_obstacle.texture);
}

void UpdateStage(void)
{

}

void DrawStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxWorld;

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_obstacle.vtx, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_obstacle.rot.y, g_obstacle.rot.x, g_obstacle.rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_obstacle.pos.x, g_obstacle.pos.y, g_obstacle.pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// テクスチャの設定
	pDevice->SetTexture(0, g_obstacle.texture);

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

}