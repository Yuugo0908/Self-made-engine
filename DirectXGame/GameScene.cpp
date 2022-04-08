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
	enemyModel = enemyModel->CreateFromObject("GreenBox");

	enemyBulletModel = enemyBulletModel->CreateFromObject("RedBullet");
	skydomeModel = skydomeModel->CreateFromObject("skydome");

	// 3Dオブジェクト生成
	playerObj = Object3d::Create();
	enemyObj = Object3d::Create();
	skydomeObj = Object3d::Create();
	enemyBulletObj = Object3d::Create();
	enemyBulletObj2 = Object3d::Create();
	enemyBulletObj3 = Object3d::Create();
	// 3Dオブジェクトにモデルを割り当てる
	playerObj->SetModel(playerModel);
	enemyObj->SetModel(enemyModel);
	skydomeObj->SetModel(skydomeModel);
	enemyBulletObj->SetModel(enemyBulletModel);
	enemyBulletObj2->SetModel(enemyBulletModel);
	enemyBulletObj3->SetModel(enemyBulletModel);

	playerObj->SetPosition({ 0.0f, 0.0f, -30.0f });
	playerObj->SetScale({ 1.0f,1.0f,1.0f });
	enemyObj->SetPosition({ 0.0f, 0.0f, 70.0f });
	enemyObj->SetScale({ 1.0f,1.0f,1.0f });

	skydomeObj->SetScale({ 1.0f, 1.0f, 1.0f });

	enemyBulletObj->SetPosition({ 0.0f, 0.0f, 70.0f });
	enemyBulletObj2->SetPosition({ 0.0f, 0.0f, 70.0f });
	enemyBulletObj3->SetPosition({ 0.0f, 0.0f, 70.0f });
	enemyBulletObj->SetScale({ 2.0f,2.0f,2.0f });
	enemyBulletObj2->SetScale({ 2.0f,2.0f,2.0f });
	enemyBulletObj3->SetScale({ 2.0f,2.0f,2.0f });

	playerPos = playerObj->GetPosition();
	enemyPos = enemyObj->GetPosition();
	enemyBulletPos = enemyBulletObj->GetPosition();
	enemyBulletPos2 = enemyBulletObj2->GetPosition();
	enemyBulletPos3 = enemyBulletObj3->GetPosition();
}

void GameScene::Update() {
	// 現在の座標を取得
	XMFLOAT3 cameraEye = Object3d::GetEye();
	XMFLOAT3 cameraTarget = Object3d::GetTarget();
	if (nowScene == 0)// タイトル
	{
		if (input->TriggerKey(DIK_SPACE)) {
			playAudio->PlayWave("Resources/BGM/bgm.wav", 255, 0.08);
			nowScene = 1;
		}
	}

	if (nowScene == 1) {
		DodgeTimer++;
		Move();
		collision();
		enemyMove();
		enemyAttack();

		if (DodgeTimer == 900) {
			nowScene = 2;
		}

		if (playerPos.x >= 50.0f) {
			playerPos.x = 50.0f;
		}
		if (playerPos.x <= -50.0f) {
			playerPos.x = -50.0f;
		}

		if (enemyPos.x >= 50.0f) {
			enemyPos.x = 50.0f;
			enemyMoveX = -enemyMoveX;
		}	
		if (enemyPos.x <= -50.0f) {
			enemyPos.x = -50.0f;
			enemyMoveX = -enemyMoveX;
		}
	}

	if (nowScene == 2) {
		collisionFlag = false;
		collisionFlag2 = false;
		collisionFlag3 = false;
		enemyAttackFlag = false;
		BulletFlag = false;
		BulletFlag2 = false;
		BulletFlag3 = false;

		enemyAttackCount = 0;
		if (input->TriggerKey(DIK_SPACE)) {
			reset();
		}
	}

	if (nowScene == 3) {
		collisionFlag = false;
		collisionFlag2 = false;
		collisionFlag3 = false;
		enemyAttackFlag = false;
		BulletFlag = false;
		BulletFlag2 = false;
		BulletFlag3 = false;

		enemyAttackCount = 0;
		if (input->TriggerKey(DIK_SPACE)) {
			reset();
		}
	}

	Object3d::SetEye(cameraEye);
	Object3d::SetTarget(cameraTarget);
	playerObj->Update();
	enemyObj->Update();
	skydomeObj->Update();
	enemyBulletObj->Update();
	enemyBulletObj2->Update();
	enemyBulletObj3->Update();
}

