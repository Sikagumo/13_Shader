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
    
    return (srcCol * g_color);
}