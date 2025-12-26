#pragma once // quaternion.hpp

#include "complex.hpp"
#include "vec3.hpp"

namespace iheay::math {

    class Quaternion {

    public:
        Quaternion(double a, double b, double c, double d);
        Quaternion(Complex z0, Complex z1);

        [[nodiscard]] double get_a() const;
        [[nodiscard]] double get_b() const;
        [[nodiscard]] double get_c() const;
        [[nodiscard]] double get_d() const;
        [[nodiscard]] Complex get_z0() const;
        [[nodiscard]] Complex get_z1() const;
        [[nodiscard]] double get_modulus() const;

        Quaternion operator+(const Quaternion& other) const;
        Quaternion operator-(const Quaternion& other) const;
        Quaternion operator*(const Quaternion& other) const;

        Quaternion operator-() const;
        Quaternion operator~() const; // conjugate 

        Quaternion inverse() const;
        Quaternion pow(int n) const;

        static Vec3 rotate_point(const Vec3& point, const Vec3& u, double phi);

        static Vec3 rotate_x(const Vec3& point, double phi);
        static Vec3 rotate_y(const Vec3& point, double phi);
        static Vec3 rotate_z(const Vec3& point, double phi);

        std::string to_string() const;

    private:

    private:
        Complex m_z0;
        Complex m_z1;

    };

}
