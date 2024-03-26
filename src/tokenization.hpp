#pragma once
#include<vector>
#include<string>

enum class TokenType{
    _exit,
    int_lit,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

//tokenizer class function with peek and consume functions
class Tokenizer{
public:
    inline Tokenizer(const std::string src): m_src(std::move(src)){
     }

    inline std::vector<Token> tokenize(std::string str){
        std::vector<Token> tokens;
        std::string buffer;

        while(peek().has_value()){
            char c = peek().value();
            if(std::isalpha(c)){
                buffer.push_back(c);
            }
        }
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

                if(buffer == "return"){
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


private:

    [[nodiscard]]std::optional<char> peek(int count=1) const{
        if(m_index + count >= m_src.length()){
            return {};
        }else{
            return m_src.at(m_index);
        }
    }

    char consume(){
        return m_src.at(m_index++);
    }
    const std::string m_src;
    int m_index;
};


/*

loop implementation


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
*/
