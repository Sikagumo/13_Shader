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
    float g_time;
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
    float4 srcCol = g_SrcTexture.Sample(g_SrcSampler, psInput.TexCoords0);
    
    if (srcCol.a < 0.25f)
    {
        discard;
    }
    
    // ノイズ計算
    float noise = frac(sin(dot(psInput.TexCoords0 * g_time,
                               float2(12.9898f, 78.233f))) * 43758.5453f) - 0.5f;
    
    if (srcCol.a == 0.0f && noise > 0.0f)
    {
        // 完全透明でノイズ有りは、ノイズカラー使用
        srcCol.rgb = float3(noise, noise, noise);
        
        // 更に不透明にする
        srcCol.a = 1.0f;
    }
    else
    {
    // ノイズカラーを加算する
        srcCol.rgb += float3(noise, noise, noise);
    }
    
    return srcCol;
}