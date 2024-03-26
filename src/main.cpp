#include<iostream>
#include <fstream>
#include<optional>
#include<sstream>
#include<vector>
#include "tokenization.hpp"

#define DEBUG_TRACE_EXECUTION


std::vector<Token> tokenize(std::string str){
    std::vector<Token> tokens;
    std::string buffer;
    for(int i=0; i < str.length(); i++){
            char c = str.at(i);
            if(std::isalpha(c)){
                buffer.push_back(c);
                i++;
                while(std::isalnum(str.at(i))){
                    buffer.push_back(str.at(i));
                    i++;
                }
                i--;

                if(buffer == "exit"){
                    tokens.push_back({.type = TokenType::_return});
                    buffer.clear();
                    continue;
                }else{
                    std::cerr<<"SyntaxError: Cannot recognize keyword" << std::endl;
                    buffer.clear();
                    exit(EXIT_FAILURE);
                }
            }

            else if(std::isspace(c)){
                continue;
            }
            else if(std::isdigit(c)){
                buffer.push_back(c);
                i++;
                while(std::isdigit(str.at(i))){
                    buffer.push_back(str.at(i));
                    i++;
                }
                i--;

                tokens.push_back({.type = TokenType::int_lit, .value = buffer});
                buffer.clear();
            }
            else if(c == ';'){
                tokens.push_back({.type = TokenType::semi});
            }
            else{
                std::cerr << "SyntaxError: no code found" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        return tokens;

}






std::string tokens_to_asm(std::vector<Token>& tokens){
    std::stringstream output;
    output << "global _start\n_start:\n";
    for(int i=0; i<tokens.size();i++){
        const Token& token = tokens.at(i);
        if(token.type == TokenType::_return){
            if((i+1 < tokens.size()) && (tokens.at(i+1).type == TokenType::int_lit)){
                if((i+2 < tokens.size()) && (tokens.at(i+2).type == TokenType::semi)){
                    output << "    mov rax, 60\n";
                    output << "    mov rdi, " << tokens.at(i+1).value.value() << "\n";
                    output << "    syscall";
                }else{
                    std::cerr << "SyntaxError: ; missing" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }else{
                    std::cerr << "SyntaxError: Please provide a return value" << std::endl;
                    exit(EXIT_FAILURE);
                }
        }
    }

    return output.str();
}




int main(int argc, char** argv){
    if(argc != 2){
        std::cerr << "Invalid arguments" << std::endl;
        std::cerr << "Pass in only the file after executable eg: .exe file.hy" << std::endl;
        return EXIT_FAILURE;
    }

    std::string contents;
    std::stringstream contents_stream;
    {
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();        
        contents = contents_stream.str();
    }
    std::vector<Token> tokens = tokenize(contents);
    {
        std::fstream file("../out.asm", std::ios::out);
        file << tokens_to_asm(tokens);
        std::cout << "Assembly file created" << std::endl;
    }

    system("nasm -felf64 ../out.asm");
    system("ld -o out ../out.o");
    return EXIT_SUCCESS;
}