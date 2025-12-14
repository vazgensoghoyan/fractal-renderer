#ifndef MESH_HPP
#define MESH_HPP

#include "vec3.hpp"

#include <vector>
#include <array>

namespace iheay::objmodel {

    class Mesh {

    public:
        Mesh(const std::vector<iheay::math::Vec3>& v, 
             const std::vector<std::vector<int>>& i) : vetrices_(v), indexes_(i) { }

        const std::vector<iheay::math::Vec3>& get_vertices() const { return vetrices_; }
        const std::vector<std::vector<int>>& get_indexes() const { return indexes_; }

    private:
        std::vector<iheay::math::Vec3> vetrices_;
        std::vector<std::vector<int>> indexes_;

    };

}

#endif // MESH_HPP