#pragma once

#include "vec3.hpp"

#include <vector>
#include <array>

namespace iheay::objmodel {

    class Mesh {

    public:
        Mesh(const std::vector<iheay::math::Vec3>& v, 
             const std::vector<std::vector<int>>& i) : m_vertices(v), m_indexes(i) { }

        const std::vector<iheay::math::Vec3>& get_vertices() const { return m_vertices; }
        const std::vector<std::vector<int>>& get_indexes() const { return m_indexes; }

    private:
        std::vector<iheay::math::Vec3> m_vertices;
        std::vector<std::vector<int>> m_indexes;

    };

}
