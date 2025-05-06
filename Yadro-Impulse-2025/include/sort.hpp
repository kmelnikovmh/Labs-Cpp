#ifndef SORT_HPP_
#define SORT_HPP_

#include <string>

#include <tape.hpp>

namespace tape_sort {

class SortTape {
private:
    const std::string m_input_path;
    const std::string m_output_path;
    const std::string m_config_path;
    const std::string m_tmp_path;
    int m_max_files;
    int m_max_ram;

public:
    SortTape(const std::string& input_path,
             const std::string& output_path,
             const std::string& config_path,
             const std::string& tmp_path,
             int max_files,
             int max_ram);

    std::string split_and_sort_one_backet(Tape& input_tape, int& counter);
    void merge_two_tapes_in_first(Tape& lhs, Tape& rhs);

    void external_sort();
};
}  // namespace tape_sort
#endif
