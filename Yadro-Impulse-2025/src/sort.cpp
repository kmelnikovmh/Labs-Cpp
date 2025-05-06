#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

#include <sort.hpp>

namespace tape_sort {

SortTape::SortTape(const std::string& input_path,
                   const std::string& output_path,
                   const std::string& config_path,
                   const std::string& tmp_path,
                   int max_files,
                   int max_ram)
    : m_input_path(input_path),
      m_output_path(output_path),
      m_config_path(config_path),
      m_tmp_path(tmp_path),
      m_max_files(max_files),
      m_max_ram(max_ram) {
}

std::string SortTape::split_and_sort_one_backet(Tape& input_tape, int& counter) {
    std::vector<int> buffer;
    buffer.reserve(m_max_ram);

    for (int i = 0; i < m_max_ram && !input_tape.is_eof(); ++i) {
        buffer.push_back(input_tape.read());
        input_tape.next_pos();
    }
    std::sort(buffer.begin(), buffer.end());

    for (int i = 0; i < buffer.size(); ++i) {
        std::cout << buffer[i] << " ";
    }

    std::string file_path = m_tmp_path + "/tape_" + std::to_string(counter++) + ".temp";
    create_file_tape(file_path, buffer.size());
    Tape output_tape(file_path, buffer.size(), m_config_path);

    for (auto val : buffer) {
        output_tape.write(val);
        output_tape.next_pos();
    }

    return file_path;
}

void SortTape::merge_two_tapes_in_first(Tape& lhs, Tape& rhs) {
    std::string tmp_merge_name = m_tmp_path + "merge.temp";
    create_file_tape(tmp_merge_name, lhs.size() + rhs.size());
    Tape tmp_merge_tape(tmp_merge_name, lhs.size() + rhs.size(), m_config_path);

    bool lhs_valid = !lhs.is_eof();
    bool rhs_valid = !rhs.is_eof();

    int lhs_val = lhs_valid ? lhs.read() : 0;
    int rhs_val = rhs_valid ? rhs.read() : 0;

    while (lhs_valid && rhs_valid) {
        if (lhs_val <= rhs_val) {
            tmp_merge_tape.write(lhs_val);
            tmp_merge_tape.next_pos();
            lhs.next_pos();
            lhs_valid = !lhs.is_eof();
            if (lhs_valid) {
                lhs_val = lhs.read();
            }
        } else {
            tmp_merge_tape.write(rhs_val);
            tmp_merge_tape.next_pos();
            rhs.next_pos();
            rhs_valid = !rhs.is_eof();
            if (rhs_valid) {
                rhs_val = rhs.read();
            }
        }
    }

    while (lhs_valid) {
        tmp_merge_tape.write(lhs_val);
        tmp_merge_tape.next_pos();
        lhs.next_pos();
        lhs_valid = !lhs.is_eof();
        if (lhs_valid) {
            lhs_val = lhs.read();
        }
    }

    while (rhs_valid) {
        tmp_merge_tape.write(rhs_val);
        tmp_merge_tape.next_pos();
        rhs.next_pos();
        rhs_valid = !rhs.is_eof();
        if (rhs_valid) {
            rhs_val = rhs.read();
        }
    }

    std::filesystem::path tmp_merge_path(tmp_merge_name);
    std::filesystem::path lhs_path(lhs.get_file_path());
    std::filesystem::remove(lhs_path);
    std::filesystem::rename(tmp_merge_path, lhs_path);
}

void SortTape::external_sort() {
    int input_size = std::filesystem::file_size(m_input_path) / sizeof(int);
    Tape input_tape(m_input_path, input_size, m_config_path);

    create_file_tape(m_output_path, 0);

    std::vector<std::string> tape_pool;
    int file_counter = 0;

    while (!input_tape.is_eof()) {
        std::string backet = split_and_sort_one_backet(input_tape, file_counter);
        tape_pool.push_back(backet);

        if (tape_pool.size() == m_max_files) {
            // Нерекурсивная версия mergesort без копирования памяти
            for (int k = 0; (1 << k) < m_max_files; k++) {
                for (int i = 0; i < m_max_files; i += 2 * (1 << k)) {
                    int lhs_size = std::filesystem::file_size(tape_pool[i]) / sizeof(int);
                    int rhs_size =
                        std::filesystem::file_size(tape_pool[i + (1 << k)]) / sizeof(int);
                    Tape lhs(tape_pool[i], lhs_size, m_config_path);
                    Tape rhs(tape_pool[i + (1 << k)], rhs_size, m_config_path);
                    merge_two_tapes_in_first(lhs, rhs);
                }
            }

            int last_tape_pool_size = std::filesystem::file_size(tape_pool[0]) / sizeof(int);
            Tape last_tape_pool(tape_pool[0], last_tape_pool_size, m_config_path);
            int temp_result_size = std::filesystem::file_size(m_output_path) / sizeof(int);
            Tape temp_result(m_output_path, temp_result_size, m_config_path);
            merge_two_tapes_in_first(temp_result, last_tape_pool);

            for (int i = 0; i < m_max_files; ++i) {
                std::filesystem::path tape_path(tape_pool[i]);
                std::filesystem::remove(tape_path);
                tape_pool.clear();
            }

            file_counter = 0;
        }
    }

    std::cout << std::endl << tape_pool.size() << std::endl;

    if (tape_pool.size() != 0) {
        for (int i = 1; i < tape_pool.size(); ++i) {
            int lhs_size = std::filesystem::file_size(tape_pool[0]) / sizeof(int);
            int rhs_size = std::filesystem::file_size(tape_pool[i]) / sizeof(int);
            Tape lhs(tape_pool[0], lhs_size, m_config_path);
            Tape rhs(tape_pool[i], rhs_size, m_config_path);
            merge_two_tapes_in_first(lhs, rhs);

            std::filesystem::path tape_path(tape_pool[i]);
            std::filesystem::remove(tape_path);
        }

        int last_tape_pool_size = std::filesystem::file_size(tape_pool[0]) / sizeof(int);
        Tape last_tape_pool(tape_pool[0], last_tape_pool_size, m_config_path);
        int temp_result_size = std::filesystem::file_size(m_output_path) / sizeof(int);
        Tape temp_result(m_output_path, temp_result_size, m_config_path);
        merge_two_tapes_in_first(temp_result, last_tape_pool);
    }

    std::filesystem::remove(tape_pool[0]);

    std::cout << "Stop\n\n";
}

}  // namespace tape_sort
