#include <chrono>
#include <math.h>
#include <string>
#include <DxLib.h>
#include "main.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// ウインドウモードで起動
	ChangeWindowMode(TRUE);
	SetGraphMode(SCREEN_X, SCREEN_Y, 32, 60);

	// ＤＸライブラリの初期化
	if (DxLib_Init() < 0)
	{
		// エラーが発生したら直ちに終了
		return -1;
	}

	// 初期化
	Init();

	// 実行
	Run();

	// 解放
	Release();

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}

void Init(void)
{

	// 描画に使用する画像の読み込み
	texDragon = LoadGraph("Data/Dragon.png");

	// 描画に使用する画像の読み込み
	texDragonNor = LoadGraph("Data/Dragon_n.png");

	// 描画に使用する画像の読み込み
	texFire = LoadGraph("Data/Fire.png");

	// 描画に使用する画像の読み込み
	texNoize = LoadGraph("Data/Noise128.png");

	// 描画に使用する画像の読み込み
	texWhite = LoadGraph("Data/White.png");


	// ピクセルシェーダのロード
	psSimpleColor = LoadPixelShader((PATH_SHADER + "SimpleColor.cso").c_str());

	// ピクセルシェーダのロード/定数バッファの作成
	psCustomColor = LoadPixelShader((PATH_SHADER + "CustomColor.cso").c_str());
	psCustomColorConstBuf = CreateShaderConstantBuffer(sizeof(float) * 4);

	// ピクセルシェーダのロード/定数バッファの作成
	psCircleColor = LoadPixelShader((PATH_SHADER + "CircleColor.cso").c_str());
	psCircleColorConstBuf = CreateShaderConstantBuffer(sizeof(float) * 4);

	// テクスチャシェーダのロード/定数バッファの作成
	psTexture = LoadPixelShader((PATH_SHADER + "Texture.cso").c_str());
	psTextureConstBuf = CreateShaderConstantBuffer(sizeof(float) * 4);

	// グレースケールシェーダのロード/定数バッファの作成
	psTextureGray = LoadPixelShader((PATH_SHADER + "TextureGray.cso").c_str());
	psTextureGrayConstBuf = CreateShaderConstantBuffer(sizeof(float) * 4);

	// セピアシェーダのロード/定数バッファの作成
	psTextureSepia = LoadPixelShader((PATH_SHADER + "TextureSepia.cso").c_str());
	psTextureSepiaConstBuf = CreateShaderConstantBuffer((sizeof(float) * 4) * 2);

	// ノイズシェーダのロード/定数バッファの作成
	psTextureNoize = LoadPixelShader((PATH_SHADER + "TextureNoize.cso").c_str());
	psTextureNoizeConstBuf = CreateShaderConstantBuffer((sizeof(float) * 4) * 2);

	// モザイクシェーダのロード/定数バッファの作成
	psTextureMosaic = LoadPixelShader((PATH_SHADER + "TextureMosaic.cso").c_str());
	psTextureMosaicConstBuf = CreateShaderConstantBuffer((sizeof(float) * 4) * 2);

	// スクロールシェーダのロード/定数バッファの作成
	psUvScroll = LoadPixelShader((PATH_SHADER + "UVScroll.cso").c_str());
	psUvScrollConstBuf = CreateShaderConstantBuffer((sizeof(float) * 4) * 2);

	// 歪みシェーダのロード/定数バッファの作成
	psUvDist = LoadPixelShader((PATH_SHADER + "UVDistotion.cso").c_str());
	psUvDistConstBuf = CreateShaderConstantBuffer((sizeof(float) * 4) * 2);
}

void Release(void)
{
	// ピクセルシェーダーを解放
	DeleteShader(psSimpleColor);
	DeleteShader(psCustomColor);
	DeleteShader(psCircleColor);
	DeleteShader(psTexture);
	DeleteShader(psTextureGray);
	DeleteShader(psTextureSepia);
	DeleteShader(psTextureNoize);
	DeleteShader(psTextureMosaic);
	DeleteShader(psUvScroll);
	DeleteShader(psUvDist);

	// ピクセルシェーダー用定数バッファを解放
	DeleteShaderConstantBuffer(psCustomColorConstBuf);
	DeleteShaderConstantBuffer(psTextureConstBuf);
	DeleteShaderConstantBuffer(psTextureGrayConstBuf);
	DeleteShaderConstantBuffer(psTextureSepiaConstBuf);
	DeleteShaderConstantBuffer(psTextureNoizeConstBuf);
	DeleteShaderConstantBuffer(psTextureMosaicConstBuf);
	DeleteShaderConstantBuffer(psUvScrollConstBuf);
	DeleteShaderConstantBuffer(psUvDistConstBuf);
}

