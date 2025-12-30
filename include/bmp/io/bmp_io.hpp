#pragma once // bmp/io/bmp_io.hpp

#include "bmp/bmp.hpp"
#include <string>

namespace iheay::bmp::io {

Bmp load(const std::string& path);
void save(const Bmp& bmp, const std::string& path);

} // namespace io
