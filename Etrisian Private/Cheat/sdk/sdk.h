#pragma once
#include <format>
#include "../../render/ImGui/imgui.h"
#include <iostream>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace SDK {
    class Vec3 {
    public:
        constexpr Vec3() : X(0.f), Y(0.f), Z(0.f) {}
        constexpr Vec3(float x, float y, float z) : X(x), Y(y), Z(z) {}
        ~Vec3() {};

        float X;
        float Y;
        float Z;

        inline operator bool() const noexcept {
            return X != 0.f && Y != 0.f;
        }

        inline Vec3 operator+(const Vec3& v) const noexcept {
            return Vec3(X + v.X, Y + v.Y, Z + v.Z);
        }

        inline Vec3& operator+=(const Vec3& v) noexcept {
            X += v.X; Y += v.Y; Z += v.Z;
            return *this;
        }

        inline Vec3 operator-(const Vec3& v) const noexcept {
            return Vec3(X - v.X, Y - v.Y, Z - v.Z);
        }

        inline Vec3& operator-=(const Vec3& v) noexcept {
            X -= v.X; Y -= v.Y; Z -= v.Z;
            return *this;
        }

        inline Vec3 operator*(const Vec3& v) const noexcept {
            return Vec3(X * v.X, Y * v.Y, Z * v.Z);
        }

        inline Vec3& operator*=(const Vec3& v) noexcept {
            X *= v.X; Y *= v.Y; Z *= v.Z;
            return *this;
        }

        inline Vec3 operator/(const Vec3& v) const noexcept {
            return Vec3(X / v.X, Y / v.Y, Z / v.Z);
        }

        inline Vec3& operator/=(const Vec3& v) noexcept {
            X /= v.X; Y /= v.Y; Z /= v.Z;
            return *this;
        }

        inline Vec3 operator*(float s) const noexcept {
            return Vec3(X * s, Y * s, Z * s);
        }

        inline Vec3& operator*=(float s) noexcept {
            X *= s; Y *= s; Z *= s;
            return *this;
        }

        inline Vec3 operator/(float s) const noexcept {
            return Vec3(X / s, Y / s, Z / s);
        }

        inline Vec3& operator/=(float s) noexcept {
            X /= s; Y /= s; Z /= s;
            return *this;
        }

        inline float Dot(const Vec3 v) const noexcept {
            return (X * v.X) + (Y * v.Y) + (Z * v.Z);
        }

        inline Vec3 Cross(const Vec3& v) const noexcept {
            return Vec3(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X);
        }

        inline float Length() const noexcept {
            return std::sqrtf(X * X + Y * Y + Z * Z);
        }

        inline float LengthSqr() const noexcept {
            return X * X + Y * Y + Z * Z;
        }

        inline float Distance(const Vec3& v) const noexcept {
            return (*this - v).Length();
        }

        inline float DistanceTo(const Vec3& v) const noexcept {
            return Distance(v);
        }

        inline void Normalise() noexcept {
            const float len = Length();
            if (len > std::numeric_limits<float>::epsilon()) {
                *this /= len;
            }
        }

        inline Vec3 Normalised() const noexcept {
            Vec3 temp = *this;
            temp.Normalise();
            return temp;
        }

        inline void ClampAngles() noexcept {
            X = std::fmod(X + 90.f, 180.f) - 90.f;
            if (X > 89.f) {
                X = 89.f;
            }

            if (X < -89.f) {
                X = -89.f;
            }

            Y = std::fmod(Y + 180.f, 360.f) - 180.f;
            if (Y > 180.f) {
                Y = 180.f;
            }

            if (Y < -180.f) {
                Y = -180.f;
            }

            Z = 0.f;
        }

        inline bool IsZero() const noexcept {
            return X == 0.f && Y == 0.f && Z == 0.f;
        }

        inline bool Empty() const noexcept {
            return IsZero();
        }

        inline bool IsValid() const noexcept {
            return !std::isnan(X) && !std::isinf(X) &&
                !std::isnan(Y) && !std::isinf(Y) &&
                !std::isnan(Z) && !std::isinf(Z);
        }

        static Vec3 CalculateAngle(const Vec3& src, const Vec3& dst) {
            const auto& delta = src - dst;
            const float hyp = std::sqrtf(delta.X * delta.X + delta.Y * delta.Y);

            Vec3 angle(0.f, 0.f, 0.f);
            if (hyp > std::numeric_limits<float>::epsilon()) {
                angle.X = std::atan2f(-delta.Z, hyp) * (180.f / static_cast<float>(M_PI));
                angle.Y = std::atan2f(delta.Y, delta.X) * (180.f / static_cast<float>(M_PI));
            }

            return angle;
        }
    };

    inline Vec3 operator*(float s, const Vec3& v) noexcept {
        return v * s;
    }

    class Vec2 {
    public:
        float X, Y;

        constexpr Vec2() : X(0.f), Y(0.f) {}
        constexpr Vec2(float x, float y) : X(x), Y(y) {}
        inline Vec2(const ImVec2& v) : X(v.x), Y(v.y) {}
        ~Vec2() = default;

        inline operator ImVec2() const {
            return ImVec2(X, Y);
        }

        inline operator bool() const noexcept {
            return X != 0.f && Y != 0.f;
        }

        inline Vec2 operator+(const Vec2& v) const noexcept {
            return Vec2(X + v.X, Y + v.Y);
        }

        inline Vec2& operator+=(const Vec2& v) noexcept {
            X += v.X; Y += v.Y;
            return *this;
        }

        inline Vec2 operator-(const Vec2& v) const noexcept {
            return Vec2(X - v.X, Y - v.Y);
        }

        inline Vec2& operator-=(const Vec2& v) noexcept {
            X -= v.X; Y -= v.Y;
            return *this;
        }

        inline Vec2 operator*(const Vec2& v) const noexcept {
            return Vec2(X * v.X, Y * v.Y);
        }

        inline Vec2& operator*=(const Vec2& v) noexcept {
            X *= v.X; Y *= v.Y;
            return *this;
        }

        inline Vec2 operator/(const Vec2& v) const noexcept {
            return Vec2(X / v.X, Y / v.Y);
        }

        inline Vec2& operator/=(const Vec2& v) noexcept {
            X /= v.X; Y /= v.Y;
            return *this;
        }

        inline Vec2 operator+(float s) const noexcept {
            return Vec2(X + s, Y + s);
        }

        inline Vec2& operator+=(float s) noexcept {
            X += s; Y += s;
            return *this;
        }

        inline Vec2 operator-(float s) const noexcept {
            return Vec2(X - s, Y - s);
        }

        inline Vec2& operator-=(float s) noexcept {
            X -= s; Y -= s;
            return *this;
        }

        inline Vec2 operator*(float s) const noexcept {
            return Vec2(X * s, Y * s);
        }

        inline Vec2& operator*=(float s) noexcept {
            X *= s; Y *= s;
            return *this;
        }

        inline Vec2 operator/(float s) const noexcept {
            return Vec2(X / s, Y / s);
        }

        inline Vec2& operator/=(float s) noexcept {
            X /= s; Y /= s;
            return *this;
        }

        inline float Length() const noexcept {
            return std::sqrtf(X * X + Y * Y);
        }

        inline float LengthSqr() const noexcept {
            return X * X + Y * Y;
        }

        inline Vec2 Flip() const noexcept {
            return Vec2(Y, X);
        }

        inline bool IsZero() const noexcept {
            return X == 0.f && Y == 0.f;
        }

        inline bool Empty() const noexcept {
            return IsZero();
        }
    };

    inline Vec2 operator*(float s, const Vec2& v) noexcept {
        return v * s;
    }

    enum PlayerBoneIdx {
        BONE_IDX_HEAD,          // 0
        BONE_IDX_NECK,          // 1
        BONE_IDX_CHEST,         // 2
        BONE_IDX_MID,           // 3
        BONE_IDX_TUMMY,         // 4
        BONE_IDX_PELVIS,        // 5
        BONE_IDX_LEFT_FOOT_1,   // 6
        BONE_IDX_LEFT_FOOT_2,   // 7
        BONE_IDX_LEFT_FOOT_3,   // 8
        BONE_IDX_LEFT_FOOT_4,   // 9
        BONE_IDX_LEFT_HAND_1,   // 10
        BONE_IDX_LEFT_HAND_2,   // 11
        BONE_IDX_LEFT_HAND_3,   // 12
        BONE_IDX_LEFT_HAND_4,   // 13
        BONE_IDX_RIGHT_FOOT_1,  // 14
        BONE_IDX_RIGHT_FOOT_2,  // 15
        BONE_IDX_RIGHT_FOOT_3,  // 16
        BONE_IDX_RIGHT_FOOT_4,  // 17
        BONE_IDX_RIGHT_HAND_1,  // 18
        BONE_IDX_RIGHT_HAND_2,  // 19
        BONE_IDX_RIGHT_HAND_3,  // 20
        BONE_IDX_RIGHT_HAND_4,  // 21
        BONE_IDX_COUNT
    };
}


namespace std {
    template<>
    struct formatter<SDK::Vec3> {
        constexpr auto parse(format_parse_context& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}') {
                throw format_error("invalid format specifier for Vec3");
            }
            return it;
        }

        auto format(const SDK::Vec3& v, format_context& ctx) const {
            return format_to(ctx.out(), "({}, {}, {})", v.X, v.Y, v.Z);
        }
    };

    template<>
    struct formatter<SDK::Vec2> {
        constexpr auto parse(format_parse_context& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}') {
                throw format_error("invalid format specifier for Vec3");
            }
            return it;
        }

        auto format(const SDK::Vec2& v, format_context& ctx) const {
            return format_to(ctx.out(), "({}, {})", v.X, v.Y);
        }
    };
}