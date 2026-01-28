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
    float g_speed;
    float empty1;
    float empty2;
}

// 描画するテクスチャ
Texture2D g_SrcTexture : register(t0);

// サンプラー：適切な色を決める処理
SamplerState g_SrcSampler : register(s0);

/* DirectXパターン
SamplerState g_SrcSampler : register(s0)
{
    AddressU = WRAP;
    AddressV = WRAP;
};*/

float4 main(PS_INPUT psInput) : SV_TARGET
{
    // uvスクロール
    float2 uv = psInput.TexCoords0;
    
    // 0.0f~1.0 + 0.1 = 0.1~1.1 CLAMP
    // 0.0f~1.0 + 0.1 = 0.1~1.0,  WRAP
    // 0.0f~1.0 + 0.1 = 0.1~1.1
    
    uv.x += (g_time * g_speed);
    uv.y += (g_time * g_speed);
    
    // 変更されたuv座標を元に色を取得する
    float4 srcCol = g_SrcTexture.Sample(g_SrcSampler, uv);
    if (srcCol.a < 0.01f)
    {
        // 描画しない
        discard;
    }
    
    return srcCol *= g_color;
}