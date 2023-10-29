#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <future>
#include <random>

void createFiles(int numFiles, const std::string& prefix) {
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> distribution(1, 1000); 

    for (int i = 0; i < numFiles; ++i) {
        std::string filename = prefix + std::to_string(i) + ".txt";
        int fileSize = distribution(generator);

        std::ofstream file(filename);
        for (int j = 0; j < fileSize; ++j) {
            file << "A";
        }
        file.close();
    }
}

int loadFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return 0;
    }

    int charCount = 0;
    while (file.get() != EOF) {
        charCount++;
    }
    file.close();

    return charCount;
}

int main() {
    const int numFiles = 5;
    const std::string filePrefix = "file";

    createFiles(numFiles, filePrefix);

    std::vector<std::future<int>> futures;

    for (int i = 0; i < numFiles; ++i) {
        std::string filename = filePrefix + std::to_string(i) + ".txt";
        futures.push_back(std::async(std::launch::async, loadFile, filename));
    }

    int totalSize = 0;

    for (int i = 0; i < numFiles; ++i) {
        int fileSize = futures[i].get();
        std::string filename = filePrefix + std::to_string(i) + ".txt";
        std::cout << "File " << filename << " has " << fileSize << " characters." << std::endl;
        totalSize += fileSize;
    }

    std::cout << "Total size of all files: " << totalSize << " characters." << std::endl;

    return 0;
}
