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
	textureHandleStage_ = TextureManager::Load("stage.jpg");
	modelStage_ = Model::Create();
	worldTransformStage_.translation_ = {0, -1.5f, 0};
	worldTransformStage_.scale_ = {4.5f, 1, 40};
	worldTransformStage_.Initialize();
	//プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();
	//ビーム
	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	worldTransformBeam_.scale_ = {0.33f, 0.33f, 0.33f};
	worldTransformBeam_.Initialize();
	//敵
	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	worldTransformEnemy_.scale_ = {0.33f, 0.33f, 0.33f};
	worldTransformEnemy_.Initialize();
	
	//サウンド
	// soundDataHandle_ = audio_->LoadWave("se_sad03.wav");
	// audio_->PlayWave(soundDataHandle_);
	// voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
}
//更新
void GameScene::Update() {
	//プレイヤー更新
	PlayerUpdate();
	//ビーム更新
	BeamUpdate();
	EnemyUpdate();


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
}
//プレイヤー更新
void GameScene::PlayerUpdate() {
	//移動
	//右へ移動
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransformPlayer_.translation_.x += 0.1f;
	}
	//左へ移動
	if (input_->PushKey(DIK_LEFT)) {
		worldTransformPlayer_.translation_.x -= 0.1f;
	}
	if (worldTransformPlayer_.translation_.x <= -4) 
	{
		worldTransformPlayer_.translation_.x = -4;
	}
	if (worldTransformPlayer_.translation_.x >= 4) 
	{
		worldTransformPlayer_.translation_.x = 4;
	}
	//行列更新
	worldTransformPlayer_.UpdateMatrix();
}
//ビーム更新
void GameScene::BeamUpdate() 
{
	//発射
	BeamBorn();
	//移動
	BeamMove();

	//行列更新
	worldTransformBeam_.UpdateMatrix();
}
//ビーム移動
void GameScene::BeamMove() 
{
	if (beamFlag_ == 1)
	{
		worldTransformBeam_.translation_.z+= 0.5f;
		worldTransformBeam_.rotation_.x += 0.1f;
		if (worldTransformBeam_.translation_.z >= 40)
		{
			beamFlag_ = 0;
			worldTransformBeam_.translation_.z = 0;
		}
	}
}
//ビーム発射
void GameScene::BeamBorn() 
{
if (beamFlag_ == 0)
	{
		if (input_->PushKey(DIK_SPACE))
		{
			worldTransformBeam_.translation_.x = worldTransformPlayer_.translation_.x;
			worldTransformBeam_.translation_.z += 0.1f;
			beamFlag_ = 1;
		}
	}
}
//敵更新
void GameScene::EnemyUpdate() 
{
	//敵移動
	EnemyMove(); 
	//敵発生
	EnemyBorn();
	worldTransformEnemy_.UpdateMatrix();
}
//敵移動
void GameScene::EnemyMove()
{
	if (enemyFlag_ == 1) {
		worldTransformEnemy_.translation_.z-= 0.2f;
		worldTransformEnemy_.rotation_.x -= 0.1f;
		if (worldTransformEnemy_.translation_.z <= -5) 
		{
			enemyFlag_ = 0;
			worldTransformEnemy_.translation_.z = 40;
		}
	}
};
//敵発生
void GameScene::EnemyBorn() 
{ 
	if (enemyFlag_ == 0) 
	{
		int x = rand() % 80;
		float x2 = (float)x / 10 - 4;
		worldTransformEnemy_.translation_.x = x2;
		worldTransformEnemy_.translation_.z = 40;
		enemyFlag_ = 1;
		//worldTransformEnemy_.translation_.x = worldTransformPlayer_.translation_.x;
		//worldTransformEnemy_.translation_.z -= 0.1f;
	}
};
	//表示
void GameScene::Draw() 
{

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	spriteBG_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//ステージ
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);
	//プレイヤー
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
	//ビーム
	if (beamFlag_ == 1) 
	{
		modelBeam_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_);
	}
	//敵
	if (enemyFlag_ == 1) 
	{
		modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandleEnemy_);
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
