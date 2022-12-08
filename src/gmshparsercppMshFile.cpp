#include "gmshparsercppMshFile.h"
#include "gmshparsercppMshLexer.h"
#include "fmt/printf.h"
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

const std::vector<MshFile::PhysicalName> &
MshFile::get_physical_names() const
{
    return this->physical_names;
}

const std::vector<MshFile::PointEntity> &
MshFile::get_point_entities() const
{
    return this->point_entities;
}

const std::vector<MshFile::MultiDEntity> &
MshFile::get_curve_entities() const
{
    return this->curve_entities;
}

const std::vector<MshFile::MultiDEntity> &
MshFile::get_surface_entities() const
{
    return this->surface_entities;
}

const std::vector<MshFile::MultiDEntity> &
MshFile::get_volume_entities() const
{
    return this->volume_entities;
}

const std::vector<MshFile::Node> &
MshFile::get_nodes() const
{
    return this->nodes;
}

const std::vector<MshFile::ElementBlock> &
MshFile::get_element_blocks() const
{
    return this->element_blocks;
}

void
MshFile::parse()
{
    mshFlexLexer lexer(&this->file);
    int res;
    while ((res = lexer.yylex()) != 0) {
        auto type = static_cast<Token::EType>(res);
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
        if (next.str == "$MeshFormat") {
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
            if (next.str == "$PhysicalNames")
                process_physical_names_section();
            else if (next.str == "$Entities")
                process_entities_section();
            else if (next.str == "$PartitionedEntities")
                skip_section();
            else if (next.str == "$Nodes")
                process_nodes_section();
            else if (next.str == "$Elements")
                process_elements_section();
            else if (next.str == "$Periodic")
                skip_section();
            else if (next.str == "$GhostElements")
                skip_section();
            else if (next.str == "$Parametrizations")
                skip_section();
            else if (next.str == "$NodeData")
                skip_section();
            else if (next.str == "$ElementData")
                skip_section();
            else if (next.str == "$ElementNodeData")
                skip_section();
            else if (next.str == "$InterpolationScheme")
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
    if (!this->tokens.empty()) {
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
    if (sct_end.type != Token::Section || sct_end.str != section_name)
        throw std::runtime_error(fmt::sprintf("%s tag not found.", section_name));
}

void
MshFile::process_mesh_format_section()
{
    auto sct_start = read();

    this->version = read().as_float();
    this->binary = read().as_int() == 1;
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
        PointEntity pe;
        pe.tag = read().as_int();
        pe.x = read().as_float();
        pe.y = read().as_float();
        pe.z = read().as_float();
        pe.physical_tags = process_array_of_ints();
        this->point_entities.push_back(pe);
    }

    for (int i = 0; i < num_curves; i++) {
        MultiDEntity ent;
        ent.tag = read().as_int();
        ent.min_x = read().as_float();
        ent.min_y = read().as_float();
        ent.min_z = read().as_float();
        ent.max_x = read().as_float();
        ent.max_y = read().as_float();
        ent.max_z = read().as_float();
        ent.physical_tags = process_array_of_ints();
        ent.bounding_tags = process_array_of_ints();
        this->curve_entities.push_back(ent);
    }

    for (int i = 0; i < num_surfaces; i++) {
        MultiDEntity ent;
        ent.tag = read().as_int();
        ent.min_x = read().as_float();
        ent.min_y = read().as_float();
        ent.min_z = read().as_float();
        ent.max_x = read().as_float();
        ent.max_y = read().as_float();
        ent.max_z = read().as_float();
        ent.physical_tags = process_array_of_ints();
        ent.bounding_tags = process_array_of_ints();
        this->surface_entities.push_back(ent);
    }

    for (int i = 0; i < num_volumes; i++) {
        MultiDEntity ent;
        ent.tag = read().as_int();
        ent.min_x = read().as_float();
        ent.min_y = read().as_float();
        ent.min_z = read().as_float();
        ent.max_x = read().as_float();
        ent.max_y = read().as_float();
        ent.max_z = read().as_float();
        ent.physical_tags = process_array_of_ints();
        ent.bounding_tags = process_array_of_ints();
        this->volume_entities.push_back(ent);
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
        Node node;
        node.dimension = read().as_int();
        node.entity_tag = read().as_int();
        node.parametric = read().as_int() == 1;
        // NOTE: it is not 100% clear from the doco how to read this section
        node.tags = process_array_of_ints();
        for (std::size_t i = 0; i < node.tags.size(); i++) {
            Point pt;
            pt.x = read().as_float();
            pt.y = read().as_float();
            pt.z = read().as_float();
            node.coordinates.push_back(pt);
            if (node.parametric) {
                Point par_pr;
                if (node.dimension >= 1)
                    par_pr.x = read().as_float();
                if (node.dimension >= 2)
                    par_pr.y = read().as_float();
                if (node.dimension == 3)
                    par_pr.z = read().as_float();
                node.par_coords.push_back(par_pr);
            }
        }
        this->nodes.push_back(node);
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
        ElementBlock blk;
        blk.dimension = read().as_int();
        blk.tag = read().as_int();
        blk.element_type = read().as_int();
        auto num_nodes_per_element = get_nodes_per_element(blk.element_type);
        auto num_elements_in_block = read().as_int();
        for (int j = 0; j < num_elements_in_block; j++) {
            Element el;
            el.tag = read().as_int();
            for (int k = 0; k < num_nodes_per_element; k++) {
                auto tag = read().as_int();
                el.node_tags.push_back(tag);
            }
            blk.elements.push_back(el);
        }
        this->element_blocks.push_back(blk);
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
