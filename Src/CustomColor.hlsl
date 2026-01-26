// 定数バッファ：スロット０番目(b0と書く)
cbuffer cbParam : register(b0)
{
    float4 g_color;
}

float4 main() : SV_TARGET
{
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
    return g_color;
}