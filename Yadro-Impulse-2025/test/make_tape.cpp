#include <fstream>
#include <iostream>
#include <string>
#include <random>

void create_file_tape(const std::string& file_path, int size_tape) {
    std::ofstream file(file_path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Не удалось создать файл: " + file_path);
    }

    std::mt19937 rng(239);
    int a = -232, b = 232;
    for (int i = 0; i < size_tape; ++i) {
        int value = a + rng() % (b - a + 1);
        file.write(reinterpret_cast<char*>(&value), sizeof(int));
    }

    file.close();
}

void read_ints_from_binary_file(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Не удалось открыть файл для чтения: " + file_path);
    }

    std::cout << "Создан файл с данными:\n";
    int value;
    while (file.read(reinterpret_cast<char*>(&value), sizeof(int))) {
        std::cout << value << " ";
    }
    std::cout << "\n";

    file.close();
}

int main() {
    std::cout << "Input filepath and lenght\n";
    std::string new_file_path;
    int lenght_elem;
    std::cin >> new_file_path >> lenght_elem;

    create_file_tape(new_file_path, lenght_elem);
    read_ints_from_binary_file(new_file_path);
}