//=============================================================================
//
// 衝突処理 [collision.cpp]
// Author : 
//
//=============================================================================
#include "collision.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_BOUNDING_BOX	"data/TEXTURE/bounding_box.png"


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void DrawDebugBoundingBox(BOUNDING_BOX box);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_textureBoundingBox;	// デバッグ用バウンディングボックステクスチャへのポインタ

// バウンディングボックス初期化
void InitBoundingBox(BOUNDING_BOX *box, D3DXVECTOR3 center, float length_x, float length_y, float length_z)
{
	D3DXVECTOR3 half_length = D3DXVECTOR3(length_x, length_y, length_z) / 2.0f;
	box->min = center - half_length;
	box->max = center + half_length;
}

HRESULT InitDebugBoundingBox(void) {
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (FAILED(D3DXCreateTextureFromFile(pDevice, TEXTURE_BOUNDING_BOX, &g_textureBoundingBox))) {
		return E_FAIL;
	}

	return S_OK;
}

void UninitDebugBoundingBox(void) {
	SAFE_RELEASE(g_textureBoundingBox);
}

// オブジェクトのワールド座標とローカルバウンディングボックスからワールドバウンディングボックスを返す
BOUNDING_BOX ToWorldBoundingBox(BOUNDING_BOX localBox, D3DXVECTOR3 posWorld)
{
	BOUNDING_BOX worldBox;
	worldBox.max = posWorld + localBox.max;
	worldBox.min = posWorld + localBox.min;

	return worldBox;
}

// バウンディングボックスが衝突していたらtrueを返す
bool IsIntersectedBoundingBox(BOUNDING_BOX box1, BOUNDING_BOX box2)
{
	if (box1.max.x < box2.min.x)
		return false;
	if (box1.min.x > box2.max.x)
		return false;
	if (box1.max.y < box2.min.y)
		return false;
	if (box1.min.y > box2.max.y)
		return false;
	if (box1.max.z < box2.min.z)
		return false;
	if (box1.min.z > box2.max.z)
		return false;

	return true;
}

// バウンディングボックスが衝突していたらtrueを返す
bool IsIntersectedBoundingBox(D3DXVECTOR3 pos1, BOUNDING_BOX localBox1, D3DXVECTOR3 pos2, BOUNDING_BOX localBox2)
{
	BOUNDING_BOX worldBox1, worldBox2;
	worldBox1.max = pos1 + localBox1.max;
	worldBox1.min = pos1 + localBox1.min;
	worldBox2.max = pos2 + localBox2.max;
	worldBox2.min = pos2 + localBox2.min;

	if (IsIntersectedBoundingBox(worldBox1, worldBox2))
		return true;

	return false;
}

// バウンディングボックスを描画
void DrawDebugBoundingBox(BOUNDING_BOX box)
{

}
