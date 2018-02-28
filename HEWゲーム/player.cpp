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
#include "game.h"
#include "item.h"
#include "mesh.h"
#include "player_behavior.h"
#include "score.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_PLAYER_KNIGHT		"data/TEXTURE/player_knight.png"	// 騎士テクスチャ名
#define TEXTURE_PLAYER_THIEF		"data/TEXTURE/player_thief.png"		// 盗賊テクスチャ名
#define TEXTURE_PLAYER_COOK			"data/TEXTURE/player_cook.png"		// 料理人テクスチャ名
#define TEXTURE_PLAYER_WIZARD		"data/TEXTURE/player_wizard.png"	// 魔法使いテクスチャ名

#define	PLAYER_PADDING		(-100.0f)					// プレイヤー同士の間隔
#define	VALUE_MOVE_PLAYER	(0.155f)					// 移動速度
#define	RATE_MOVE_PLAYER	(0.025f)					// 移動慣性係数
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.025f)			// 回転速度
#define	RATE_ROTATE_PLAYER	(0.10f)						// 回転慣性係数
#define	VALUE_MOVE_BULLET	(7.5f)						// 弾の移動速度
#define GRAVITY_ACCELARATION (-0.5f)					// 重力加速度
#define MAX_LIFE			(3)							// 最大体力
#define INVINCIBLE_FRAME	(100)						// 敵と当たった後の無敵フレーム


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice, PLAYER *player);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureKnight;		// テクスチャ読み込み場所
int					g_num_player;				// プレイヤーの人数
PLAYER				g_playerWk[MAX_PLAYER];		// プレイヤーワーク

