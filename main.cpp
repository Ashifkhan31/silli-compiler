#include "main.h"
#include "common.h"

#include "scanner.h"
#include "parser.h"
#include "compiler.h"

int main(int argc, char** args)
{
    if(argc != 2)
    {
        std::cout<<"Usage: compiler filepath \n";
        return 0;
    }

    std::string source;
    if(!readFile(args[1], source)) return 0;

    std::vector<Token> tokens;
    Scanner scanner(source, tokens);
    scanner.scan();
    scanner.printTokens();

    if (scanner.hadError) return 0;

    return 0;
}

void writeFile(std::string& code)
{
    std::fstream file;
    file.open("output.s", std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        std::cout<<"Unable to open output file.\n";
        return;
    }

    if (!file.write(code.data(), code.size()))
    {
        std::cout<<"Failed to write to output file.\n";
        return;
    }

    file.close();
}

bool readFile(const char* path, std::string& source)
{
    std::fstream file;
    file.open(path, std::ios::in);

    if (!file.is_open())
    {
        std::cout<<"Unable to open file "<<"'"<<path<<"'"<<".\n";
        return false;
    }

    std::string line;
    while(std::getline(file, line))
    {
        source += line + "\n";
    }

    if (file.fail() && !file.eof())
    {
        std::cout<<"An error occured while reading the file. \n";
        file.close();
        return false;
    }

    file.close();
    return true;
}
