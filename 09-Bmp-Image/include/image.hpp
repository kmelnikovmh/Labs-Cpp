#ifndef BMP_IMAGE_HPP_
#define BMP_IMAGE_HPP_

#include <stdexcept>
#include <string>
#include <vector>

namespace lab_bmp {

struct unable_to_open_file_error : std::runtime_error {
    explicit unable_to_open_file_error(const std::string &filename);
};

struct invalid_image_error : std::runtime_error {
    explicit invalid_image_error(const std::string &statement);
};

struct invalid_version_error : invalid_image_error {
    explicit invalid_version_error(uint32_t biSize);
};

struct non_positive_height_error : invalid_image_error {
    explicit non_positive_height_error(int32_t biHeight);
};

struct wrong_bits_per_pixel_error : invalid_image_error {
    explicit wrong_bits_per_pixel_error(uint16_t biBitCount);
};

struct color_palette_used_error : invalid_image_error {
    explicit color_palette_used_error();
};

struct subimage_is_out_error : std::runtime_error {
    explicit subimage_is_out_error();
};

const int BITMAPFILEHEADER_SIZE = 14;
const int BITMAPINFOHEADER_V3_SIZE = 40;
const int HEADERS_SIZE = BITMAPFILEHEADER_SIZE + BITMAPINFOHEADER_V3_SIZE;
const int BIBITCOUNT_SUPPORTED = 24;
const int BYTES_PER_PIXEL = BIBITCOUNT_SUPPORTED / 8;

class image {
private:
// Headers
#pragma pack(push, 1)

    struct BITMAPFILEHEADER {
        uint16_t bfType;
        uint32_t bfSize;
        uint16_t bfReserved1;
        uint16_t bfReserved2;
        uint32_t bfOffBits;
    };

    struct BITMAPINFOHEADER_V3 {
        uint32_t biSize;
        int32_t biWidth;
        int32_t biHeight;
        uint16_t biPlanes;
        uint16_t biBitCount;
        uint32_t biCompression;
        uint32_t biSizeImage;
        int32_t biXPelsPerMeter;
        int32_t biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t biClrImportant;
    };

#pragma pack(pop)

    BITMAPFILEHEADER
    m_file_header;
    BITMAPINFOHEADER_V3
    m_info_header;
    std::vector<uint8_t> m_data_pixels;

    void validate_headers() const;

public:
    image();
    void load_image(const std::string &file_path);
    void crop(int32_t ul_corner_x, int32_t ul_corner_y, int32_t width, int32_t height);
    void rotate_clockwise();
    void export_image(const std::string &file_path);
};
}  // namespace
   // lab_bmp
#endif