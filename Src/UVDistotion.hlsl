// ピクセルシェーダーの入力
struct PS_INPUT
{
    float4 Position : SV_POSITION; // 座標( プロジェクション空間 )
    float4 Diffuse : COLOR0; // ディフューズカラー
    float2 TexCoords0 : TEXCOORD0; // テクスチャ座標
};

// 定数バッファ：スロット０番目
cbuffer cbParam : register(b0)
{
    float4 g_color;
    float g_time;
    float g_speedX;
    float g_speedY;
    float empty;
}

// 描画するテクスチャ
Texture2D g_SrcTexture : register(t0);

// サンプラー：適切な色を決める処理
SamplerState g_SrcSampler : register(s0);

// 描画するノイズ用テクスチャ
Texture2D g_NoiseTexture : register(t1);

// サンプラー：適切な色を決める処理
SamplerState g_NoiseSampler : register(s1);

float4 main(PS_INPUT psInput) : SV_TARGET
{
    // uvスクロール
    float2 uv = psInput.TexCoords0;
    
    float2 noiseUV = uv;

    noiseUV.x += sin(g_time * g_speedX);
    noiseUV.y += frac(g_time * g_speedY);
   
    uv.y += noiseUV * 0.2f;
    uv.x += noiseUV * 0.2f;
   
    //uv.y += (noiseUv * g_speed);

    // ノイズ画像(ランダム)
    float4 noise = g_NoiseTexture.Sample(g_NoiseSampler, uv) * 2.0f - 1.0f;
    
    // fireUV[0.0f～1.0f]
    float2 fireUV = uv;
    fireUV.y += noise.r * 0.2f;
    
    // 変更されたuv座標を元に色を取得する
    float4 srcCol = g_SrcTexture.Sample(g_SrcSampler, fireUV);
    
    if (srcCol.a < 0.25f)
    {
        // 描画しない
        discard;
    }
    
    return srcCol *= g_color;
}