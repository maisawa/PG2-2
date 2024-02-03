#include <Novice.h>
#include "Player.h"
#include "Enemy.h"
#include <corecrt_math.h>

const char kWindowTitle[] = "GC1B_14_マイサワ_ショウマ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	int kWindowWidth = 1280; // ウィンドウの横幅
	int kWindowHeight = 720; // ウィンドウの縦幅

	enum Scene {
		SCENE1,//title
		SCENE2,//gameplay
	};

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	Player* player = new Player({ kWindowWidth / 2,kWindowHeight / 2 }, 16, 0, false);

	Enemy* enemy = new Enemy({ 100,300 }, 16, 10, false);

	int currentScene = Scene::SCENE1; //firstbackground

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		switch (currentScene) {
		case SCENE1:
			if (keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
				currentScene = SCENE2;
			}
			

			Novice::ScreenPrintf(620, 310, "STG");
			Novice::ScreenPrintf(590, 350, "Press SPACE");
			Novice::DrawBox(0, 0, kWindowWidth, kWindowHeight, 0.0f, BACKGROUND_BLUE, kFillModeSolid);
			//-------------------------------------------------
			break;
		case SCENE2:
			player->Update(keys, preKeys);

			enemy->Update(keys, preKeys);

			int distance1 = player->bullet_->GetPosX() - enemy->GetPosX();
			int distance2 = player->bullet_->GetPosY() - enemy->GetPosY();
			int distance3 = enemy->GetPosX() - player->GetPosX();
			int distance4 = enemy->GetPosY() - player->GetPosY();

			int a = (int)sqrtf((float)distance1 * (float)distance1 + (float)distance2 * (float)distance2);
			int b = (int)sqrtf((float)distance3 * (float)distance3 + (float)distance4 * (float)distance4);

			if (a <= (player->bullet_->GetRadius() + enemy->GetRadius())) {
				enemy->SetAlive(true);
				enemy->SetSpeedX(0);
			}
			if (b <= (enemy->GetRadius() + player->GetRadius())) {
				player->SetAlive(true);
			}
			

			player->Draw();
			enemy->Draw();

			break;
		}

		///
		/// ↑更新処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	delete player;

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