void Run(void)
{

	// メインスクリーン作成
	mainScreen = MakeScreen(SCREEN_X, SCREEN_Y, true);

	// デルタタイム
	mPreTime = std::chrono::system_clock::now();
	mTotalTime = 0.0f;

	// ESCキーが押されるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		// デルタタイム
		auto nowTime = std::chrono::system_clock::now();
		mDeltaTime = static_cast<float>(
			std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - mPreTime).count() / 1000000000.0);
		mPreTime = nowTime;

		// ゲーム起動時間
		mTotalTime += mDeltaTime;

		// メインへ切替
		SetDrawScreen(mainScreen);

		// 画面を初期化
		ClearDrawScreen();

		// 背景色の描画
		DrawBox(0, 0, SCREEN_X, SCREEN_Y, 0x00aaaa, true);
		
		Draw();


		// 裏画面に描画
		SetDrawScreen(DX_SCREEN_BACK);
		// 画面を初期化
		ClearDrawScreen();
		// 描画
		DrawGraph(0, 0, mainScreen, true);

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();

	}

	// メインスクリーン削除
	DeleteGraph(mainScreen);

}
void Draw(void)
{
	// 座標の初期化
	const int START_POS_X = 10;
	const int START_POS_Y = 80;
	mPosX = START_POS_X;
	mPosY = START_POS_Y;

	// 通常の描画
	DrawTitle("DrawGraph");
	DrawGraph(mPosX, mPosY, texDragon, true);
	mPosX += PLUS_X;

	// 単色の描画
	DrawSimpleColor();
	mPosX += PLUS_X;

	// 指定色の描画
	DrawCustomColor();
	mPosX += PLUS_X;

	// 円の描画
	DrawCircleColor();
	mPosX += PLUS_X;

	// テクスチャの描画
	DrawTexture();
	mPosX += PLUS_X;

	// グレースケールの描画
	DrawTextureGray();
	mPosX += PLUS_X;

	// セピアの描画
	DrawTextureSepia();

	mPosX = START_POS_X;
	mPosY += PLUS_Y;

	// ノイズの描画
	DrawTextureNoize();
	mPosX += PLUS_X;

	// モザイクの描画
	DrawTextureMozaic();
	mPosX += PLUS_X;

	// スクロールの描画
	DrawUVScroll();
	mPosX += PLUS_X;

	// 歪みの描画
	DrawUVDist();
	mPosX += PLUS_X;
}


void MakeSquereVertex(void)
{

	// 毎回頂点データを作成するのは無駄ですが、
	// シェーダー追加時の作業を減らすため、毎フレーム作成

	int cnt = 0;
	float sX = static_cast<float>(mPosX);
	float sY = static_cast<float>(mPosY);
	float eX = static_cast<float>(mPosX + IMG_SIZE_X - 1);
	float eY = static_cast<float>(mPosY + IMG_SIZE_Y - 1);

	// ４頂点の初期化
	for (int i = 0; i < 4; i++)
	{
		mVertex[i].rhw = 1.0f;
		mVertex[i].dif = GetColorU8(255, 255, 255, 255);
		mVertex[i].spc = GetColorU8(255, 255, 255, 255);
		mVertex[i].su = 0.0f;
		mVertex[i].sv = 0.0f;
	}

	// 左上
	mVertex[cnt].pos = VGet(sX, sY, 0.0f);
	mVertex[cnt].u = 0.0f;
	mVertex[cnt].v = 0.0f;
	cnt++;

	// 右上
	mVertex[cnt].pos = VGet(eX, sY, 0.0f);
	mVertex[cnt].u = 1.0f;
	mVertex[cnt].v = 0.0f;
	cnt++;

	// 右下
	mVertex[cnt].pos = VGet(eX, eY, 0.0f);
	mVertex[cnt].u = 1.0f;
	mVertex[cnt].v = 1.0f;
	cnt++;

	// 左下
	mVertex[cnt].pos = VGet(sX, eY, 0.0f);
	mVertex[cnt].u = 0.0f;
	mVertex[cnt].v = 1.0f;

	/*
	　～～～～～～
		0-----1
		|     |
		|     |
		3-----2
	　～～～～～～
		0-----1
		|  ／
		|／
		3
	　～～～～～～
			  1
		   ／ |
		 ／   |
		3-----2
	　～～～～～～
	*/


	// 頂点インデックス
	cnt = 0;
	mIndex[cnt++] = 0;
	mIndex[cnt++] = 1;
	mIndex[cnt++] = 3;

	mIndex[cnt++] = 1;
	mIndex[cnt++] = 2;
	mIndex[cnt++] = 3;

}

