#pragma once // bmp_io.hpp

#include <fstream>
#include <stdexcept>
#include <cstring>

#include "bmp.hpp"

namespace iheay::bmp {

    class BmpIO {
        
    public:
        static Bmp load(const std::string& path);
        static void save(const Bmp& bmp, const std::string& path);

    private:
        static uint32_t row_size_bytes(int width);

    };

}