void GameScene::reset() {

	playerObj->SetPosition({ 0.0f, 0.0f, -30.0f });
	playerObj->SetScale({ 1.0f,1.0f,1.0f });
	enemyObj->SetPosition({ 0.0f, 0.0f, 70.0f });
	enemyObj->SetScale({ 1.0f,1.0f,1.0f });

	skydomeObj->SetScale({ 1.0f, 1.0f, 1.0f });

	enemyBulletObj->SetPosition({ 0.0f, 0.0f, 70.0f });
	enemyBulletObj2->SetPosition({ 0.0f, 0.0f, 70.0f });
	enemyBulletObj3->SetPosition({ 0.0f, 0.0f, 70.0f });
	enemyBulletObj->SetScale({ 2.0f,2.0f,2.0f });
	enemyBulletObj2->SetScale({ 2.0f,2.0f,2.0f });
	enemyBulletObj3->SetScale({ 2.0f,2.0f,2.0f });

	playerPos = playerObj->GetPosition();
	enemyPos = enemyObj->GetPosition();
	enemyBulletPos = enemyBulletObj->GetPosition();
	enemyBulletPos2 = enemyBulletObj2->GetPosition();
	enemyBulletPos3 = enemyBulletObj3->GetPosition();

	nowScene = 0;
	collisionFlag = false;
	collisionFlag2 = false;
	collisionFlag3 = false;
	enemyAttackFlag = false;
	BulletFlag = false;
	BulletFlag2 = false;
	BulletFlag3 = false;

	enemyAttackCount = 0;

	collisionX = 0.0f;
	collisionY = 0.0f;
	collisionZ = 0.0f;
	Collision = 0.0f;

	collisionX2 = 0.0f;
	collisionY2 = 0.0f;
	collisionZ2 = 0.0f;
	Collision2 = 0.0f;

	collisionX3 = 0.0f;
	collisionY3 = 0.0f;
	collisionZ3 = 0.0f;
	Collision3 = 0.0f;

	PlayerHP = 360;
	DodgeTimer = 0;
	playAudio->Initialize();
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
	enemyObj->Draw();
	skydomeObj->Draw();

	if (BulletFlag == true && enemyAttackFlag == true) {
		enemyBulletObj->Draw();
	}
	if (BulletFlag2 == true && enemyAttackFlag == true) {
		enemyBulletObj2->Draw();
	}
	if ( BulletFlag3 == true && enemyAttackFlag == true) {
		enemyBulletObj3->Draw();
	}

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion 3Dオブジェクト描画
#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	// 前景スプライトの描画
	if (nowScene == 0) {
		title->Draw();
	}

	if (nowScene == 1) {
		HPText->Draw();
		PlayerHPBar->Draw();
		TimerBar->Draw();

		PlayerHPGauge->SetSize({ PlayerHP,60 });
		PlayerHPGauge->Draw();

		TimerGauge->SetSize({DodgeTimer * 2 / 5, 60});
		TimerGauge->Draw();
	}

	if (nowScene == 2) {
		GameClear->Draw();
	}

	if (nowScene == 3) {
		GameOver->Draw();
	}
	// デバッグテキストの描画
	debugText.DrawAll(dxCommon->GetCommandList());
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion 前景スプライト描画
}

void GameScene::Move() {

	if (input->PushKey(DIK_D)) {
		playerPos.x += moveAmount;
	}
	
	else if (input->PushKey(DIK_A)) {
		playerPos.x -= moveAmount;
	}
	playerObj->SetPosition(playerPos);
}

void GameScene::collision() {
	collisionX = playerPos.x - enemyBulletPos.x;
	collisionY = playerPos.y - enemyBulletPos.y;
	collisionZ = playerPos.z - enemyBulletPos.z;
	Collision = sqrtf((collisionX * collisionX) + (collisionY * collisionY) + (collisionZ * collisionZ));

	if (Collision <= playerRadius + enemyBulletRadius) {
		collisionFlag = true;
	} else {
		collisionFlag = false;
	}

	collisionX2 = playerPos.x - enemyBulletPos2.x;
	collisionY2 = playerPos.y - enemyBulletPos2.y;
	collisionZ2 = playerPos.z - enemyBulletPos2.z;
	Collision2 = sqrtf((collisionX2 * collisionX2) + (collisionY2 * collisionY2) + (collisionZ2 * collisionZ2));

	if (Collision2 <= playerRadius + enemyBulletRadius) {
		collisionFlag2 = true;
	} else {
		collisionFlag2 = false;
	}

	collisionX3 = playerPos.x - enemyBulletPos3.x;
	collisionY3 = playerPos.y - enemyBulletPos3.y;
	collisionZ3 = playerPos.z - enemyBulletPos3.z;
	Collision3 = sqrtf((collisionX3 * collisionX3) + (collisionY3 * collisionY3) + (collisionZ3 * collisionZ3));

	if (Collision3 <= playerRadius + enemyBulletRadius) {
		collisionFlag3 = true;
	} else {
		collisionFlag3 = false;
	}
}

void GameScene::enemyMove() {
	enemyPos.x += enemyMoveX;
	enemyObj->SetPosition(enemyPos);
}

void GameScene::enemyAttack() {
	enemyAttackCount++;

	if (enemyAttackCount == 60) {

		if (enemyAttackFlag == false) {
			enemyBulletPos = enemyPos;
			enemyBulletPos2 = enemyPos;
			enemyBulletPos3 = enemyPos;
			enemyAttackFlag = true;
			BulletFlag = true;
			BulletFlag2 = true;
			BulletFlag3 = true;
		}

		shot->PlayWave("Resources/SE/shot.wav", 0, 0.12);
		enemyAttackCount = 0;
	}

	if (enemyAttackFlag == true) {

		enemyBulletPos.z -= 3.0f;

		enemyBulletPos2.x += 0.5;
		enemyBulletPos2.z -= 3.0f;

		enemyBulletPos3.x -= 0.5f;
		enemyBulletPos3.z -= 3.0f;

		if (BulletFlag == true && collisionFlag == true) {
			BulletFlag = false;
			hit->PlayWave("Resources/SE/hit.wav", 0, 0.12);
			PlayerHP -= 180;
		}

		if (BulletFlag2 == true && collisionFlag2 == true) {
			BulletFlag2 = false;
			hit->PlayWave("Resources/SE/hit.wav", 0, 0.1);
			PlayerHP -= 180;
		}

		if (BulletFlag3 == true && collisionFlag3 == true) {
			BulletFlag3 = false;
			hit->PlayWave("Resources/SE/hit.wav", 0, 0.1);
			PlayerHP -= 180;
		}

		if (enemyBulletPos.z <= -60) {
			enemyAttackFlag = false;
		}
	}

	if (PlayerHP <= 0) {
		nowScene = 3;
	}

	enemyBulletObj->SetPosition(enemyBulletPos);
	enemyBulletObj2->SetPosition(enemyBulletPos2);
	enemyBulletObj3->SetPosition(enemyBulletPos3);
}