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
    float g_sizeX; // 画像サイズX
    float g_sizeY; // 画像サイズY
    float g_scale; // モザイクスケール
    float empty;
}

// 描画するテクスチャ
Texture2D g_SrcTexture : register(t0);

// サンプラー：適切な色を決める処理
SamplerState g_SrcSampler : register(s0);


float4 main(PS_INPUT psInput) : SV_TARGET
{
    // uv座標を変える
    float2 uv = psInput.TexCoords0;
    float scaleX = g_sizeX / (g_scale * 10.0f);
    float scaleY = g_sizeY / (g_scale * 10.0f);
    uv.x = floor(uv.x * scaleX) / scaleX;
    uv.y = floor(uv.y * scaleY) / scaleY;
    
    // 変更されたuv座標を元に色を取得する
    float4 srcCol = g_SrcTexture.Sample(g_SrcSampler, uv);
    
    if (srcCol.a < 0.01f)
    {
        // 描画しない
        discard;
    }
    return srcCol *= g_color;
}