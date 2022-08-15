#pragma once

#include <string>
#include <fstream>
#include <deque>
#include <vector>

namespace gmshparsercpp {

/// Class for parsing MSH files
///
class MshFile {
protected:
    struct Token {
        enum EType : int { EndOfFile = 0, Number = 1, String, Section };

        ///
        EType type;
        ///
        std::string str;
        /// Line number
        int line_no;

        int
        as_int()
        {
            if (this->type == Number)
                return std::stod(this->str);
            else
                throw std::domain_error("Token is not an integer");
        }

        double
        as_float()
        {
            if (this->type == Number)
                return std::stod(this->str);
            else
                throw std::domain_error("Token is not a float");
        }

        const std::string &
        as_string()
        {
            if (this->type == String)
                return this->str;
            else
                throw std::domain_error("Token is not a string");
        }
    };

public:
    struct PhysicalName {
        /// Physical entity dimension
        int dimension;
        /// Entity tag
        int tag;
        /// Entity name
        std::string name;
    };

    /// Construct MSH file
    ///
    /// @param file_name The MSH file name
    explicit MshFile(const std::string & file_name);

    virtual ~MshFile();

    /// Get file format version
    ///
    /// @return File format version
    double get_version() const;

    /// Query if the file is ASCII
    ///
    /// @return True if the file format is in ASCII, False if it is binary
    bool is_ascii() const;

    /// Parse the file
    void parse();

    /// Close the file
    void close();

protected:
    void process_tokens();
    void process_optional_sections();
    void process_mesh_format_section();
    void process_physical_names_section();
    void process_entities_section();
    void process_nodes_section();
    void process_elements_section();
    std::vector<int> process_array_of_ints();
    void skip_section();
    const Token & peek();
    Token read();
    void read_end_section_marker(const std::string & section_name);
    int get_nodes_per_element(int element_type);

    /// File name
    std::string file_name;
    /// Input stream
    std::ifstream file;
    /// File format version
    double version;
    /// ASCII/binary flag
    bool binary;
    /// Physical names
    std::vector<PhysicalName> physical_names;
    /// Parsed tokens (valid only during process_XYZ)
    std::deque<Token> tokens;
};

} // namespace gmshparsercpp
