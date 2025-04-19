#include <cassert>
#include <exception>
#include <image.hpp>
#include <iostream>
#include <type_traits>

namespace lab_bmp {
struct missing_arguments_error : std::runtime_error {
    explicit missing_arguments_error() : std::runtime_error("Missing arguments") {
    }
};
}  // namespace lab_bmp

int main(int argc, const char *argv[]) {
    assert(std::is_copy_constructible_v<lab_bmp::image>);
    assert(std::is_move_constructible_v<lab_bmp::image>);
    assert(std::is_copy_assignable_v<lab_bmp::image>);
    assert(std::is_move_assignable_v<lab_bmp::image>);

    try {
        if (argc != 8) {
            throw lab_bmp::missing_arguments_error();
        }

        const std::string in_file = argv[2];
        const std::string out_file = argv[3];
        const int x = std::stoi(argv[4]);
        const int y = std::stoi(argv[5]);
        const int w = std::stoi(argv[6]);
        const int h = std::stoi(argv[7]);

        lab_bmp::image main_image{};
        main_image.load_image(in_file);

        main_image.crop(x, y, w, h);
        main_image.rotate_clockwise();

        main_image.export_image(out_file);
    } catch (const std::bad_alloc &) {
        std::cerr << "Insufficient memory\n";
        return 1;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}