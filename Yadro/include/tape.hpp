#ifndef TAPE_HPP_
#define TAPE_HPP_

#include <fstream>
#include <string>

namespace tape_sort {

struct tape_error : std::runtime_error {
    explicit tape_error(const std::string& comment);
};

struct invalid_file_error : tape_error {
    explicit invalid_file_error(const std::string& comment);
};

void create_file_tape(const std::string& file_path, int size_tape);

class Tape {
private:
    const std::string m_file_path;
    const int m_lenght_elem;
    std::fstream m_tape_file;
    std::streampos m_pos;
    int m_read_delay_ms;
    int m_write_delay_ms;
    int m_next_pos_delay_ms;
    int m_rewind_delay_ms;

    bool m_is_all_read = false;

    void m_delay(int curr_delay);

public:
    Tape(const std::string& file_path, int lenght, const std::string& config_path);
    ~Tape();

    int size();
    std::string get_file_path();
    bool is_eof();

    int read();
    void write(int num);
    bool next_pos();
    void rewind();
};

}  // namespace tape_sort
#endif
