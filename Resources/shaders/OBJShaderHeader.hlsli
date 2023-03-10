
//マテリアル
cbuffer ConstBufferDataMaterial : register(b0)
{
	float4 color;//色(RGBA)
}

//3D変換行列
cbuffer ConstBufferDataTransform : register(b1)
{
	matrix viewproj; //ビュープロジェクション行列
	matrix world;    //ワールド行列
	float3 cameraPos;//カメラ座標（ワールド座標）
}

cbuffer ConstBufferDataMaterial2 : register(b2)
{
	float3 m_ambient  : packoffset(c0);//アンビエント
	float3 m_diffuse  : packoffset(c1);//ディフューズ
	float3 m_specular : packoffset(c2);//スペキュラー
	float  m_alpha    : packoffset(c2.w);//アルファ
}

// 点光源の数
static const int POINTLIGHT_NUM = 6;
struct PointLight
{
	float3 lightpos;//ライトの座標
	float3 lightcolor;//ライトの色
	float3 lightatten;//ライトの距離減衰係数
	uint   active;//有効か
};

// 平行光源の数
static const int DIRLIGHT_NUM = 3;
struct DirLight
{
	float3 lightv;    // ライトへの方向の単位ベクトル
	float3 lightcolor;    // ライトの色(RGB)
	uint   active;      //有効か
};

static const int SPOTLIGHT_NUM = 3;
struct SpotLight
{
	float3 lightv;    // ライトへの方向の単位ベクトル
	float3 lightpos;    // ライトの位置
	float3 lightcolor;    // ライトの色(RGB)
	float3 lightatten;    // ライトの減衰係数
	float2 lightfactoranglecos;//ライト減衰角度のコサイン
	uint   active;      //有効か
};

//丸影
static const int CIRCLESHADOW_NUM = 1;
struct CircleShadow
{
	float3 dir; //投影方向の逆ベクトル
	float3 casterPos; //キャスター座標
	float distanceCasterLight; //キャスターとライトの距離
	float3 atten; //距離減衰係数
	float2 factorAngleCos; //減衰角度のコサイン
	uint active;
};

//LightManagerのConstBufferと同じ型を宣言
cbuffer ConstBufferDataMaterial3 : register(b3)
{
	float3     ambientColor;
	DirLight   dirLights[DIRLIGHT_NUM];
	PointLight pointLights[POINTLIGHT_NUM];
	SpotLight  spotLights[SPOTLIGHT_NUM];
	CircleShadow circleShadows[CIRCLESHADOW_NUM];
}

cbuffer ConstBufferEffectFlags : register(b4)
{
	//フォグ
	uint isFog;
}

//頂点シェーダからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos    : SV_POSITION;//システム用頂点座標
	float4 worldpos : POSITION;//ワールド座標
	float3 normal   : NORMAL;//ワールド座標
	float2 uv       : TEXCOORD;//uv座標
};

