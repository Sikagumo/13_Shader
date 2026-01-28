// ピクセルシェーダーの入力
struct PS_INPUT
{
    float4 Position   : SV_POSITION; // 座標( プロジェクション空間 )
    float4 Diffuse    : COLOR0;      // ディフューズカラー
    float2 TexCoords0 : TEXCOORD0;   // テクスチャ座標
};

// 定数バッファ：スロット０番目(b0と書く)
cbuffer cbParam : register(b0)
{
    float4 g_color;
}


float4 main(PS_INPUT psInput) : SV_TARGET
{
    float2 uv = psInput.TexCoords0;
    float x = abs(uv.x - 0.5f);
    float y = abs(uv.y - 0.5f);
    
    float color = 0;
    
    // 右側全白
    color = x;
    
    // 上側全白
    //color = (1.0f - uv.y);
    
    //斜め
    //color = (uv.x * uv.y);
    
    // 菱形
    //color = (x + y) * 1.5f;
    
    // 円型
    //color = (0.5f > (sqrt(x * x + y * y)));
    
    // グラデーション円
    color = (x * x + y * y) * 4.0f;
    //color = sqrt(x * x + y * y) * 2.0f;
    
    float4 view = float4(color, color, color, 1.0f);
    view += g_color;
    return view;
}