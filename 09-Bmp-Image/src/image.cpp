#include <cstring>
#include <fstream>
#include <image.hpp>

namespace lab_bmp {

unable_to_open_file_error::unable_to_open_file_error(const std::string &filename)
    : std::runtime_error("Unable to open file \"" + filename + "\"") {
}

invalid_image_error::invalid_image_error(const std::string &statement)
    : std::runtime_error("Invalid BMP: " + statement) {
}

invalid_version_error::invalid_version_error(uint32_t biSize)
    : invalid_image_error("expected version 3 and header size " +
                          std::to_string(BITMAPINFOHEADER_V3_SIZE) + " , but header size is " +
                          std::to_string(biSize)) {
}

non_positive_height_error::non_positive_height_error(int32_t biHeight)
    : invalid_image_error("expected positive biHeight, got " + std::to_string(biHeight)) {
}

wrong_bits_per_pixel_error::wrong_bits_per_pixel_error(uint16_t biBitCount)
    : invalid_image_error("expected " + std::to_string(BIBITCOUNT_SUPPORTED) +
                          " bits per pixel, got " + std::to_string(biBitCount)) {
}

color_palette_used_error::color_palette_used_error()
    : invalid_image_error("color palette is unsupported") {
}

subimage_is_out_error::subimage_is_out_error()
    : std::runtime_error("The requested area is not a subimage") {
}

// IMAGE
image::image() : m_file_header{}, m_info_header{} {
}

void image::load_image(const std::string &file_path) {
    std::ifstream filestream(file_path, std::ios_base::binary);

    if (!filestream) {
        throw unable_to_open_file_error(file_path);
    }

    filestream.read(reinterpret_cast<char *>(&m_file_header), HEADERS_SIZE);
    validate_headers();

    m_data_pixels.resize(static_cast<size_t>(m_info_header.biSizeImage));
    filestream.read(reinterpret_cast<char *>(m_data_pixels.data()),
                    static_cast<std::streamsize>(m_info_header.biSizeImage));
}

void image::crop(int32_t ul_corner_x, int32_t ul_corner_y, int32_t width, int32_t height) {
    if (m_info_header.biWidth < ul_corner_x + width ||
        m_info_header.biHeight < ul_corner_y + height) {
        throw subimage_is_out_error{};
    }

    const int32_t row_size_old = (m_info_header.biWidth * BYTES_PER_PIXEL + 3) & ~3;
    const int32_t row_size_new = (width * BYTES_PER_PIXEL + 3) & ~3;

    std::vector<uint8_t> new_data(
        static_cast<std::vector<uint8_t>::size_type>(height * row_size_new), 0);

    for (int32_t curr_row = 0; curr_row < height; ++curr_row) {
        const int32_t src_row = m_info_header.biHeight - ul_corner_y - height + curr_row;

        uint8_t *src_ptr = m_data_pixels.data() + static_cast<ptrdiff_t>(src_row * row_size_old) +
                           static_cast<ptrdiff_t>(ul_corner_x * BYTES_PER_PIXEL);
        uint8_t *dest_ptr = new_data.data() + static_cast<ptrdiff_t>(curr_row * row_size_new);

        std::memcpy(dest_ptr, src_ptr, static_cast<size_t>(width) * BYTES_PER_PIXEL);
    }

    m_data_pixels = std::move(new_data);

    m_info_header.biWidth = width;
    m_info_header.biHeight = height;
    m_info_header.biSizeImage = static_cast<uint32_t>(m_data_pixels.size());
    m_file_header.bfSize = HEADERS_SIZE + m_info_header.biSizeImage;
}

void image::rotate_clockwise() {
    const int32_t row_size_old = (m_info_header.biWidth * BYTES_PER_PIXEL + 3) & ~3;
    const int32_t new_width = m_info_header.biHeight;
    const int32_t new_height = m_info_header.biWidth;
    const int32_t row_size_new = (new_width * BYTES_PER_PIXEL + 3) & ~3;

    std::vector<uint8_t> rotated_data(
        static_cast<std::vector<uint8_t>::size_type>(new_height * row_size_new), 0);

    for (int32_t curr_row = 0; curr_row < m_info_header.biHeight; ++curr_row) {
        for (int32_t curr_col = 0; curr_col < m_info_header.biWidth; ++curr_col) {
            const int32_t src_row = m_info_header.biHeight - curr_row - 1;
            const int32_t dest_row = new_height - curr_col - 1;
            const int32_t dest_col = src_row;

            uint8_t *src_ptr = m_data_pixels.data() +
                               static_cast<ptrdiff_t>(src_row * row_size_old) +
                               static_cast<ptrdiff_t>(curr_col * BYTES_PER_PIXEL);
            uint8_t *dest_ptr = rotated_data.data() +
                                static_cast<ptrdiff_t>(dest_row * row_size_new) +
                                static_cast<ptrdiff_t>(dest_col * BYTES_PER_PIXEL);

            std::memcpy(dest_ptr, src_ptr, BYTES_PER_PIXEL);
        }
    }

    m_data_pixels = std::move(rotated_data);

    m_info_header.biWidth = new_width;
    m_info_header.biHeight = new_height;
    m_info_header.biSizeImage = static_cast<uint32_t>(m_data_pixels.size());
    m_file_header.bfSize = HEADERS_SIZE + m_info_header.biSizeImage;
}

void image::export_image(const std::string &file_path) {
    std::ofstream filestream(file_path, std::ios_base::binary);

    if (!filestream) {
        throw unable_to_open_file_error(file_path);
    }

    filestream.write(reinterpret_cast<char *>(&m_file_header), HEADERS_SIZE);
    filestream.write(reinterpret_cast<char *>(m_data_pixels.data()), m_info_header.biSizeImage);
}

void image::validate_headers() const {
    if (m_info_header.biSize != BITMAPINFOHEADER_V3_SIZE) {
        throw invalid_version_error(m_info_header.biSize);
    } else if (m_info_header.biHeight <= 0) {
        throw non_positive_height_error(m_info_header.biHeight);
    } else if (m_info_header.biBitCount != BIBITCOUNT_SUPPORTED) {
        throw wrong_bits_per_pixel_error(m_info_header.biBitCount);
    } else if (m_info_header.biClrUsed != 0) {
        throw color_palette_used_error();
    }
}
}  // namespace lab_bmp