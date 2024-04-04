#include "gmshparsercpp/MshFile.h"
#include "fmt/printf.h"
#include <system_error>

namespace gmshparsercpp {

MshFile::MshFile(const std::string & file_name) :
    file_name(file_name),
    file(this->file_name),
    lexer(&this->file),
    version(0.),
    binary(false),
    endianness(0)
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
    MshLexer::Token token = this->lexer.peek();
    do {
        if (token.type == MshLexer::Token::Section) {
            token = this->lexer.read();
            process_section(token);
        }
        else
            throw std::runtime_error("Expected start of section marker not found.");
        token = this->lexer.peek();
    } while (token.type != MshLexer::Token::EndOfFile);
}

void
MshFile::process_section(const MshLexer::Token & token)
{
    if (token.str == "$MeshFormat")
        process_mesh_format_section();
    else if (token.str == "$PhysicalNames")
        process_physical_names_section();
    else if (token.str == "$Entities")
        process_entities_section();
    else if (token.str == "$PartitionedEntities")
        skip_section();
    else if (token.str == "$Nodes")
        process_nodes_section();
    else if (token.str == "$Elements")
        process_elements_section();
    else if (token.str == "$Periodic")
        skip_section();
    else if (token.str == "$GhostElements")
        skip_section();
    else if (token.str == "$Parametrizations")
        skip_section();
    else if (token.str == "$NodeData")
        skip_section();
    else if (token.str == "$ElementData")
        skip_section();
    else if (token.str == "$ElementNodeData")
        skip_section();
    else if (token.str == "$InterpolationScheme")
        skip_section();
    else
        skip_section();
}

