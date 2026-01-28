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

float rand(float2 co)
{
    // -0.50～0.49
    float a = frac(dot(co, float2(2.067390879775102f, 12.451168662908249f))) - 0.5f;
    float s = a * (6.182785114200511f + a * a * (-38.026512460676566f + a * a * 53.392573080032137f));
    
    // -0.99～0.99
    float t = frac(s * 43758.5453f);
    return t;
}

float4 main(PS_INPUT psInput) : SV_TARGET
{
    // UV座標とテクスチャを参照して、最適な色を取得する
    float4 srcCol = g_SrcTexture.Sample(g_SrcSampler, psInput.TexCoords0);
    
    if (srcCol.a < 0.01f)
    {
        // 描画しない
        discard;
    }
    srcCol.r = rand(srcCol.r);
    srcCol.g = rand(srcCol.g);
    srcCol.b = rand(srcCol.b);
    
    return (srcCol * g_color);
}