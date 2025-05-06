#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <filesystem>

#include <sort.hpp>
#include <tape.hpp>

std::string path_to_dir = "../test/data-test/";
std::string config_path = "../tape_config.txt";
std::string tmp_path = "../tmp/";
int max_files = 4;
int max_ram = 32;

TEST_CASE("Create and work with tape") {

    int first_size = std::filesystem::file_size(path_to_dir + "first.data") / sizeof(int);
    CHECK(first_size == 149);
    
    tape_sort::Tape m_tape(path_to_dir + "first.data", first_size, config_path);
    int val = m_tape.read();
    CHECK(val == m_tape.read());

    m_tape.next_pos();
    CHECK(val != m_tape.read());

    for (int i = 1; i <= 148; ++i) m_tape.next_pos();
    CHECK(m_tape.next_pos() == false);
    CHECK(m_tape.is_eof() == false);
    m_tape.read();
    CHECK(m_tape.is_eof() == true);

    m_tape.rewind();
    CHECK(m_tape.is_eof() == false);
    CHECK(m_tape.next_pos() == true);

    int val_2 = m_tape.read();
    m_tape.write(val_2 - 1);
    CHECK(val_2 != m_tape.read());
}

TEST_CASE("Create and work with SortTape") {
    std::string input_file = "first.data";
    int input_file_size = std::filesystem::file_size(path_to_dir + input_file) / sizeof(int);
    std::string output_file = "first.data.out";

    tape_sort::SortTape m_sort(path_to_dir + input_file, path_to_dir + output_file, config_path, tmp_path, max_files, max_ram);
    m_sort.external_sort();

    int output_file_size = std::filesystem::file_size(path_to_dir + output_file) / sizeof(int);
    CHECK(input_file_size == output_file_size);

    bool right_sort = true;
    tape_sort::Tape m_tape(path_to_dir + "first.data.out", output_file_size, config_path);
    int lhs_val = m_tape.read();
    m_tape.next_pos();
    while(!m_tape.is_eof() && right_sort) {
        int rhs_val = m_tape.read();
        if (lhs_val > rhs_val) {
            right_sort = false;
        }

        lhs_val = m_tape.read();
        m_tape.next_pos();
    }
    CHECK(right_sort == true);

}