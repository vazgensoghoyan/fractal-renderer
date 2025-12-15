#pragma once

#include <cmath>

namespace iheay::math {

    class Vec3 {

    public:
        Vec3(double x, double y, double z);

        double get_length() const {
            return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
        }

        Vec3 operator-() const { 
            return Vec3(-x_, -y_, -z_); 
        }

        Vec3 operator+(const Vec3& o) const { 
            return Vec3(x_ + o.x_, y_ + o.y_, z_ + o.z_); 
        }

        Vec3 operator-(const Vec3& o) const { 
            return *this + (-o); 
        }

        Vec3 operator*(double scalar) const { 
            return Vec3(x_ * scalar, y_ * scalar, z_ * scalar); 
        }

        Vec3 operator/(double scalar) const {
            return Vec3(x_ / scalar, y_ / scalar, z_ / scalar);
        }

        Vec3 normalize() const {
            return *this / get_length();
        }

    private:
        double x_;
        double y_;
        double z_;

    };

}
