#ifndef PRIMERS_HPP_
#define PRIMERS_HPP_

#include <exception>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace primers {

struct unable_output_file_error : std::runtime_error {
    explicit unable_output_file_error();
};

class Primers {
private:
    int m_total_sequence;
    std::unordered_map<std::string, std::unordered_set<int>> m_primers;

public:
    void read_dna(const std::string& file_data_path);

    std::vector<std::pair<std::string, std::unordered_set<int>>> greedy_cover();
    std::vector<std::pair<std::string, std::unordered_set<int>>> ilp_cover();
};
}  // namespace primers
#endif
