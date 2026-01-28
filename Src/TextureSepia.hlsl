// ピクセルシェーダーの入力
struct PS_INPUT
{
    float4 Position : SV_POSITION; // 座標( プロジェクション空間 )
    float4 Diffuse : COLOR0; // ディフューズカラー
    float2 TexCoords0 : TEXCOORD0; // テクスチャ座標
};

// 定数バッファ：スロット0番目
cbuffer cbParam : register(b0)
{
    float4 g_color;
    float g_sepia_pow;
    float empty1;
    float empty2;
    float empty3;
}

// 描画するテクスチャ
Texture2D g_SrcTexture : register(t0);

// サンプラー：適切な色を決める処理
SamplerState g_SrcSampler : register(s0);


float4 main(PS_INPUT psInput) : SV_TARGET
{
    // UV座標とテクスチャを参照して、最適な色を取得する
    float4 srcCol = g_SrcTexture.Sample(g_SrcSampler, psInput.TexCoords0);
    
    if (srcCol.a < 0.01f)
    {
        // 描画しない
        discard;
    }
    
    // 一度グレースケールを剪む
    float gray = dot(srcCol.rgb, float3(0.299, 0.587, 0.114));
    srcCol.rgb = gray;
    
    
    // ①シンプルセピア
    const float3 COLOR_SEPIA = float3(1.07f, 0.74f, 0.43f);
    //srcCol.rgb *= COLOR_SEPIA;
    
    // ②調整付きセピア
    float3 sepia = srcCol.rgb;
    sepia.r = dot(srcCol.rgb, float3(0.393f, 0.769f, 0.189f));
    sepia.g = dot(srcCol.rgb, float3(0.349f, 0.686f, 0.168f));
    sepia.b = dot(srcCol.rgb, float3(0.272f, 0.534f, 0.131f));
    
    srcCol.rgb = lerp(srcCol.rbg, sepia, g_sepia_pow);

    
    return srcCol * g_color;
}