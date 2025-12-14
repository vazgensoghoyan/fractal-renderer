#ifndef VEC3_HPP
#define VEC3_HPP

namespace iheay::math {

    class Vec3 {

    public:
        Vec3(double x, double y, double z);

        Vec3 operator+(const Vec3& other) const;
        Vec3 operator-(const Vec3& other) const;

        Vec3 operator*(double scalar) const;
        Vec3 operator/(double scalar) const;

        Vec3 normalize() const;

    private:
        double x_;
        double y_;
        double z_;

    };

}

#endif // VEC3_HPP
