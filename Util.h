#pragma once

#include "Vec3.h"
#include "Vec2.h"
#include "Vec4.h"
#include "M4.h"
#include"TextureManager.h"

//����
void Vec4xM4(Vec4& v, const M4& m4);
void Vec3xM4(Vec3& v, const M4& m4, const bool w);
Vec3 GetVec3xM4(Vec3 v, const M4 m4, const bool w);
void Vec3xM4andDivisionW(Vec3& v, const M4& m4, const bool w);

//���W�A���n
const float pi = 3.141592f;
float AngletoRadi(const float& angle);
float RaditoAngle(const float& radian);
float GetRadianVec3(const Vec3& v1, const Vec3& v2);

//���`�⊮
Vec3 LerpVec3(const Vec3& v1, const Vec3& v2, float t);
Vec3 SlerpVec3(const Vec3& v1, const Vec3& v2, float t);

float EaseIn(float t);

float EaseOut(float t);

//�Փ�
bool CollisionCircleCircle(const Vec3& pos1, const float& r1,
	const Vec3& pos2, const float& r2);

bool CollisionRayCircle(const Vec3& sv, const Vec3& ev, const float r, const Vec3& pos, const float r2);

bool CollisionBox(const int x1, const int y1, const int x2, const int y2, const int r1, const int r2);


//�֗�
/// <summary>
/// 1���]1�ŕԂ��i0��0�j
/// </summary>
/// <returns></returns>
float sign(float num);

//�ϊ�
Vec2 Vec3toVec2(const Vec3& v, const XMMATRIX& view, const XMMATRIX& projection);
/// <summary>
/// distance�́Aprojection��nearPos����̈ʒu!!
/// </summary>
/// <param name="v"></param>
/// <param name="view"></param>
/// <param name="projection"></param>
/// <param name="distance"></param>
/// <returns></returns>
Vec3 Vec2toVec3(const Vec2& v, const XMMATRIX& view, const XMMATRIX& projection, const float distance);

/// <summary>
/// �X�N���[�����W��projection��near,far�̈ʒu�ɕϊ�����return...Pos�ɑ��
/// </summary>
/// <param name="v"></param>
/// <param name="returnNearPos"></param>
/// <param name="returnFarPos"></param>
/// <param name="view"></param>
/// <param name="projection"></param>
void Vec2toNearFarPos(const Vec2& pos, Vec3& returnNearPos, Vec3& returnFarPos, const XMMATRIX& view, const XMMATRIX& projection);


