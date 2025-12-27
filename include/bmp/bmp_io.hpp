#pragma once // bmp/bmp_io.hpp

#include <string>
#include "bmp/bmp.hpp"

namespace iheay::bmp {

class BmpIO {
public:
    static Bmp load(const std::string& path);
    static void save(const Bmp& bmp, const std::string& path);
};

} // namespace iheay::bmp
