#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {

}

GameScene::~GameScene() {

}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio) {
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->playAudio = audio;
	this->shot = audio;
	this->hit = audio;
	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
	}
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(0, L"Resources/title.png")) {
		assert(0);
	}

	if (!Sprite::LoadTexture(1, L"Resources/HPText.png")) {
		assert(0);
	}

	if (!Sprite::LoadTexture(2, L"Resources/HPBar.png")) {
		assert(0);
	}

	if (!Sprite::LoadTexture(3, L"Resources/PlayerHPGauge.png")) {
		assert(0);
	}

	if (!Sprite::LoadTexture(4, L"Resources/TimerGauge.png")) {
		assert(0);
	}

	if (!Sprite::LoadTexture(5, L"Resources/GameClear.png")) {
		assert(0);
	}

	if (!Sprite::LoadTexture(6, L"Resources/GameOver.png")) {
		assert(0);
	}
	//スプライト
	title = Sprite::Create(0, { 0.0f,0.0f });
	title->SetSize({ 1280.0f,720.0f });

	HPText = Sprite::Create(1, { 0.0f,0.0f });
	HPText->SetSize({ 1280.0f,720.0f });

	PlayerHPBar = Sprite::Create(2, { 0.0f,45.0f });
	PlayerHPBar->SetSize({ 360.0f,60.0f });

	PlayerHPGauge = Sprite::Create(3, { 0.0f,45.0f });
	PlayerHPGauge->SetSize({ 30.0f,60.0f });

	TimerBar = Sprite::Create(2, { 0.0f,160.0f });
	TimerBar->SetSize({ 360.0f,60.0f });

	TimerGauge = Sprite::Create(4, { 0.0f,160.0f });
	TimerGauge->SetSize({ 30.0f,60.0f });

	GameClear = Sprite::Create(5, { 0.0f,0.0f });
	GameClear->SetSize({ 1280.0f,720.0f });

	GameOver = Sprite::Create(6, { 0.0f,0.0f });
	GameOver->SetSize({ 1280.0f,720.0f });

	//.objの名前を指定してモデルを読み込む
	playerModel = playerModel->CreateFromObject("RedBox");
	skydomeModel = skydomeModel->CreateFromObject("skydome");

	//モデル名を指定して読み込み
	FbxLoader::GetInstance()->LoadModelFromFile("cube");

	// 3Dオブジェクト生成
	playerObj = Object3d::Create();
	skydomeObj = Object3d::Create();

	// 3Dオブジェクトにモデルを割り当てる
	playerObj->SetModel(playerModel);
	skydomeObj->SetModel(skydomeModel);


	playerObj->SetPosition({ 0.0f, 30.0f, 0.0f });
	playerObj->SetScale({ 1.0f,1.0f,1.0f });

	skydomeObj->SetScale({ 1.0f, 1.0f, 1.0f });

	playerPos = playerObj->GetPosition();
}

void GameScene::Update() {
	// 現在の座標を取得
	XMFLOAT3 cameraEye = Object3d::GetEye();
	XMFLOAT3 cameraTarget = Object3d::GetTarget();


	Object3d::SetEye(cameraEye);
	Object3d::SetTarget(cameraTarget);
	playerObj->Update();
	skydomeObj->Update();
}

void GameScene::reset() {

}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());
	// 背景スプライト描画
	//backGround->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion 背景スプライト描画
#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon->GetCommandList());
	// 3Dオブクジェクトの描画
	
	playerObj->Draw();
	skydomeObj->Draw();

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	// 前景スプライトの描画
	
	// デバッグテキストの描画
	debugText.DrawAll(dxCommon->GetCommandList());
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}