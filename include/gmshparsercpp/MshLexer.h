#pragma once

#include <fstream>

namespace gmshparsercpp {

class MshLexer {
public:
    struct Token {
        enum EType : int { EndOfFile = 0, Number = 1, String, Section };

        ///
        EType type;
        ///
        std::string str;
        /// Line number
        int line_no;

        int
        as_int() const
        {
            if (this->type == Number)
                return std::stoi(this->str);
            else
                throw std::domain_error("Token is not a number");
        }

        double
        as_float() const
        {
            if (this->type == Number)
                return std::stod(this->str);
            else
                throw std::domain_error("Token is not a number");
        }

        std::string
        as_string() const
        {
            if (this->type == String)
                return this->str;
            else
                throw std::domain_error("Token is not a string");
        }
    };

    explicit MshLexer(std::ifstream * in);

    /// Look at the next token awaiting in the input stream
    Token peek();

    /// Read a token from the input stream
    Token read();

    /// Read binary blob from the input stream
    template <typename T>
    T
    read_blob()
    {
        T val;
        this->in->read((char *) &val, sizeof(T));
        return val;
    }

private:
    /// Read a token from an input stream
    Token read_token();
    /// Look at a character from an input stream without reading it
    int peek_char();
    /// Read a character from an input stream
    char read_char();

    /// Input stream
    std::ifstream * in;
    /// Flag indicating if we have a token cached
    bool have_token;
    /// Cached token
    Token curr;
};

} // namespace gmshparsercpp
