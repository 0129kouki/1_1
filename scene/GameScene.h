#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	void PlayerUpdate();
	void BeamUpdate();
	void BeamMove();
	void BeamBorn();
	void EnemyUpdate();
	void EnemyMove();
	void EnemyBorn();
	void Collision();
	void CollisionPlayerEnemy();
	void CollisionEnemy();
	void CollisionBeamEnemy();
	void GamePlayUpdate();         //ゲームプレイ更新
	void GamePlayDraw3D();         //ゲームプレイ3D表示
	void GamePlayDraw2DBack();     //ゲームプレイ背景2D表示
	void GamePlayDraw2DNear();     //ゲームプレイ近景2D表示
	void TitleUpdate();            //タイトル更新
	void TitleDraw2DNear();        //タイトル2D
	void EndingUpdate();           //エンディング更新
	void GameOverDraw2DNear();     //エンディング
	void Ending2Draw();            //エンター
	void GamePlayStart();          //初期化処理
	void StageUpdate();            //シーン更新
	void EnemyJump();              //敵消滅
	void DrawScore();              //スコア数値表示
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	//BG(スプライト)
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;
	//ビュープロジェクション(共通)
	ViewProjection viewProjection_;
	//ステージ
	uint32_t textureHandleStage_ = 0;
	Model* modelStage_ = nullptr;
	WorldTransform worldTransformStage_[20];
	//プレイヤー
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;
	int playerLife_ = 3;
	//ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_[10];
	//ビーム存在フラグ
	int beamFlag_[10] = {};
	//ビーム発射タイマー
	int beamTimer_ = 0;
	//敵
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = 0;
	WorldTransform worldTransformEnemy_[10];
	//敵存在フラグ
	int enemyFlag_[10] = {};
	//敵のスピード
	float enemySpeed_[10] = {};
	//敵ジャンプの移動速度
	float enemyJumpSpeed_[10] = {};
	//スコア
	int gameScore_ = 0;
	//サウンド
	uint32_t soundDataHandleTitleBGM_ = 0;     //タイトルBGM
	uint32_t soundDataHandleGamePlayBGM_ = 0;  //ゲームプレイBGM
	uint32_t soundDataHandleGameOverBGM_ = 0;  //ゲームオーバーBGM
	uint32_t soundDataHandleEnemyHitSE_ = 0;   //敵ヒットSE
	uint32_t soundDataHandlePlayerHitSE_ = 0; //プレイヤーヒットSE
	uint32_t voiceHandleBGM_ = 0;              //音声再生ハンドル
	//シーンモード(0：ゲームプレイ　1：ゲームタイトル)
	int sceneMode_ = 1;
	//タイトル(スプライト)
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = nullptr;
	uint32_t  gameTimer_ = 0;

	uint32_t textureHandleEnter_ = 0;
	Sprite* spriteEnter_ = nullptr;

	uint32_t textureHandleEnding_ = 0;
	Sprite* spriteEnding_ = nullptr;

	uint32_t textureHandleEnding2_ = 0;
	Sprite* spriteEnding2_ = nullptr;
	//スコア数値(スプライト)
	uint32_t textureHandleNumber_ = 0;
	Sprite* spriteNumber_[5] = {};

	//スコア文字
	uint32_t textureHandleScore_ = 0;
	Sprite* spriteNumberScore_ = 0;
	//プレイヤー残機
	//uint32_t textureHandlePlayer_ = 0;
	Sprite* spriteNumberPlayer1_ = 0;
	Sprite* spriteNumberPlayer2_ = 0;
	Sprite* spriteNumberPlayer3_ = 0;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
