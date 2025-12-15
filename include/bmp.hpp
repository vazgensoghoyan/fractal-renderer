#pragma once

#include "bmp_structs.hpp"

#include <cstdint>
#include <vector>
#include <string>


class Bmp {
public:
    void load(const std::string& path);
    void save(const std::string& path) const;

    const Pixel& at(int x, int y) const;

    void set_pixel();

private:
    static constexpr int ALIGNMENT = 4;

    BmpFileHeader fileHeader{};
    BmpV5Header v5Header{};

    std::vector<Pixel> pixels;

    int padding() const;
};
