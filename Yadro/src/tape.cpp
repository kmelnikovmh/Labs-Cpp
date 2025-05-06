#include "../include/tape.hpp"
#include <chrono>
#include <thread>

namespace tape_sort {

tape_error::tape_error(const std::string& comment) : std::runtime_error(comment) {};

invalid_file_error::invalid_file_error(const std::string& comment)
    : tape_error("Invalid file: " + comment) {};

void create_file_tape(const std::string& file_path, int size_tape) {
    std::ofstream file(file_path, std::ios::binary);
    if (!file) {
        throw invalid_file_error(file_path + " can't create");
    }

    int zero = 0;
    for (int i = 0; i < size_tape; ++i) {
        file.write(reinterpret_cast<char*>(&zero), sizeof(int));
    }

    file.close();
}

// TAPE
Tape::Tape(const std::string& file_path, int lenght, const std::string& config_path)
    : m_file_path(file_path), m_lenght_elem(lenght), m_pos(0) {
    const std::filesystem::path file_name(file_path);
    m_tape_file.open(file_name, std::ios::out | std::ios::in | std::ios::binary);
    if (!m_tape_file) {
        throw invalid_file_error(file_path);
    }

    const std::filesystem::path config_name(config_path);
    std::ifstream config_file(config_name);
    if (!config_file) {
        throw invalid_file_error(config_path);
    }
    config_file.exceptions(std::ios_base::failbit | std::ios_base::badbit);
    config_file >> m_read_delay_ms >> m_write_delay_ms >> m_next_pos_delay_ms >> m_rewind_delay_ms;
}

Tape::~Tape() {
    m_tape_file.close();
}

void Tape::m_delay(int curr_delay) {
    std::this_thread::sleep_for(std::chrono::milliseconds(curr_delay));
}

int Tape::size() {
    return m_lenght_elem;
}

std::string Tape::get_file_path() {
    return m_file_path;
}

bool Tape::is_eof() {
    if (m_lenght_elem == 0) {
        return true;
    }
    return m_is_all_read;
}

int Tape::read() {
    m_delay(m_read_delay_ms);

    int num;
    m_tape_file.seekg(m_pos, std::ios::beg);
    m_tape_file.read(reinterpret_cast<char*>(&num), sizeof(int));

    if (m_pos >= m_lenght_elem * sizeof(int) - sizeof(int)) {
        m_is_all_read = true;
    }
    return num;
}

void Tape::write(int num) {
    m_delay(m_write_delay_ms);

    m_tape_file.seekp(m_pos, std::ios::beg);
    m_tape_file.write(reinterpret_cast<char*>(&num), sizeof(int));
}

bool Tape::next_pos() {
    if (m_pos >= m_lenght_elem * sizeof(int) - sizeof(int)) {
        return false;
    }

    m_delay(m_next_pos_delay_ms);
    m_pos += sizeof(int);

    return true;
}

void Tape::rewind() {
    m_delay(m_rewind_delay_ms);

    m_tape_file.clear();

    m_pos = 0;
}
}  // namespace tape_sort
