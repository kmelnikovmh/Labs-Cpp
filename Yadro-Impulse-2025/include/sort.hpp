#ifndef SORT_HPP_
#define SORT_HPP_

namespace tape_sort {

void external_sort(const std::string& input_path,
                   const std::string& output_path,
                   const std::string& config_path,
                   const std::string& tmp_path,
                   int max_files,
                   int max_ram);
}
#endif
