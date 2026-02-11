#ifndef ASSMPQ_TEST_H_
#define ASSMPQ_TEST_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

namespace assmpq::test {

    inline auto load_file(const std::string& filename)-> std::vector<char>
    {
        std::vector<char> buffer;
        std::ifstream file(filename, std::ios::binary);

        if (file.is_open()) {
            file.seekg(0, std::ios::end);
            buffer.resize(static_cast<size_t>(file.tellg()));
            file.seekg(0, std::ios::beg);
            file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
            file.close();
        } else {
            // Handle error: e.g., throw an exception or return an empty vector
            std::cerr << "Error opening file: " << filename << "\n";
        }

        return buffer;
    }

}

#endif // ASSMPQ_TEST_H_