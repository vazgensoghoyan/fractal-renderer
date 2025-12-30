#include <gtest/gtest.h>
#include <vector>
#include <fstream>
#include <cstdio>

#include "bmp/bmp.hpp"
#include "bmp/io/bmp_io.hpp"

using namespace iheay::bmp;

TEST(BmpTest, ConstructorAndEmpty) {
    std::vector<BgrPixel> pixels(4, {255, 0, 0});
    Bmp bmp(2, 2, pixels);

    EXPECT_EQ(bmp.width(), 2);
    EXPECT_EQ(bmp.height(), 2);
    EXPECT_EQ(bmp.pixels().size(), 4);

    auto empty_bmp = Bmp::empty(3, 3);
    EXPECT_EQ(empty_bmp.width(), 3);
    EXPECT_EQ(empty_bmp.height(), 3);
    for (const auto& p : empty_bmp.pixels())
        EXPECT_EQ(p.r, 255);  // white
}

TEST(BmpTest, ConstructorThrowsOnInvalidSizes) {
    std::vector<BgrPixel> pixels(4);
    EXPECT_THROW(Bmp(-1, 2, pixels), std::runtime_error);
    EXPECT_THROW(Bmp(2, 0, pixels), std::runtime_error);
    EXPECT_THROW(Bmp(2, 2, std::vector<BgrPixel>(3)), std::runtime_error); // size mismatch
}

TEST(BmpTest, GetSetPixel) {
    Bmp bmp = Bmp::empty(2, 2);

    bmp.set_pixel(0, 0, {10, 20, 30});
    EXPECT_EQ(bmp.get_pixel(0, 0).r, 30);
    EXPECT_EQ(bmp.get_pixel(0, 0).g, 20);
    EXPECT_EQ(bmp.get_pixel(0, 0).b, 10);

    EXPECT_TRUE(bmp.try_set_pixel(1, 1, {1, 2, 3}));
    EXPECT_FALSE(bmp.try_set_pixel(2, 2, {0, 0, 0}));

    EXPECT_THROW(bmp.set_pixel(-1, 0, {0,0,0}), std::runtime_error);
    EXPECT_THROW(bmp.get_pixel(0, 2), std::runtime_error);
}

TEST(BmpTest, BottomUpAndPadding) {
    // Создадим 3x2 изображение (ширина 3, padding должен быть 3*3=9 байт -> padded to 12)
    std::vector<BgrPixel> pixels = {
        {0,0,0}, {1,1,1}, {2,2,2},
        {3,3,3}, {4,4,4}, {5,5,5}
    };
    Bmp bmp(3, 2, pixels);

    // Проверяем сами пиксели
    EXPECT_EQ(bmp.get_pixel(0,0).r, 0);
    EXPECT_EQ(bmp.get_pixel(2,0).r, 2);
    EXPECT_EQ(bmp.get_pixel(0,1).r, 3);
    EXPECT_EQ(bmp.get_pixel(2,1).r, 5);
}

TEST(BmpTest, SaveAndLoad) {
    const char* filename = "test.bmp";
    std::vector<BgrPixel> pixels = {
        {10,20,30}, {40,50,60},
        {70,80,90}, {100,110,120}
    };
    Bmp bmp(2, 2, pixels);

    io::save(bmp, filename);

    Bmp loaded = io::load(filename);
    EXPECT_EQ(loaded.width(), 2);
    EXPECT_EQ(loaded.height(), 2);

    for (int y = 0; y < 2; ++y)
        for (int x = 0; x < 2; ++x)
            EXPECT_EQ(loaded.get_pixel(x,y).r, bmp.get_pixel(x,y).r);

    std::remove(filename);
}

TEST(BmpTest, LoadInvalidFileThrows) {
    const char* filename = "nonexistent.bmp";
    EXPECT_THROW(io::load(filename), std::runtime_error);

    // Создадим пустой файл
    std::ofstream f("empty.bmp");
    f.close();
    EXPECT_THROW(io::load("empty.bmp"), std::runtime_error);
    std::remove("empty.bmp");

    // Создадим файл с неверной сигнатурой
    std::ofstream f2("bad.bmp", std::ios::binary);
    f2.put('X'); f2.put('Y');
    f2.close();
    EXPECT_THROW(io::load("bad.bmp"), std::runtime_error);
    std::remove("bad.bmp");
}

TEST(BmpTest, LargeImageThrows) {
    std::vector<BgrPixel> pixels;
    // ширина*высота > 1e9
    EXPECT_THROW(Bmp::empty(40000, 40000), std::runtime_error);
}

TEST(BmpTest, SaveInvalidPathThrows) {
    Bmp bmp = Bmp::empty(2, 2);
    EXPECT_THROW(io::save(bmp, "/invalid/path.bmp"), std::runtime_error);
}
