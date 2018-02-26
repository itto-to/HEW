//=============================================================================
//
// ステージ処理 [stage.cpp]
// Author : 
//
//=============================================================================
#include "stage.h"

#include "camera.h"
#include "collision.h"
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
	bool use;			// プレイヤーが参加していればtrue
	float speed_factor; // 速度係数 デフォルトは1.0f
} LANE;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LANE g_lane[MAX_PLAYER];
OBSTACLE g_obstacle;



HRESULT InitStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// レーン初期化
	for (int no = 0; no < MAX_PLAYER; no++)
	{
		g_lane[no].use = true;
		g_lane[no].speed_factor = 1.0f;
	}

	g_obstacle.pos = D3DXVECTOR3(-SCREEN_WIDTH / 2.0f, LANE_Y(0), LANE_Z(0));
	g_obstacle.move = D3DXVECTOR3(10.f, 0.0f, 0.0f);
	g_obstacle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_obstacle.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 当たり判定初期化
	InitBoundingBox(&g_obstacle.hitBox, D3DXVECTOR3(0.0f, 0.0f, 0.0f), OBSTACLE_WIDTH, OBSTACLE_HEIGHT, 0.0f);

	// 画面外判定初期化
	InitBoundingBox(&g_obstacle.screenBox, D3DXVECTOR3(0.0f, 0.0f, 0.0f), OBSTACLE_WIDTH, OBSTACLE_HEIGHT, 0.0f);

	// 頂点作成
	MakeVertex(pDevice, &g_obstacle.vtx, OBSTACLE_WIDTH, OBSTACLE_HEIGHT);

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

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_obstacle.vtx, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// スケールを反映
	D3DXMatrixIdentity(&mtxScl);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

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

#ifdef _DEBUG
	BOUNDING_BOX worldBox = ToWorldBoundingBox(g_obstacle.hitBox, g_obstacle.pos);
	DrawDebugBoundingBox(worldBox);
#endif
}

OBSTACLE *GetObstacle(void)
{
	return &g_obstacle;
}