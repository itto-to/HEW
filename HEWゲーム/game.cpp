//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : 
//
//=============================================================================
#include "game.h"

#include "background.h"
#include "bullet.h"
#include "camera.h"
#include "effect.h"
#include "explosion.h"
#include "item.h"
#include "life.h"
#include "light.h"
#include "player.h"
#include "score.h"
#include "shadow.h"
#include "sound.h"
#include "stage.h"
#include "timer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void CheckHitPlayerObstacle(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	// ライトの初期化
	InitLight();

	// 影の初期化
	//InitShadow();

	// プレイヤーの初期化
	InitPlayer();

	// ステージの初期化
	InitStage();

	// 背景の初期化
	InitBackground();

	// タイマーの初期化
	InitTimer();
	ResetTimer(30);

	// スコアの初期化
	InitScore();

	// BGM再生
	PlaySound(SOUND_LABEL_BGM000);

#ifdef _DEBUG
	// バウンディングボックス描画用初期化
	InitDebugBoundingBox();
#endif

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	// ライトの終了処理
	UninitLight();

	// プレイヤーの終了処理
	UninitPlayer();

	// ステージの終了処理
	UninitStage();

	// 背景の終了処理
	UninitBackground();

	// タイマーの終了処理
	UninitTimer();

	// スコアの終了処理
	UninitScore();

	// BGM停止
	PlaySound(SOUND_LABEL_BGM000);

#ifdef _DEBUG
	// バウンディングボックス描画用終了処理
	UninitDebugBoundingBox();
#endif
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	// 背景処理の更新
	UpdateBackground();

	// プレイヤー処理の更新
	UpdatePlayer();

	// ステージの更新処理
	UpdateStage();

	// プレイヤーと障害物の当たり判定
	CheckHitPlayerObstacle();

	// ライフ処理の更新
	UpdateLife();

	// タイマー処理の更新
	UpdateTimer();

	// スコア処理の更新
	UpdateScore();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{
	// カメラの設定
	SetCamera();

	// 背景の描画
	DrawBackground();

	// プレイヤー処理の描画
	DrawPlayer();

	// ステージの描画処理
	DrawStage();

	// ライフ処理の描画
	DrawLife();

	// タイマー処理の描画
	DrawTimer();

	// スコア処理の描画
	DrawScore();
}

void CheckHitPlayerObstacle(void)
{
	for (int player_no = 0; player_no < NumPlayer(); player_no++)
	{
		PLAYER *player = GetPlayer(player_no);

		if (player->is_invincible)	// 無敵なら判定しない
			continue;

		// プレイヤーのバウンディングボックス取得
		BOUNDING_BOX playerBox = ToWorldBoundingBox(player->hitBox, player->pos);

		// 障害物のバウンディングボックス取得
		OBSTACLE *obstacle = GetObstacle();
		BOUNDING_BOX obstacleBox = ToWorldBoundingBox(obstacle->hitBox, obstacle->pos);

		if (IsIntersectedBoundingBox(playerBox, obstacleBox))
		{
			// プレイヤーのライフ減少
			player->life--;
			// 無敵状態に
			player->is_invincible = true;
			player->invincible_counter = 0;
		}
	}
}
