#pragma once
#include "CollisionPrimitive.h"

/// <summary>
/// �����蔻��w���p�[�N���X
/// </summary>
class Collision
{
public:
	/// <summary>
	/// �����m�̓����蔻��
	/// </summary>
	/// <param name="sphere"></param>
	/// <param name="sphere"></param>
	/// <returns></returns>
	static bool CheckSphere2Sphere(const Sphere& sphere, const Sphere& sphere2);
	/// <summary>
	/// ���ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="sphere"></param>
	/// <param name="plane"></param>
	/// <param name="inter"></param>
	/// <returns></returns>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane,
		DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// �_�ƎO�p�`�̍ŋߐړ_�����߂�
	/// </summary>
	/// <param name="point"></param>
	/// <param name="triangle"></param>
	/// <param name="closest"></param>
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point,
		const Triangle& triangle, DirectX::XMVECTOR* closest);

	/// <summary>
	/// ���Ɩ@���t���O�p�`�̓����蔻��`�F�b�N
	/// </summary>
	/// <param name="sphere"></param>
	/// <param name="triangle"></param>
	/// <param name="inter"></param>
	/// <returns></returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle,
		DirectX::XMVECTOR* inter = nullptr);
};

