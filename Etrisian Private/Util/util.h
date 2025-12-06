#pragma once
#include <numbers>
#include <cmath>
#include "../Render/ImGui/imgui.h"

extern int screen_width;
extern int screen_height;

struct Vector2
{
	float x = {}, y = {};

	constexpr bool is_zero() const noexcept {
		return x == 0.f && y == 0.f;
	}
	constexpr const Vector2& operator-(const Vector2& other) const noexcept
	{
		return Vector2{ x - other.x, y - other.y };
	}

	ImVec2 toImVec2() const {
		return ImVec2(x, y);
	}

};

struct Vector4
{
	float x = {}, y = {}; float z = {}, w = {};
};


struct Vector3
{
	constexpr Vector3(
		const float x = 0.f,
		const float y = 0.f,
		const float z = 0.f) noexcept :
		x(x), y(y), z(z) {
	}

	constexpr const Vector3& operator-(const Vector3& other) const noexcept
	{
		return Vector3{ x - other.x, y - other.y, z - other.z };
	}

	bool IsValid() const {
		return !std::isnan(x) && !std::isnan(y) && !std::isnan(z) &&
			!std::isinf(x) && !std::isinf(y) && !std::isinf(z);
	}

	constexpr const Vector3& operator+(const Vector3& other) const noexcept
	{
		return Vector3{ x + other.x, y + other.y, z + other.z };
	}

	constexpr const Vector3& operator/(const float factor) const noexcept
	{
		return Vector3{ x / factor, y / factor, z / factor };
	}
	constexpr const Vector3& operator*(const float factor) const noexcept
	{
		return Vector3{ x * factor, y * factor, z * factor };
	}

	bool operator==(const Vector3& other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator!=(const Vector3& other) const {
		return x != other.x || y != other.y || z != other.z;
	}

	Vector3& operator+=(const Vector3& other) noexcept
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	inline float Dot(Vector3 v)
	{
		return (x * v.x + y * v.y + z * v.z);
	}

	float length(void) {
		auto sqr = [](float n) {
			return static_cast<float>(n * n);
			};

		return sqrt(sqr(x) + sqr(y) + sqr(z));
	}

	float distance_to(const Vector3& other) {
		Vector3 delta;
		delta.x = x - other.x;
		delta.y = y - other.y;
		delta.z = z - other.z;

		return delta.length();
	}

	Vector3 normalize() const {
		float length = std::sqrt(x * x + y * y + z * z);
		return { x / length, y / length, z / length };
	}

	float dot(const Vector3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	float x, y, z;

	constexpr bool is_zero() const noexcept {
		return x == 0.f && y == 0.f && z == 0.f;
	}

};

struct Matrix4x4 {
	float m[4][4];
};

Vector3 transform_vector(const Vector3& vec, const Matrix4x4& matrix) {
	Vector3 result;

	result.x = vec.x * matrix.m[0][0] + vec.y * matrix.m[1][0] + vec.z * matrix.m[2][0] + matrix.m[3][0];
	result.y = vec.x * matrix.m[0][1] + vec.y * matrix.m[1][1] + vec.z * matrix.m[2][1] + matrix.m[3][1];
	result.z = vec.x * matrix.m[0][2] + vec.y * matrix.m[1][2] + vec.z * matrix.m[2][2] + matrix.m[3][2];

	return result;
}

float gtc_distance(const Vector2& v1, const Vector2& v2) {
	float dx = v2.x - v1.x;
	float dy = v2.y - v1.y;
	return std::sqrt(dx * dx + dy * dy);
}