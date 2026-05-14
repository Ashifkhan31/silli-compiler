#include "main.h"
#include "common.h"

#include "scanner.h"
#include "parser.h"

int main(int argc, char** args)
{
    if(argc != 2)
    {
        std::cout<<"Usage: compiler filepath \n";
        return 0;
    }

    std::string source;
    if(!readfile(args[1], source)) return 0;

    std::vector<Token> tokens;
    Scanner scanner(source, tokens);
    scanner.scan();

    if (scanner.hadError) return 0;

    Parser parser = Parser(tokens);
    ASTnode* astRoot = parser.parse();

    if (parser.hadError) return 0;

    //compile

    return 0;
}

bool readfile(const char* path, std::string& source)
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
