//=============================================================================
//
// プレイヤー処理 [player.cpp]
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
// マクロ定義
//*****************************************************************************
#define TEXTURE_PLAYER_KNIGHT		"data/TEXTURE/player_knight.png"	// 騎士テクスチャ名
#define TEXTURE_PLAYER_THIEF		"data/TEXTURE/player_thief.png"		// 盗賊テクスチャ名
#define TEXTURE_PLAYER_COOK			"data/TEXTURE/player_cook.png"		// 料理人テクスチャ名
#define TEXTURE_PLAYER_WIZARD		"data/TEXTURE/player_wizard.png"	// 魔法使いテクスチャ名


#define PLAYER_WIDTH		(100)
#define PLAYER_HEIGHT		(100)
#define HALF_PLAYER_WIDTH	(PLAYER_WIDTH / 2)
#define HALF_PLAYER_HEIGHT	(PLAYER_HEIGHT / 2)

#define	PLAYER_RADIUS		(10.0f)						// 半径
#define	VALUE_MOVE_PLAYER	(0.155f)					// 移動速度
#define	RATE_MOVE_PLAYER	(0.025f)					// 移動慣性係数
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.025f)			// 回転速度
#define	RATE_ROTATE_PLAYER	(0.10f)						// 回転慣性係数
#define	VALUE_MOVE_BULLET	(7.5f)						// 弾の移動速度
#define MAX_PLAYER			(4)							// プレイヤーの数
#define PLAYER_JUMP_SPEED	(15.f)						// プレイヤーのジャンプスピード
#define GRAVITY_ACCELARATION (-0.5f)						// 重力加速度

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice, PLAYER *player);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureKnight;		// テクスチャ読み込み場所

PLAYER				g_player;					// プレイヤーワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_pD3DTextureKnight = NULL;

	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.ground = g_player.pos.y;
	g_player.state = PLAYER_ONGROUND;

	// 頂点作成
	MakeVertexPlayer(pDevice, &g_player);


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_PLAYER_KNIGHT,		// ファイルの名前
								&g_pD3DTextureKnight);	// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	// テクスチャの開放
	SAFE_RELEASE(g_pD3DTextureKnight);

	// メッシュの開放
	SAFE_RELEASE(g_player.vtx);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	if (g_player.state == PLAYER_JUMP) {
		g_player.pos += g_player.move;
		g_player.move.y += GRAVITY_ACCELARATION;
	}

	// ジャンプ処理
	if (GetKeyboardTrigger(DIK_Z) && g_player.state != PLAYER_JUMP) {
		g_player.move.y = PLAYER_JUMP_SPEED;
		g_player.state = PLAYER_JUMP;
	}

	// 着地処理
	if (g_player.pos.y < g_player.ground) {
		g_player.pos.y = g_player.ground;
		g_player.state = PLAYER_ONGROUND;	// プレイヤーの状態を着地中に
	}

	//float fDiffRotY;

	//// 目的の角度までの差分
	//fDiffRotY = g_player.rotDest.y - g_player.rot.y;
	//if(fDiffRotY > D3DX_PI)
	//{
	//	fDiffRotY -= D3DX_PI * 2.0f;
	//}
	//if(fDiffRotY < -D3DX_PI)
	//{
	//	fDiffRotY += D3DX_PI * 2.0f;
	//}

	//// 目的の角度まで慣性をかける
	//g_player.rot.y += fDiffRotY * RATE_ROTATE_PLAYER;
	//if(g_player.rot.y > D3DX_PI)
	//{
	//	g_player.rot.y -= D3DX_PI * 2.0f;
	//}
	//if(g_player.rot.y < -D3DX_PI)
	//{
	//	g_player.rot.y += D3DX_PI * 2.0f;
	//}

	///// 位置移動
	//g_player.pos.x += g_player.move.x;
	//g_player.pos.y += g_player.move.y;
	//g_player.pos.z += g_player.move.z;

	//if(g_player.pos.x < -630.0f)
	//{
	//	g_player.pos.x = -630.0f;
	//}
	//if(g_player.pos.x > 630.0f)
	//{
	//	g_player.pos.x = 630.0f;
	//}
	//if(g_player.pos.y < 10.0f)
	//{
	//	g_player.pos.y = 10.0f;
	//}
	//if(g_player.pos.y > 150.0f)
	//{
	//	g_player.pos.y = 150.0f;
	//}
	//if(g_player.pos.z > 630.0f)
	//{
	//	g_player.pos.z = 630.0f;
	//}
	//if(g_player.pos.z < -630.0f)
	//{
	//	g_player.pos.z = -630.0f;
	//}

	//// 移動量に慣性をかける
	//g_player.move.x += (0.0f - g_player.move.x) * RATE_MOVE_PLAYER;
	//g_player.move.y += (0.0f - g_player.move.y) * RATE_MOVE_PLAYER;
	//g_player.move.z += (0.0f - g_player.move.z) * RATE_MOVE_PLAYER;



	//if((g_player.move.x * g_player.move.x
	//+ g_player.move.y * g_player.move.y
	//+ g_player.move.z * g_player.move.z) > 1.0f)
	//{
	//	D3DXVECTOR3 pos;

	//	pos.x = g_player.pos.x + sinf(g_player.rot.y) * 10.0f;
	//	pos.y = g_player.pos.y + 2.0f;
	//	pos.z = g_player.pos.z + cosf(g_player.rot.y) * 10.0f;

	//	// エフェクトの設定
	//	SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	//						D3DXCOLOR(0.85f, 0.05f, 0.65f, 0.50f), 14.0f, 14.0f, 20);
	//	SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	//						D3DXCOLOR(0.65f, 0.85f, 0.05f, 0.30f), 10.0f, 10.0f, 20);
	//	SetEffect(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	//						D3DXCOLOR(0.45f, 0.45f, 0.05f, 0.15f), 5.0f, 5.0f, 20);
	//}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxWorld;

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_player.vtx, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureKnight);

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}


//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice, PLAYER *player)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&player->vtx,				// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		player->vtx->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-HALF_PLAYER_WIDTH,  HALF_PLAYER_HEIGHT, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3( HALF_PLAYER_WIDTH,  HALF_PLAYER_HEIGHT, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-HALF_PLAYER_WIDTH, -HALF_PLAYER_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3( HALF_PLAYER_WIDTH, -HALF_PLAYER_HEIGHT, 0.0f);

		// 法線の設定
		pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		player->vtx->Unlock();
	}

	return S_OK;
}


//=============================================================================
// プレイヤーを取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_player;
}

//=============================================================================
// 位置取得
//=============================================================================
D3DXVECTOR3 GetPositionPlayer(void)
{
	return g_player.pos;
}

//=============================================================================
// 向き取得
//=============================================================================
D3DXVECTOR3 GetRotationPlayer(void)
{
	return g_player.rot;
}

//=============================================================================
// 目的の向き取得
//=============================================================================
D3DXVECTOR3 GetRotationDestPlayer(void)
{
	return g_player.rotDest;
}

//=============================================================================
// 移動量取得
//=============================================================================
D3DXVECTOR3 GetMovePlayer(void)
{
	return g_player.move;
}