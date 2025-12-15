#pragma once

#include <cmath>

namespace iheay::math {

    class Vec3 {

    public:
        Vec3(double x, double y, double z);

        double get_length() const {
            return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
        }

        Vec3 operator-() const { 
            return Vec3(-m_x, -m_y, -m_z); 
        }

        Vec3 operator+(const Vec3& o) const { 
            return Vec3(m_x + o.m_x, m_y + o.m_y, m_z + o.m_z); 
        }

        Vec3 operator-(const Vec3& o) const { 
            return *this + (-o); 
        }

        Vec3 operator*(double scalar) const { 
            return Vec3(m_x * scalar, m_y * scalar, m_z * scalar); 
        }

        Vec3 operator/(double scalar) const {
            return Vec3(m_x / scalar, m_y / scalar, m_z / scalar);
        }

        Vec3 normalize() const {
            return *this / get_length();
        }

    private:
        double m_x;
        double m_y;
        double m_z;

    };

}
