#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;
//コンストラクタ
GameScene::GameScene() {}

//デストラクタ
GameScene::~GameScene() {
	delete spriteBG_;
	delete modelStage_;
	delete modelPlayer_;
	delete spriteTitle_;
	delete spriteEnter_;
}
//初期化
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});
	//ビュープロジェックションの初期化
	viewProjection_.eye = {0, 1, -6};
	viewProjection_.target = {0, 1, 0};
	viewProjection_.Initialize();
	//ステージ
	textureHandleStage_ = TextureManager::Load("stage2.jpg");
	modelStage_ = Model::Create();
	for (int s = 0; s < 20; s++)
	{
	worldTransformStage_[s].translation_ = {0, -1.5f, 2.0f * s - 5};
	worldTransformStage_[s].scale_ = {4.5f, 1, 1};
	worldTransformStage_[s].Initialize();
	}
	//プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();
	//ビーム
	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	for (int b = 0; b < 10; b++) {
		worldTransformBeam_[b].scale_ = {0.33f, 0.33f, 0.33f};
		worldTransformBeam_[b].Initialize();
	}
	//敵
	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	for (int e = 0; e < 10; e++) {
		worldTransformEnemy_[e].scale_ = {0.5f, 0.5f, 0.5f};
		worldTransformEnemy_[e].Initialize(/*dxCommon_->GetDevice()*/);
	}
	//サウンドデータの読み込み
	soundDataHandleTitleBGM_ = audio_->LoadWave("Audio/Ring05.wav");
	soundDataHandleGamePlayBGM_ = audio_->LoadWave("Audio/Ring08.wav");
	soundDataHandleGameOverBGM_ = audio_->LoadWave("Audio/Ring09.wav");
	soundDataHandleEnemyHitSE_ = audio_->LoadWave("Audio/chord.wav");
	soundDataHandlePlayerHitSE_ = audio_->LoadWave("Audio/tada.wav");
	//タイトルBGMを再生
	voiceHandleBGM_ = audio_->PlayWave(soundDataHandleTitleBGM_, true);
	//タイトル(2Dスプライト)
	textureHandleTitle_ = TextureManager::Load("title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});

	//エンター
	textureHandleEnter_ = TextureManager::Load("enter.png");
	spriteEnter_ = Sprite::Create(textureHandleEnter_, {800 / 2, 500});

	//エンディング
	textureHandleEnding_ = TextureManager::Load("gameover.png");
	spriteEnding_ = Sprite::Create(textureHandleEnding_, {0, 0});
	//スコア数値(2Dスプライト)
	textureHandleNumber_ = TextureManager::Load("number.png");
	for (int i = 0; i < 5; i++) {
		spriteNumber_[i] = Sprite::Create(textureHandleNumber_, {300.0f + i * 26, 0});
	}
	//スコア
	textureHandleScore_ = TextureManager::Load("score.png");
	spriteNumberScore_ = Sprite::Create(textureHandleScore_, {180, 0});

	//プレイヤー残機
	textureHandlePlayer_ = TextureManager::Load("player.png");
	spriteNumberPlayer1_ = Sprite::Create(textureHandlePlayer_, {900, 0});
	spriteNumberPlayer2_ = Sprite::Create(textureHandlePlayer_, {950, 0});
	spriteNumberPlayer3_ = Sprite::Create(textureHandlePlayer_, {1000, 0});

}
//更新
void GameScene::Update() {
	gameTimer_++;
	CollisionEnemy();
	//プレイヤー更新
	// PlayerUpdate();
	//ビーム更新
	// BeamUpdate();
	//敵更新
	// EnemyUpdate();
	//衝突判定
	// Collision();
	// XMFLOAT2 position = sprite_-> GetPosition();
	// position.x += 2.0f;
	// position.y += 1.0f;
	// sprite_->SetPosition(position);
	// if (input_->TriggerKey(DIK_SPACE))
	//{
	//	audio_->StopWave(voiceHandle_);
	// }
	// value_++;
	// std::string strDebug = std::string("Value:") + std::to_string(value_);
	// debugText_->Print(strDebug, 50, 50, 1.0f);
	//各シーンの更新処理を呼び出す
	switch (sceneMode_) {
	case 0:

		//ゲームプレイ更新
		GamePlayUpdate();
		break;
	case 1:
		//タイトル
		TitleUpdate();
		break;
	case 2:
		//エンディング
		EndingUpdate();
		break;
	}
}
//ステージ更新
void GameScene::StageUpdate() {
	//各ステージでループ
	for (int s = 0;s < 20;s++) {
	//手前に移動
		worldTransformStage_[s].translation_.z -= 0.1f;
		//端まで来たら奥へ戻る
		if (worldTransformStage_[s].translation_.z < -5) {
			worldTransformStage_[s].translation_.z += 40;
		}
		//行列更新
		worldTransformStage_[s].UpdateMatrix();
	}
}
//ゲームプレイ更新
void GameScene::GamePlayUpdate() {
	PlayerUpdate(); //プレイヤー更新
	EnemyUpdate();  //敵更新
	BeamUpdate();   //ビーム更新
	Collision();    //衝突判定
	StageUpdate();

}
//プレイヤー更新
void GameScene::PlayerUpdate() {
	//移動
	//右へ移動
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransformPlayer_.translation_.x += 0.1f;
	}
	//右移動制限
	if (worldTransformPlayer_.translation_.x >= 4) {
		worldTransformPlayer_.translation_.x = 4;
	}
	//左へ移動
	if (input_->PushKey(DIK_LEFT)) {
		worldTransformPlayer_.translation_.x -= 0.1f;
	}
	//左移動制限
	if (worldTransformPlayer_.translation_.x <= -4) {
		worldTransformPlayer_.translation_.x = -4;
	}
	//行列更新
	worldTransformPlayer_.UpdateMatrix();
}
//ビーム更新
void GameScene::BeamUpdate() {
	//発射
	BeamBorn();
	//移動
	BeamMove();
	for (int b = 0; b < 10; b++) {

		//行列更新
		worldTransformBeam_[b].UpdateMatrix();
	}
}
//ビーム移動
void GameScene::BeamMove() {
	for (int b = 0; b < 10; b++) {

		if (beamFlag_[b] == 1) {
			worldTransformBeam_[b].translation_.z += 0.5f;
			worldTransformBeam_[b].rotation_.x += 0.1f;
			if (worldTransformBeam_[b].translation_.z >= 40) {
				beamFlag_[b] = 0;
				worldTransformBeam_[b].translation_.z = 0;
			}
		}
	}
}
//ビーム発射
void GameScene::BeamBorn() {
	if (beamTimer_ == 0) {
		for (int b = 0; b < 10; b++) {
			if (beamFlag_[b] == 0) {
				if (input_->PushKey(DIK_SPACE)) {
					worldTransformBeam_[b].translation_.x = worldTransformPlayer_.translation_.x;
					worldTransformBeam_[b].translation_.z += 0.1f;
					beamFlag_[b] = 1;
					beamTimer_ = 1;
					break;
				}
			}
		}
	} else {
		//発射タイマーが1以上
		// 10を超えると再び発射が可能
		beamTimer_++;
		if (beamTimer_ > 10) {
			beamTimer_ = 0;
		}
	}
}
//敵更新
void GameScene::EnemyUpdate() {
	//敵移動
	EnemyMove();
	//敵発生
	EnemyBorn();
	//敵消滅
	EnemyJump();
	for (int e = 0; e < 10; e++) {
		worldTransformEnemy_[e].UpdateMatrix();
	}
}
//敵移動
void GameScene::EnemyMove() 
{
	for (int e = 0; e < 10; e++) 
	{
		if (enemyFlag_[e] == 1) 
		{
			worldTransformEnemy_[e].translation_.z -= 0.2f;
			worldTransformEnemy_[e].rotation_.x -= 0.1f;
			if (worldTransformEnemy_[e].translation_.z <= -5) 
			{
				enemyFlag_[e] = 0;
				worldTransformEnemy_[e].translation_.z = 40;
			}
			worldTransformEnemy_[e].translation_.x += enemySpeed_[e];
	if (worldTransformEnemy_[e] .translation_.x <= -4) 
	{
				enemySpeed_[e] = 0.2f;
	}
	if (worldTransformEnemy_[e].translation_.x >= 4) 
	{
		enemySpeed_[e] = -0.2f;
	}
		}
	}

	//for (int e = 0; e < 10; e++) {
	//	//敵スピード
	//	if (rand() % 2 == 0) {
	//		// worldTransformEnemy_[e].translation_.z -= 0.2f;
	//		// worldTransformEnemy_[e].rotation_.x -= 0.1f;
	//		enemySpeed_[e] = 0.2f;
	//	} else {
	//		// worldTransformEnemy_[e].translation_.z = 40;
	//		enemySpeed_[e] = -0.2f;
	//	}
	//}
};
//敵発生
void GameScene::EnemyBorn() {
	//乱数で発生
	if (rand() % 10 == 0) {
		//敵でループ
		for (int e = 0; e < 10; e++) {
			//存在しなければ
			if (enemyFlag_[e] == 0) {
				int x = rand() % 80;
				float x2 = (float)x / 10 - 4;
				worldTransformEnemy_[e].translation_.x = x2;
				worldTransformEnemy_[e].translation_.z = 40;
				enemyFlag_[e] = 1;
			//敵スピード
			if (rand() % 2 == 0) {

				enemySpeed_[e] = 0.2f;
			} else {
				enemySpeed_[e] = -2.0f;
			}
				//ループ終了
				break;
			}
		}/*
		for (int e = 0; e < 10; e++) {
		}*/
	}
	// if (enemyFlag_ == 0) {
	//	int x = rand() % 80;
	//	float x2 = (float)x / 10 - 4;
	//	worldTransformEnemy_.translation_.x = x2;
	//	worldTransformEnemy_.translation_.z = 40;
	//	enemyFlag_ = 1;
	//	// worldTransformEnemy_.translation_.x = worldTransformPlayer_.translation_.x;
	//	// worldTransformEnemy_.translation_.z -= 0.1f;
	// }
};
//衝突判定呼び出し(プレイヤーと敵)
void GameScene::Collision() {
	//衝突判定
	CollisionPlayerEnemy();
}
//衝突判定呼び出し(敵と弾)
void GameScene::CollisionEnemy() {
	//衝突判定
	CollisionBeamEnemy();
}
//衝突判定(プレイヤーと敵)
void GameScene::CollisionPlayerEnemy() {

	//敵が存在すれば
	for (int e = 0; e < 10; e++) {
		if (enemyFlag_[e] == 1) {
			//差を求める
			float dx =
			  abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_[e].translation_.x);
			float dz =
			  abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_[e].translation_.z);
			//衝突したら
			if (dx < 1 && dz < 1) {
				playerLife_ -= 1;
				//存在しない
				enemyFlag_[e] = 0;
				//プレイヤーヒットSE
				audio_->PlayWave(soundDataHandlePlayerHitSE_);
				if (playerLife_ == 0) {
					//モードをゲームプレイへ変更
					sceneMode_ = 2;
					// BGM切り替え
					audio_->StopWave(voiceHandleBGM_); // BGMを停止
					voiceHandleBGM_ =
					  audio_->PlayWave(soundDataHandleGameOverBGM_, true); //ゲームオーバーBGMを再生
	
				}
			}
		}
	}
}
//衝突判定(敵と弾)
void GameScene::CollisionBeamEnemy() {
	//敵でループ
	for (int e = 0; e < 10; e++) {
		//敵が存在すれば
		if (enemyFlag_[e] == 1) {
			//ビームでループ
			for (int b = 0; b < 10; b++) {
				//ビームが存在すれば
				if (beamFlag_[b] == 1) {
					//差を求める
					float dx = abs(
					  worldTransformBeam_[b].translation_.x -
					  worldTransformEnemy_[e].translation_.x);
					float dz = abs(
					  worldTransformBeam_[b].translation_.z -
					  worldTransformEnemy_[e].translation_.z);
					//衝突したら
					if (dx < 1 && dz < 1) {
						//プレイヤーヒットSE
						audio_->PlayWave(soundDataHandleEnemyHitSE_);
						//スコア加算
						gameScore_ += 1;
						//弾初期化
						worldTransformBeam_[b].translation_.z = 0;
						//存在しない
						enemyFlag_[e] = 2;
						beamFlag_[b] = 0;
						enemyJumpSpeed_[e] = 1;
					}
				}
			}
		}
	}
	////敵と弾が存在すれば
	// if (enemyFlag_ == 1 && beamFlag_ == 1)
	//{
	//	//差を求める
	//	float dx = abs(worldTransformBeam_.translation_.x - worldTransformEnemy_.translation_.x);
	//	float dz = abs(worldTransformBeam_.translation_.z - worldTransformEnemy_.translation_.z);
	//	//衝突したら
	//	if (dx < 1 && dz < 1)
	//	{
	//		//スコア加算
	//		gameScore_ += 1;
	//		//弾初期化
	//		worldTransformBeam_.translation_.z = 0;
	//		//存在しない
	//		enemyFlag_ = 0;
	//		beamFlag_ = 0;
	//	}
	// }
}
//敵ジャンプ
void GameScene::EnemyJump() 
{ 
	//敵でループ
	for (int e = 0;e < 10;e++) 
	{ 
		//消滅演出ならば
		if (enemyFlag_[e] == 2)
		{
			//移動(Y座標に速度を加える)
			worldTransformEnemy_[e].translation_.y += enemyJumpSpeed_[e];
			//速度を減らす
			enemyJumpSpeed_[e] -= 0.1f;
			//斜め移動
			worldTransformEnemy_[e].translation_.x += enemySpeed_[e] * 2;
			//下は落ちると消滅
			if (worldTransformEnemy_[e].translation_.y < -3) 
			{
				//存在しない
				enemyFlag_[e] = 0;
				worldTransformEnemy_[e].translation_.y = 0;
			}
		}
	}
}
//表示
void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	//各シーンの背景2D表示を呼び出す
	switch (sceneMode_) {
	case 0:
		//ゲームプレイ2D背景表示
		GamePlayDraw2DBack();
		break;
	case 1:
		//タイトル2D表示
		TitleDraw2DNear();
		break;
	case 2:
		//エンディング
		GameOverDraw2DNear();
		break;
	}
	/// </summary>

	// spriteBG_->Draw();
	//  スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	//各シーンの背景3D表示を呼び出す
	switch (sceneMode_) {
	case 0:
		GamePlayDraw3D(); //ゲームプレイ3D背景表示
		break;
	case 2:
		GamePlayDraw3D(); //ゲームプレイ3D背景表示
		break;
	}

	/// </summary>
	////ステージ
	// modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);
	////プレイヤー
	// modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
	////ビーム
	// if (beamFlag_ == 1) {
	//	modelBeam_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_);
	// }
	////敵
	// if (enemyFlag_ == 1) {
	//	modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandleEnemy_);
	// }
	//  3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	//各シーンの近景2D表示を呼び出す
	switch (sceneMode_) {
	case 0:
		GamePlayDraw2DNear(); //ゲームプレイ2D近景表示
		break;
	case 2:
		//エンディング
		Ending2Draw();
		break;
	}
	//ゲームスコア
	// char str[100];
	// sprintf_s(str, "SCORE %d", gameScore_);
	// debugText_->Print(str, 200, 10, 2);
	// char str2[100];
	// sprintf_s(str2, "PLAYERLIFE %d", playerLife_);
	// debugText_->Print(str2, 600, 10, 2);
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
//ゲームプレイ表示3D
void GameScene::GamePlayDraw3D() {
	//ステージ
	for (int s = 0;s < 20;s++) 
	{
		modelStage_->Draw(worldTransformStage_[s], viewProjection_, textureHandleStage_);
	}
	//プレイヤー
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
	//ビーム
	for (int b = 0; b < 10; b++) {
		if (beamFlag_[b] == 1) {
			modelBeam_->Draw(worldTransformBeam_[b], viewProjection_, textureHandleBeam_);
		}
	}
	//敵
	for (int e = 0; e < 10; e++) {
		if (enemyFlag_[e] == 1 || enemyFlag_[e] ==2) {
			modelEnemy_->Draw(worldTransformEnemy_[e], viewProjection_, textureHandleEnemy_);
		}
	}
}
//ゲームプレイ表示2D背景
void GameScene::GamePlayDraw2DBack() {
	//背景
	spriteBG_->Draw();
}
//ゲームプレイ表示2D近景
void GameScene::GamePlayDraw2DNear() 
{
	DrawScore();
	/*char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	char str2[100];
	sprintf_s(str2, "PLAYERLIFE %d", playerLife_);
	debugText_->Print(str2, 600, 10, 2);*/
}
//タイトル更新
void GameScene::TitleUpdate() {
	//エンターキーを押した瞬間
	if (input_->TriggerKey(DIK_RETURN)) {
		GamePlayStart();
		//モードをゲームプレイへ変更
		sceneMode_ = 0;
		//BGM切り替え
		audio_->StopWave(voiceHandleBGM_);//BGMを停止
		voiceHandleBGM_ = audio_->PlayWave(soundDataHandleGamePlayBGM_, true);//ゲームプレイBGMを再生
	}
}
//タイトル表示2D
void GameScene::TitleDraw2DNear() {

	//タイトル表示
	spriteTitle_->Draw();

	if (gameTimer_ % 40 >= 20) {
		//エンター表示
		spriteEnter_->Draw();
	}
}
//エンディング更新
void GameScene::EndingUpdate() {
	//エンターキーを押した瞬間
	if (input_->TriggerKey(DIK_RETURN)) {
		//モードをゲームプレイへ変更
		sceneMode_ = 1;
	// BGM切り替え
	audio_->StopWave(voiceHandleBGM_);                                     // BGMを停止
	voiceHandleBGM_ = audio_->PlayWave(soundDataHandleTitleBGM_, true); //タイトルBGMを再生
	}
}
//エンディング表示2D
void GameScene::GameOverDraw2DNear() {

	//背景
	spriteBG_->Draw();
	//エンディング表示
	spriteEnding_->Draw();

	// if (gameTimer_ % 40 >= 20) {
	//  //エンター表示
	//  spriteEnter_->Draw();
	// }
}
//エンディング表示2D(エンター)
void GameScene::Ending2Draw() {
	if (gameTimer_ % 40 >= 20) {
		//エンター表示
		spriteEnter_->Draw();
	}
}
//スコア&プレイヤー&フラグ初期化
void GameScene::GamePlayStart() {
	
	playerLife_ = 3;
	gameScore_ = 0;
	for (int e = 0; e < 10; e++) {

		enemyFlag_[e] = 0;
	}
	for (int b = 0; b < 10; b++) {

		beamFlag_[b] = 0;
	}
}
//スコアの数値の表示
void GameScene::DrawScore()
{
	//各桁の数値を描画
	char eachNumber[5] = {};          //各桁の値
	int number = gameScore_;          //表示する数字

	int keta = 10000;                 //最初の桁
	spriteNumberScore_->Draw();       //スコア文字
	for ( int i = 0;i < 5;i++) {
		eachNumber[i] = number / keta;//今の桁の値を求める
		number = number % keta;       //次の桁以下の値を取り出す
		keta = keta / 10;             //桁を進める
	}
	for (int i = 0; i < 5; i++)
	{
		spriteNumber_[i]->SetSize({32, 64});
		spriteNumber_[i]->SetTextureRect({32.0f * eachNumber[i], 0}, {32, 64});
		spriteNumber_[i]->Draw();
	}
	//プレイヤー
	if (playerLife_ >= 3) {
	spriteNumberPlayer1_->SetSize({40, 40});
	spriteNumberPlayer1_->Draw();
	}
	if (playerLife_ >= 2) {
		spriteNumberPlayer2_->SetSize({40, 40});
		spriteNumberPlayer2_->Draw();
	}
	if (playerLife_ >= 1) {
		spriteNumberPlayer3_->SetSize({40, 40});
		spriteNumberPlayer3_->Draw();
	}
}