#ifndef MESH_HPP
#define MESH_HPP

#include "vec3.hpp"

#include <vector>
#include <array>

namespace iheay::objmodel {

    class Mesh {

    public:
        Mesh(std::vector<iheay::math::Vec3> v, std::vector<std::vector<int>> i);

        const std::vector<iheay::math::Vec3>& get_vertices() const;
        const std::vector<std::vector<int>>& get_indexes() const;

    private:
        std::vector<iheay::math::Vec3> vetrices_;
        std::vector<std::vector<int>> indexes_;

    };

}

#endif // MESH_HPP