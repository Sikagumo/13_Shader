// ピクセルシェーダーの入力
struct PS_INPUT
{
    float4 Position : SV_POSITION; // 座標( プロジェクション空間 )
    float4 Diffuse : COLOR0; // ディフューズカラー
    float2 TexCoords0 : TEXCOORD0; // テクスチャ座標
};

// 定数バッファ：スロット１番目
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
    
    float gray;
    
    // ①３色の平均値を取る
    //gray = (srcCol.r + srcCol.g + srcCol.b) / 3.0f;
    
    // ② ITU-R Rec BT.601(内積なので、色乗算して足している)
    gray = dot(srcCol.rgb, float3(0.299, 0.587, 0.114));
    
    srcCol.rgb = gray;
    
    return srcCol;
}