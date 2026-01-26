#pragma once

// スクリーンサイズ
const int SCREEN_X = 1080;
const int SCREEN_Y = 800;
// イメージサイズ
const int IMG_SIZE_X = 128;
const int IMG_SIZE_Y = 128;
const int IMG_SIZE_HX = IMG_SIZE_X / 2;
const int IMG_SIZE_HY = IMG_SIZE_Y / 2;
// 位置調整
const int PLUS_X = 150;
const int TITLE_Y = -20;
// シェーダーパス
const std::string PATH_SHADER = "x64/Debug/";
// デルタタイム
std::chrono::system_clock::time_point mPreTime;
float mDeltaTime;
float mTotalTime;
// 描画位置
int mPosX;
int mPosY;
// 頂点情報
VERTEX2DSHADER mVertex[4];
WORD mIndex[6];
// スクリーンハンドル
int mainScreen;
// 画像ハンドル
int texDragon;
int texDragonNor;
int texFire;
int texNoize;
int texWhite;

// シェーダーハンドル

void Init(void);
void Release(void);
void Run(void);
// 描画用の四角頂点を作成
void MakeSquereVertex(void);
// 改行
void NewLine(void);
// タイトル描画
void DrawTitle(std::string title);
// 枠描画
void DrawFrame(void);
// シェーダー描画関数