/// <summary>
/// 改行
/// </summary>
/// <param name=""></param>
void NewLine(void)
{
	mPosX = 10;
	mPosY += 180;
}

void DrawTitle(std::string title)
{
	DrawString(mPosX, mPosY + TITLE_Y, title.c_str(), 0x000000);
}

void DrawFrame(void)
{
	DrawBox(mPosX - 1, mPosY - 1, mPosX + IMG_SIZE_X, mPosY + IMG_SIZE_Y, 0x000000, false);
}


void DrawSimpleColor(void)
{
	DrawTitle("単色");

	// シェーダーの設定
	SetUsePixelShader(psSimpleColor);

	// ポリゴンの生成
	MakeSquereVertex();

	// 描画
	DrawPolygonIndexed2DToShader(
		mVertex, 4, mIndex, 2);

	// 枠描画
	DrawFrame();
}
void DrawCustomColor(void)
{
	DrawTitle("指定色");

	// シェーダーの設定
	SetUsePixelShader(psCustomColor);


	// 定数バッファのポインタアドレス
	COLOR_F* cbBuf = (COLOR_F*)GetBufferShaderConstantBuffer(psCustomColorConstBuf);
	cbBuf->r = 0.5f;
	cbBuf->g = 0.5f;
	cbBuf->b = 0.5f;
	cbBuf->a = 1.0f;
	
	
	/*
	float* cbBuf = (float*)GetBufferShaderConstantBuffer(psCustomColorConstBuf);

	// R
	*cbBuf++ = 0.5f;

	// G
	*cbBuf++ = 0.5f;

	// B
	*cbBuf++ = 0.5f;

	// Alpha
	*cbBuf++ = 1.0f;*/

	// ピクセルシェーダ用の定数バッファを更新し、書き込んだ値を反映
	UpdateShaderConstantBuffer(psCustomColorConstBuf);

	// ピクセルシェーダ用の定数バッファを定数バッファレジスタに送る
	SetShaderConstantBuffer(psCustomColorConstBuf, DX_SHADERTYPE_PIXEL, 0);

	// ポリゴンの生成
	MakeSquereVertex();

	// 描画
	DrawPolygonIndexed2DToShader(
		mVertex, 4, mIndex, 2);

	// 枠描画
	DrawFrame();
}
void DrawCircleColor(void)
{
	DrawTitle("円形");

	// シェーダーの設定
	SetUsePixelShader(psCircleColor);


	// 定数バッファのポインタアドレス
	COLOR_F* cbBuf = (COLOR_F*)GetBufferShaderConstantBuffer(psCircleColorConstBuf);
	cbBuf->r = 0.0f;
	cbBuf->g = 0.25f;
	cbBuf->b = 1.0f;
	cbBuf->a = 1.0f;

	// ピクセルシェーダ用の定数バッファを更新し、書き込んだ値を反映
	UpdateShaderConstantBuffer(psCircleColorConstBuf);

	// ピクセルシェーダ用の定数バッファを定数バッファレジスタに送る
	SetShaderConstantBuffer(psCircleColorConstBuf, DX_SHADERTYPE_PIXEL, 0);

	// ポリゴンの生成
	MakeSquereVertex();

	// 描画
	DrawPolygonIndexed2DToShader(
		mVertex, 4, mIndex, 2);

	// 枠描画
	DrawFrame();
}
void DrawTexture(void)
{
	DrawTitle("テクスチャ");

	// シェーダーの設定
	SetUsePixelShader(psTexture);

	// シェーダーにテクスチャを転送
	SetUseTextureToShader(0, texDragon);

	// 定数バッファのポインタアドレス
	COLOR_F* cbBuf = (COLOR_F*)GetBufferShaderConstantBuffer(psTextureConstBuf);
	cbBuf->r = 1.0f;
	cbBuf->g = 1.0f;
	cbBuf->b = 1.0f;
	cbBuf->a = abs(sin(mTotalTime));

	// ピクセルシェーダ用の定数バッファを更新し、書き込んだ値を反映
	UpdateShaderConstantBuffer(psTextureConstBuf);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	// ピクセルシェーダ用の定数バッファを定数バッファレジスタに送る
	SetShaderConstantBuffer(psTextureConstBuf, DX_SHADERTYPE_PIXEL, 0);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	// ポリゴンの生成
	MakeSquereVertex();

	// 描画
	DrawPolygonIndexed2DToShader(
		mVertex, 4, mIndex, 2);

	// 枠描画
	DrawFrame();
}
void DrawTextureGray(void)
{
	DrawTitle("グレースケール");

	// シェーダーの設定
	SetUsePixelShader(psTextureGray);

	// シェーダーにテクスチャを転送
	SetUseTextureToShader(0, texDragon);

	// 定数バッファのポインタアドレス
	COLOR_F* cbBuf = (COLOR_F*)GetBufferShaderConstantBuffer(psTextureGrayConstBuf);

	// ピクセルシェーダ用の定数バッファを更新し、書き込んだ値を反映
	UpdateShaderConstantBuffer(psTextureGrayConstBuf);

	// ピクセルシェーダ用の定数バッファを定数バッファレジスタに送る
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	SetShaderConstantBuffer(psTextureGrayConstBuf, DX_SHADERTYPE_PIXEL, 0);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ポリゴンの生成
	MakeSquereVertex();

	// 描画
	DrawPolygonIndexed2DToShader(
		mVertex, 4, mIndex, 2);

	// 枠描画
	DrawFrame();
}
void DrawTextureSepia(void)
{
	DrawTitle("セピア");

	// シェーダーの設定
	SetUsePixelShader(psTextureSepia);

	// シェーダーにテクスチャを転送
	SetUseTextureToShader(0, texDragon);

	// 定数バッファのポインタアドレス
	COLOR_F* cbBuf = (COLOR_F*)GetBufferShaderConstantBuffer(psTextureSepiaConstBuf);
	cbBuf->r = 1.0f;
	cbBuf->g = 1.0f;
	cbBuf->b = 1.0f;
	cbBuf->a = 1.0f;
	cbBuf++;
	cbBuf->r = 1.0f;


	// ピクセルシェーダ用の定数バッファを更新し、書き込んだ値を反映
	UpdateShaderConstantBuffer(psTextureSepiaConstBuf);

	SetUseTextureToShader(0, texDragon);

	// ピクセルシェーダ用の定数バッファを定数バッファレジスタに送る
	SetShaderConstantBuffer(psTextureSepiaConstBuf, DX_SHADERTYPE_PIXEL, 0);

	// ポリゴンの生成
	MakeSquereVertex();

	// 描画
	DrawPolygonIndexed2DToShader(
		mVertex, 4, mIndex, 2);

	// 枠描画
	DrawFrame();
}
void DrawTextureNoize(void)
{
	DrawTitle("ノイズ");

	// シェーダーの設定
	SetUsePixelShader(psTextureNoize);

	// シェーダーにテクスチャを転送
	SetUseTextureToShader(0, texDragon);

	// 定数バッファのポインタアドレス
	COLOR_F* cbBuf = (COLOR_F*)GetBufferShaderConstantBuffer(psTextureNoizeConstBuf);
	cbBuf->r = 1.0f;
	cbBuf->g = 1.0f;
	cbBuf->b = 1.0f;
	cbBuf->a = 0.5f;
	cbBuf++;
	cbBuf->r = mTotalTime;

	// ピクセルシェーダ用の定数バッファを更新し、書き込んだ値を反映
	UpdateShaderConstantBuffer(psTextureNoizeConstBuf);

	// ピクセルシェーダ用の定数バッファを定数バッファレジスタに送る
	SetShaderConstantBuffer(psTextureNoizeConstBuf, DX_SHADERTYPE_PIXEL, 0);

	// ポリゴンの生成
	MakeSquereVertex();

	// 描画
	DrawPolygonIndexed2DToShader(
		mVertex, 4, mIndex, 2);

	// 枠描画
	DrawFrame();
}
void DrawTextureMozaic(void)
{
	DrawTitle("モザイク");

	// シェーダーの設定
	SetUsePixelShader(psTextureMosaic);

	// シェーダーにテクスチャを転送
	SetUseTextureToShader(0, texDragon);

	// 定数バッファのポインタアドレス
	COLOR_F* cbBuf = (COLOR_F*)GetBufferShaderConstantBuffer(psTextureMosaicConstBuf);
	cbBuf->r = 1.0f;
	cbBuf->g = 1.0f;
	cbBuf->b = 1.0f;
	cbBuf->a = 1.0f;
	cbBuf++;
	cbBuf->r = 128;
	cbBuf->g = 128;
	cbBuf->b = abs(sin(mTotalTime));

	// ピクセルシェーダ用の定数バッファを更新し、書き込んだ値を反映
	UpdateShaderConstantBuffer(psTextureMosaicConstBuf);
	
	// ピクセルシェーダ用の定数バッファを定数バッファレジスタに送る
	SetShaderConstantBuffer(psTextureMosaicConstBuf, DX_SHADERTYPE_PIXEL, 0);

	// ポリゴンの生成
	MakeSquereVertex();

	// 描画
	DrawPolygonIndexed2DToShader(
		mVertex, 4, mIndex, 2);

	// 枠描画
	DrawFrame();
}
void DrawTextureCrayon(void)
{
}
void DrawUVScroll(void)
{
	DrawTitle("UVスクロール");

	// シェーダーの設定
	SetUsePixelShader(psUvScroll);

	// シェーダーにテクスチャを転送
	SetUseTextureToShader(0, texDragon);

	SetTextureAddressModeUV(DX_TEXADDRESS_WRAP, DX_TEXADDRESS_WRAP);

	// 定数バッファのポインタアドレス
	COLOR_F* cbBuf = (COLOR_F*)GetBufferShaderConstantBuffer(psUvScrollConstBuf);
	cbBuf->r = 1.0f;
	cbBuf->g = 1.0f;
	cbBuf->b = 1.0f;
	cbBuf->a = 1.0f;
	cbBuf++;
	cbBuf->r = mTotalTime;
	const float speed = 2.5f;
	cbBuf->g = speed;


	// ピクセルシェーダ用の定数バッファを更新し、書き込んだ値を反映
	UpdateShaderConstantBuffer(psUvScrollConstBuf);

	// ピクセルシェーダ用の定数バッファを定数バッファレジスタに送る
	SetShaderConstantBuffer(psUvScrollConstBuf, DX_SHADERTYPE_PIXEL, 0);

	// ポリゴンの生成
	MakeSquereVertex();

	// 描画
	DrawPolygonIndexed2DToShader(
		mVertex, 4, mIndex, 2);

	// 枠描画
	DrawFrame();
}
void DrawUVDist(void)
{
	DrawTitle("歪み");

	// シェーダーの設定
	SetUsePixelShader(psUvDist);

	// シェーダーにテクスチャを転送
	SetUseTextureToShader(0, texFire);
	SetUseTextureToShader(1, texNoize);

	SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);

	// 定数バッファのポインタアドレス
	COLOR_F* cbBuf = (COLOR_F*)GetBufferShaderConstantBuffer(psUvDistConstBuf);
	cbBuf->r = 1.0f;
	cbBuf->g = 1.0f;
	cbBuf->b = 1.0f;
	cbBuf->a = 1.0f;
	cbBuf++;
	cbBuf->r = mTotalTime;


	// ピクセルシェーダ用の定数バッファを更新し、書き込んだ値を反映
	UpdateShaderConstantBuffer(psUvDistConstBuf);

	// ピクセルシェーダ用の定数バッファを定数バッファレジスタに送る
	SetShaderConstantBuffer(psUvDistConstBuf, DX_SHADERTYPE_PIXEL, 0);

	// ポリゴンの生成
	MakeSquereVertex();

	// 描画
	DrawPolygonIndexed2DToShader(
		mVertex, 4, mIndex, 2);

	// 枠描画
	DrawFrame();
}