void
MshFile::process_tokens()
{
    const auto & next = this->lexer.peek();
    if (next.type == MshLexer::Token::Section) {
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
}

void
MshFile::read_end_section_marker(const std::string & section_name)
{
    auto sct_end = this->lexer.read();
    if (sct_end.type != MshLexer::Token::Section || sct_end.str != section_name)
        throw std::runtime_error(fmt::sprintf("%s tag not found.", section_name));
}

void
MshFile::process_mesh_format_section()
{
    this->version = this->lexer.read().as_float();
    this->binary = this->lexer.read().as_int() == 1;
    auto data_size = this->lexer.read();
    if (this->binary)
        this->endianness = this->lexer.read_blob<int>();
    read_end_section_marker("$EndMeshFormat");
}

void
MshFile::process_physical_names_section()
{
    auto num_entities = this->lexer.read().as_int();
    for (int i = 0; i < num_entities; i++) {
        auto dimension = this->lexer.read().as_int();
        auto tag = this->lexer.read().as_int();
        auto name = this->lexer.read().as_string();
        PhysicalName pn = { dimension, tag, name };
        this->physical_names.push_back(pn);
    }

    read_end_section_marker("$EndPhysicalNames");
}

void
MshFile::process_entities_section()
{
    auto num_points = this->lexer.read().as_int();
    auto num_curves = this->lexer.read().as_int();
    auto num_surfaces = this->lexer.read().as_int();
    auto num_volumes = this->lexer.read().as_int();

    for (int i = 0; i < num_points; i++) {
        PointEntity pe;
        pe.tag = this->lexer.read().as_int();
        pe.x = this->lexer.read().as_float();
        pe.y = this->lexer.read().as_float();
        pe.z = this->lexer.read().as_float();
        pe.physical_tags = process_array_of_ints();
        this->point_entities.push_back(pe);
    }

    for (int i = 0; i < num_curves; i++) {
        MultiDEntity ent;
        ent.tag = this->lexer.read().as_int();
        ent.min_x = this->lexer.read().as_float();
        ent.min_y = this->lexer.read().as_float();
        ent.min_z = this->lexer.read().as_float();
        ent.max_x = this->lexer.read().as_float();
        ent.max_y = this->lexer.read().as_float();
        ent.max_z = this->lexer.read().as_float();
        ent.physical_tags = process_array_of_ints();
        ent.bounding_tags = process_array_of_ints();
        this->curve_entities.push_back(ent);
    }

    for (int i = 0; i < num_surfaces; i++) {
        MultiDEntity ent;
        ent.tag = this->lexer.read().as_int();
        ent.min_x = this->lexer.read().as_float();
        ent.min_y = this->lexer.read().as_float();
        ent.min_z = this->lexer.read().as_float();
        ent.max_x = this->lexer.read().as_float();
        ent.max_y = this->lexer.read().as_float();
        ent.max_z = this->lexer.read().as_float();
        ent.physical_tags = process_array_of_ints();
        ent.bounding_tags = process_array_of_ints();
        this->surface_entities.push_back(ent);
    }

    for (int i = 0; i < num_volumes; i++) {
        MultiDEntity ent;
        ent.tag = this->lexer.read().as_int();
        ent.min_x = this->lexer.read().as_float();
        ent.min_y = this->lexer.read().as_float();
        ent.min_z = this->lexer.read().as_float();
        ent.max_x = this->lexer.read().as_float();
        ent.max_y = this->lexer.read().as_float();
        ent.max_z = this->lexer.read().as_float();
        ent.physical_tags = process_array_of_ints();
        ent.bounding_tags = process_array_of_ints();
        this->volume_entities.push_back(ent);
    }

    read_end_section_marker("$EndEntities");
}

void
MshFile::process_nodes_section()
{
    auto num_entity_blocks = this->lexer.read().as_int();
    auto num_nodes = this->lexer.read().as_int();
    auto min_node_tag = this->lexer.read().as_int();
    auto max_node_tag = this->lexer.read().as_int();

    for (int i = 0; i < num_entity_blocks; i++) {
        Node node;
        node.dimension = this->lexer.read().as_int();
        node.entity_tag = this->lexer.read().as_int();
        node.parametric = this->lexer.read().as_int() == 1;
        // NOTE: it is not 100% clear from the doco how to read this section
        node.tags = process_array_of_ints();
        for (std::size_t i = 0; i < node.tags.size(); i++) {
            Point pt;
            pt.x = this->lexer.read().as_float();
            pt.y = this->lexer.read().as_float();
            pt.z = this->lexer.read().as_float();
            node.coordinates.push_back(pt);
            if (node.parametric) {
                Point par_pr;
                if (node.dimension >= 1)
                    par_pr.x = this->lexer.read().as_float();
                if (node.dimension >= 2)
                    par_pr.y = this->lexer.read().as_float();
                if (node.dimension == 3)
                    par_pr.z = this->lexer.read().as_float();
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
    auto num_entity_blocks = this->lexer.read().as_int();
    auto num_elements = this->lexer.read().as_int();
    auto min_node_tag = this->lexer.read().as_int();
    auto max_node_tag = this->lexer.read().as_int();

    for (int i = 0; i < num_entity_blocks; i++) {
        ElementBlock blk;
        blk.dimension = this->lexer.read().as_int();
        blk.tag = this->lexer.read().as_int();
        blk.element_type = static_cast<ElementType>(this->lexer.read().as_int());
        auto num_nodes_per_element = get_nodes_per_element(blk.element_type);
        auto num_elements_in_block = this->lexer.read().as_int();
        for (int j = 0; j < num_elements_in_block; j++) {
            Element el;
            el.tag = this->lexer.read().as_int();
            for (int k = 0; k < num_nodes_per_element; k++) {
                auto tag = this->lexer.read().as_int();
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
    auto n = this->lexer.read().as_int();
    for (int i = 0; i < n; i++) {
        auto num = this->lexer.read().as_int();
        array.push_back(num);
    }
    return array;
}

void
MshFile::skip_section()
{
    MshLexer::Token token;
    do {
        token = this->lexer.read();
    } while (token.type != MshLexer::Token::Section);
}

int
MshFile::get_nodes_per_element(ElementType element_type)
{
    // clang-format off
    switch (element_type) {
        case LINE2: return 2;
        case TRI3: return 3;
        case QUAD4: return 4;
        case TET4: return 4;
        case HEX8: return 8;
        case PRISM6: return 6;
        case PYRAMID5: return 5;
        case LINE3: return 3;
        case TRI6: return 6;
        case QUAD9: return 9;
        case TET10: return 10;
        case HEX27: return 27;
        case PRISM18: return 18;
        case PYRAMID14: return 14;
        case POINT: return 1;
        case QUAD8: return 8;
        case HEX20: return 20;
        case PRISM15: return 15;
        case PYRAMID13: return 13;
        case ITRI9: return 9;
        case TRI10: return 10;
        case ITRI12: return 12;
        case TRI15: return 15;
        case ITRI15: return 15;
        case TRI21: return 21;
        case LINE4: return 4;
        case LINE5: return 5;
        case LINE6: return 6;
        case TET20: return 20;
        case TET35: return 35;
        case TET56: return 56;
        case HEX64: return 64;
        case HEX125: return 125;
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
