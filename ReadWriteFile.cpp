#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

void saveToFile(const std::string &file, const std::string &content)
{
    std::ofstream ofs(file, std::ofstream::trunc | std::ofstream::binary);
    ofs << content;
    ofs.close();
    ofs.open(file, std::ofstream::app);
    ofs << " msj";
    ofs.close();
}
std::string readFile(const std::string &file)
{
    std::stringstream ret;
    std::ifstream ifs(file, std::ifstream::binary);
    ret << ifs.rdbuf();
    return ret.str();
}

int main()
{
    std::string file{"hello.txt"};
    saveToFile(file, "hello");
    std::cout << readFile(file) << std::endl;
}
