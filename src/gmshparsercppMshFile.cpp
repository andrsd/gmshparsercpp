#include "gmshparsercppMshFile.h"
#include "gmshparsercppMshLexer.h"
#include "fmt/printf.h"
#include <iostream>
#include <system_error>

int
mshFlexLexer::yywrap()
{
    return 1;
}

namespace gmshparsercpp {

MshFile::MshFile(const std::string & file_name) :
    file_name(file_name),
    file(this->file_name),
    version(0.),
    binary(false)
{
    if (!this->file.is_open())
        throw std::system_error(std::error_code(),
                                fmt::sprintf("Unable to open file '%s'.", this->file_name));
}

MshFile::~MshFile()
{
    close();
}

double
MshFile::get_version() const
{
    return this->version;
}

bool
MshFile::is_ascii() const
{
    return !this->binary;
}

void
MshFile::parse()
{
    mshFlexLexer lexer(&this->file);
    int res;
    while ((res = lexer.yylex()) != 0) {
        Token::EType type = static_cast<Token::EType>(res);
        Token token = { type, std::string(lexer.YYText(), lexer.YYLeng()), lexer.lineno() };
        this->tokens.push_back(token);
    }
    this->tokens.push_back({ Token::EndOfFile });

    process_tokens();
}

void
MshFile::process_tokens()
{
    const auto & next = peek();
    if (next.type == Token::Section) {
        if (next.str.compare("$MeshFormat") == 0) {
            process_mesh_format_section();
            process_optional_sections();
        }
        else
            throw std::runtime_error("Expected $MeshFormat section marker not found.");
    }
    else
        throw std::runtime_error("Expected start of section marker not found.");
}

void
MshFile::process_optional_sections()
{
    Token next = peek();
    while (next.type != Token::EndOfFile) {
        if (next.type == Token::Section) {
            if (next.str.compare("$PhysicalNames") == 0)
                process_physical_names_section();
            else if (next.str.compare("$Entities") == 0)
                process_entities_section();
            else if (next.str.compare("$PartitionedEntities") == 0)
                skip_section();
            else if (next.str.compare("$Nodes") == 0)
                skip_section();
            else if (next.str.compare("$Elements") == 0)
                skip_section();
            else if (next.str.compare("$Periodic") == 0)
                skip_section();
            else if (next.str.compare("$GhostElements") == 0)
                skip_section();
            else if (next.str.compare("$Parametrizations") == 0)
                skip_section();
            else if (next.str.compare("$NodeData") == 0)
                skip_section();
            else if (next.str.compare("$ElementData") == 0)
                skip_section();
            else if (next.str.compare("$ElementNodeData") == 0)
                skip_section();
            else if (next.str.compare("$InterpolationScheme") == 0)
                skip_section();
            else
                skip_section();
            next = peek();
        }
        else
            throw std::runtime_error("Expected section marker not found.");
    }
}

const MshFile::Token &
MshFile::peek()
{
    return this->tokens.front();
}

MshFile::Token
MshFile::read()
{
    if (this->tokens.size() > 0) {
        auto token = this->tokens.front();
        this->tokens.pop_front();
        return token;
    }
    else
        throw std::runtime_error("Unexpected end of file.");
}

void
MshFile::read_end_section_marker(const std::string & section_name)
{
    auto sct_end = read();
    if (sct_end.type != Token::Section || sct_end.str.compare(section_name) != 0)
        throw std::runtime_error(fmt::sprintf("%s tag not found.", section_name));
}

void
MshFile::process_mesh_format_section()
{
    auto sct_start = read();

    auto version = read().as_float();
    this->version = version;
    auto file_type = read().as_int();
    this->binary = file_type == 1;
    auto data_size = read();

    read_end_section_marker("$EndMeshFormat");
}

void
MshFile::process_physical_names_section()
{
    auto sct_start = read();

    auto num_entities = read().as_int();
    for (int i = 0; i < num_entities; i++) {
        auto dimension = read().as_int();
        auto tag = read().as_int();
        auto name = read().as_string();
        PhysicalName pn = { dimension, tag, name };
        this->physical_names.push_back(pn);
    }

    read_end_section_marker("$EndPhysicalNames");
}

void
MshFile::process_entities_section()
{
    auto sct_start = read();

    auto num_points = read().as_int();
    auto num_curves = read().as_int();
    auto num_surfaces = read().as_int();
    auto num_volumes = read().as_int();

    for (int i = 0; i < num_points; i++) {
        auto tag = read().as_int();
        auto x = read().as_float();
        auto y = read().as_float();
        auto z = read().as_float();
        auto phys_tags = process_array_of_ints();
    }

    for (int i = 0; i < num_curves; i++) {
        auto tag = read().as_int();
        auto min_x = read().as_float();
        auto min_y = read().as_float();
        auto min_z = read().as_float();
        auto max_x = read().as_float();
        auto max_y = read().as_float();
        auto max_z = read().as_float();
        auto phys_tags = process_array_of_ints();
        auto bounding_points = process_array_of_ints();
    }

    for (int i = 0; i < num_surfaces; i++) {
        auto tag = read().as_int();
        auto min_x = read().as_float();
        auto min_y = read().as_float();
        auto min_z = read().as_float();
        auto max_x = read().as_float();
        auto max_y = read().as_float();
        auto max_z = read().as_float();
        auto phys_tags = process_array_of_ints();
        auto bounding_curves = process_array_of_ints();
    }

    for (int i = 0; i < num_volumes; i++) {
        auto tag = read().as_int();
        auto min_x = read().as_float();
        auto min_y = read().as_float();
        auto min_z = read().as_float();
        auto max_x = read().as_float();
        auto max_y = read().as_float();
        auto max_z = read().as_float();
        auto phys_tags = process_array_of_ints();
        auto bounding_surfaces = process_array_of_ints();
    }

    read_end_section_marker("$EndEntities");
}

void
MshFile::process_nodes_section()
{
    auto sct_start = read();

    auto num_entity_blocks = read().as_int();
    auto num_nodes = read().as_int();
    auto min_node_tag = read().as_int();
    auto max_node_tag = read().as_int();

    for (int i = 0; i < num_entity_blocks; i++) {
        auto dimension = read().as_int();
        auto tag = read().as_int();
        bool parametric = read().as_int() == 1;
        auto node_tags = process_array_of_ints();
        auto x = read().as_float();
        auto y = read().as_float();
        auto z = read().as_float();
        if (parametric) {
            double u, v, w;
            if (dimension >= 1)
                u = read().as_float();
            if (dimension >= 2)
                v = read().as_float();
            if (dimension == 3)
                w = read().as_float();
        }
    }

    read_end_section_marker("$EndNodes");
}

void
MshFile::process_elements_section()
{
    auto sct_start = read();

    auto num_entity_blocks = read().as_int();
    auto num_elements = read().as_int();
    auto min_node_tag = read().as_int();
    auto max_node_tag = read().as_int();

    for (int i = 0; i < num_entity_blocks; i++) {
        auto dimension = read().as_int();
        auto tag = read().as_int();
        auto element_type = read().as_int();
        auto num_elements_in_block = read().as_int();
        for (int j = 0; j < num_elements_in_block; j++) {
            auto element_tag = read().as_int();
            auto num_nodes_per_element = get_nodes_per_element(element_type);
            for (int k = 0; k < num_nodes_per_element; k++) {
                auto node_tag = read().as_int();
            }
        }
    }

    read_end_section_marker("$EndElements");
}

std::vector<int>
MshFile::process_array_of_ints()
{
    std::vector<int> array;
    auto n = read().as_int();
    for (int i = 0; i < n; i++) {
        auto num = read().as_int();
        array.push_back(num);
    }
    return array;
}

void
MshFile::skip_section()
{
    auto token = read();

    do {
        token = read();
    } while (token.type != Token::Section);
}

int
MshFile::get_nodes_per_element(int element_type)
{
    // clang-format off
    switch (element_type) {
        case 1: return 2;
        case 2: return 3;
        case 3: return 4;
        case 4: return 4;
        case 5: return 8;
        case 6: return 6;
        case 7: return 5;
        case 8: return 3;
        case 9: return 6;
        case 10: return 9;
        case 11: return 10;
        case 12: return 27;
        case 13: return 18;
        case 14: return 14;
        case 15: return 1;
        case 16: return 8;
        case 17: return 20;
        case 18: return 15;
        case 19: return 13;
        case 20: return 9;
        case 21: return 10;
        case 22: return 12;
        case 23: return 15;
        case 24: return 15;
        case 25: return 21;
        case 26: return 4;
        case 27: return 5;
        case 28: return 6;
        case 29: return 20;
        case 30: return 35;
        case 31: return 56;
        case 92: return 64;
        case 93: return 125;
        default:
            throw std::domain_error(fmt::sprintf("Unknown element type '%d'", element_type));
    }
    // clang-format on
}

void
MshFile::close()
{
    if (this->file.is_open())
        this->file.close();
}

} // namespace gmshparsercpp