char *player_textureFileName[MAX_PLAYER] =
{
	TEXTURE_PLAYER_KNIGHT,
	TEXTURE_PLAYER_THIEF,
	TEXTURE_PLAYER_COOK,
	TEXTURE_PLAYER_WIZARD,
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// プレイ人数設定
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
		g_playerWk[no].speed_factor = 1.0f;
		
		// 当たり判定初期化
		g_playerWk[no].hitBox.max = PLAYER_BB_MAX;
		g_playerWk[no].hitBox.min = PLAYER_BB_MIN;

		// 画面外判定用バウンディングボックス初期化
		// 画面外判定初期化
		InitBoundingBox(&g_playerWk[no].screenBox, D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_WIDTH, PLAYER_HEIGHT, 0.0f);

		// 頂点作成
		MakeVertex(pDevice, &g_playerWk[no].vtx, PLAYER_WIDTH, PLAYER_HEIGHT);
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_PLAYER_KNIGHT,			// ファイルの名前
		&g_playerWk[0].texture);		// 読み込むメモリー
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_PLAYER_WIZARD,			// ファイルの名前
		&g_playerWk[1].texture);		// 読み込むメモリー
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_PLAYER_THIEF,			// ファイルの名前
		&g_playerWk[2].texture);		// 読み込むメモリー
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_PLAYER_COOK,			// ファイルの名前
		&g_playerWk[3].texture);		// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	for(int no = 0; no < MAX_PLAYER; no++)
	{
		// テクスチャの開放
		SAFE_RELEASE(g_playerWk[no].texture);

		// メッシュの開放
		SAFE_RELEASE(g_playerWk[no].vtx);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	for(int no = 0; no < MAX_PLAYER; no++)
	{
#ifdef _DEBUG
		// TEST: 速度変化テスト
		if (GetKeyboardPress(DIK_I)) {
			g_playerWk[no].speed_factor += 0.1f;
		}
		if (GetKeyboardPress(DIK_O)) {
			g_playerWk[no].speed_factor -= 0.1f;
		}
		PrintDebugProc("速度係数：%f", g_playerWk[no].speed_factor);
#endif
		switch (g_playerWk[no].state)
		{
		case PLAYER_ONGROUND:
			UpdatePlayerOnGround(&g_playerWk[no]);
			break;

		case PLAYER_JUMP:
			UpdatePlayerJump(&g_playerWk[no]);
			break;

		case PLAYER_SLIDING:
			UpdatePlayerSliding(&g_playerWk[no]);
			break;

		case PLAYER_DEAD:
			UpdatePlayerDead(&g_playerWk[no]);
			break;

		default:
			break;
		}

		// 死亡判定
		if (g_playerWk[no].life <= 0)
		{
			g_playerWk[no].state = PLAYER_DEAD;
		}

		if(g_playerWk[no].state == PLAYER_JUMP)
		{
			g_playerWk[no].pos += g_playerWk[no].move;
			g_playerWk[no].move.y += GRAVITY_ACCELARATION;
		}

		// 着地処理
		if(g_playerWk[no].pos.y < g_playerWk[no].ground)
		{
			g_playerWk[no].pos.y = g_playerWk[no].ground;
			g_playerWk[no].state = PLAYER_ONGROUND;	// プレイヤーの状態を着地中に
		}

		// 無敵カウントが一定以上なら元の状態に
		if (g_playerWk[no].is_invincible)
		{
			g_playerWk[no].invincible_counter++;
			if (g_playerWk[no].invincible_counter > INVINCIBLE_FRAME)
			{
				g_playerWk[no].is_invincible = false;
			}
		}


		//float fDiffRotY;

		//// 目的の角度までの差分
		//fDiffRotY = g_playerWk[no].rotDest.y - g_playerWk[no].rot.y;
		//if(fDiffRotY > D3DX_PI)
		//{
		//	fDiffRotY -= D3DX_PI * 2.0f;
		//}
		//if(fDiffRotY < -D3DX_PI)
		//{
		//	fDiffRotY += D3DX_PI * 2.0f;
		//}

		//// 目的の角度まで慣性をかける
		//g_playerWk[no].rot.y += fDiffRotY * RATE_ROTATE_PLAYER;
		//if(g_playerWk[no].rot.y > D3DX_PI)
		//{
		//	g_playerWk[no].rot.y -= D3DX_PI * 2.0f;
		//}
		//if(g_playerWk[no].rot.y < -D3DX_PI)
		//{
		//	g_playerWk[no].rot.y += D3DX_PI * 2.0f;
		//}

		///// 位置移動
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

		//// 移動量に慣性をかける
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

		//	// エフェクトの設定
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
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxWorld;

	for (int no = 0; no < MAX_PLAYER; no++)
	{

		// 描画
		if (!g_playerWk[no].is_invincible || ((g_playerWk[no].invincible_counter % 10) > 4))
		{
			DrawMesh(g_playerWk[no].vtx, g_playerWk[no].texture, g_playerWk[no].pos, g_playerWk[no].rot, g_playerWk[no].scl);
		}

		//// 頂点バッファをデバイスのデータストリームにバインド
		//pDevice->SetStreamSource(0, g_playerWk[no].vtx, 0, sizeof(VERTEX_3D));

		//// 頂点フォーマットの設定
		//pDevice->SetFVF(FVF_VERTEX_3D);

		//// ワールドマトリックスの初期化
		//D3DXMatrixIdentity(&mtxWorld);

		//// 回転を反映
		//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_playerWk[no].rot.y, g_playerWk[no].rot.x, g_playerWk[no].rot.z);
		//D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		//// 移動を反映
		//D3DXMatrixTranslation(&mtxTranslate, g_playerWk[no].pos.x, g_playerWk[no].pos.y, g_playerWk[no].pos.z);
		//D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		//// ワールドマトリックスの設定
		//pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		//// テクスチャの設定
		//pDevice->SetTexture(0, g_pD3DTextureKnight);

		//// 描画
		//if (!g_playerWk[no].is_invincible || ((g_playerWk[no].invincible_counter % 10) > 4))
		//{
		//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		//}

#ifdef _DEBUG
		// バウンディングボックスを描画
		BOUNDING_BOX worldBox = ToWorldBoundingBox(g_playerWk[no].hitBox, g_playerWk[no].pos);
		DrawDebugBoundingBox(worldBox);

		// デバッグ情報を表示
		D3DXVECTOR3 pos = g_playerWk[no].pos;
		PrintDebugProc("***プレイヤー%d番***\n", no);
		PrintDebugProc("座標 X：%f Y:%f Z:%f\n", pos.x, pos.y, pos.z);
		PrintDebugProc("ライフ : %d\n", g_playerWk[no].life);

		switch (g_playerWk[no].state)
		{
		case PLAYER_ONGROUND:
			PrintDebugProc("状態：%s", "着地\n");
			break;

		case PLAYER_JUMP:
			PrintDebugProc("状態：%s", "ジャンプ\n");
			break;

		case PLAYER_DEAD:
			PrintDebugProc("状態：%s", "死亡\n");
			break;
		}

		if (g_playerWk[no].is_invincible) {
			PrintDebugProc("無敵\n");
		}
		else {
			PrintDebugProc("無敵じゃない\n");
		}

#endif
	}
}


void ChangePlayerState(PLAYER_STATE nextState) {
	// 現在のステートのExit処理


	// 次のステートのEnter処理
	switch (nextState)
	{
		
	}
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
PLAYER *GetPlayer(int no)
{
	return &g_playerWk[no];
}

//=============================================================================
// 位置取得
//=============================================================================
D3DXVECTOR3 GetPositionPlayer(void)
{
	return g_playerWk[0].pos;
}

//=============================================================================
// 向き取得
//=============================================================================
D3DXVECTOR3 GetRotationPlayer(void)
{
	return g_playerWk[0].rot;
}

//=============================================================================
// 目的の向き取得
//=============================================================================
D3DXVECTOR3 GetRotationDestPlayer(void)
{
	return g_playerWk[0].rotDest;
}

//=============================================================================
// 移動量取得
//=============================================================================
D3DXVECTOR3 GetMovePlayer(void)
{
	return g_playerWk[0].move;
}

//=============================================================================
// 現在のプレイヤー人数取得
//=============================================================================
int NumPlayer(void)
{
	return g_num_player;
}