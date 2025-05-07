#include <fstream>
#include <iostream>
#include <queue>

#include <primers.hpp>

namespace primers {
unable_output_file_error::unable_output_file_error()
    : std::runtime_error("Unable output answer to file") {};

void Primers::read_dna(const std::string& file_data_path) {
    const std::filesystem::path filename(file_data_path);
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw unable_output_file_error();
    }

    std::string buff;
    int id = 0;
    while (file >> buff) {
        if (buff[0] == '>') {
            ++id;
            continue;
        }

        const char* p = &buff[0];

        bool has_primer = false;
        for (int d = 20; d <= 24; ++d) {
            int gc = 0;
            for (int j = 0; j < d; ++j) {
                if (p[j] == 'g' || p[j] == 'c') {
                    ++gc;
                }
            }

            std::string primer(p, d);
            // 0.5 <= gc/d && gc/d <= 0.7
            if (d <= gc * 2 && 10 * gc <= 7 * d) {
                m_primers[primer].insert(id);
                has_primer = true;
            }

            const std::string bases = "atgc";
            for (int pos = 0; pos < d; ++pos) {
                for (char b : bases) {
                    if (p[pos] == b) {
                        continue;
                    }

                    std::string mutant(p, d);
                    mutant[pos] = b;

                    int new_gc = gc;
                    if (p[pos] == 'g' || p[pos] == 'c') {
                        --new_gc;
                    }
                    if (b == 'g' || b == 'c') {
                        ++new_gc;
                    }

                    if (d <= new_gc * 2 && 10 * new_gc <= 7 * d) {
                        m_primers[mutant].insert(id);
                        has_primer = true;
                    }
                }
            }
        }
        if (has_primer == false) {
            std::cout << "Sequence id: " << id << " - does not have the required primer\n";
        }
    }
    m_total_sequence = id;
}

std::vector<std::pair<std::string, std::unordered_set<int>>> Primers::greedy_cover() {
    std::unordered_set<int> uncovered;
    for (int i = 0; i < m_total_sequence; ++i) {
        uncovered.insert(i);
    }

    std::priority_queue<std::pair<int, std::string>> pq;

    for (const auto& [primer, covers] : m_primers) {
        pq.emplace(covers.size(), primer);
    }

    std::vector<std::pair<std::string, std::unordered_set<int>>> result;

    while (!uncovered.empty() && !pq.empty()) {
        auto [count_covers, primer] = pq.top();
        pq.pop();

        if (count_covers == 0) {
            continue;
        }

        const auto& covered_set = m_primers.at(primer);
        std::unordered_set<int> effect;
        for (auto id : covered_set) {
            if (uncovered.count(id) != 0) {
                effect.insert(id);
            }
        }

        if (effect.empty()) {
            continue;
        }

        result.push_back({primer, covered_set});
        for (auto id : effect) {
            uncovered.erase(id);
        }

        // Update prior queue
        std::priority_queue<std::pair<int, std::string>> new_pq;
        for (const auto& [primer, covers] : m_primers) {
            int uncovered_count = 0;
            for (auto id : covers) {
                if (uncovered.count(id) > 0) {
                    ++uncovered_count;
                }
            }
            new_pq.emplace(uncovered_count, primer);
        }
        std::swap(pq, new_pq);
    }

    return result;
}

std::vector<std::pair<std::string, std::unordered_set<int>>> Primers::ilp_cover() {
    return this->greedy_cover();
}
}  // namespace primers
