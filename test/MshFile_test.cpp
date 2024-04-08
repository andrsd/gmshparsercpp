#include <gmock/gmock.h>
#include "TestConfig.h"
#include "ExceptionTestMacros.h"
#include "gmshparsercpp/MshFile.h"

using namespace gmshparsercpp;
using namespace testing;

TEST(MshFileTest, empty)
{
    EXPECT_THROW_MSG(
        {
            std::string file_name =
                std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/empty.msh");
            MshFile f(file_name);
            f.parse();
        },
        "Expected start of section marker not found.");
}

TEST(MshFileTest, header)
{
    std::string file_name = std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/header-only.msh");
    MshFile f(file_name);
    EXPECT_NO_THROW({ f.parse(); });
    EXPECT_EQ(f.get_version(), 4.1);
    EXPECT_TRUE(f.is_ascii());
}

TEST(MshFileTest, non_existent_file)
{
    std::string file_name =
        std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/non-existent-file.msh");
    EXPECT_THAT_THROW_MSG({ MshFile f(file_name); },
                          MatchesRegex("Unable to open file '.+/non-existent-file.msh'."));
}
TEST(MshFileTest, missing_end_section_marker)
{
    std::string file_name =
        std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/missing-end-section-marker.msh");
    EXPECT_THROW_MSG(
        {
            MshFile f(file_name);
            f.parse();
        },
        "$EndMeshFormat tag not found.");
}

TEST(MshFileTest, unsupported_version)
{
    std::string file_name =
        std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/unsupported-version.msh");
    EXPECT_THROW_MSG(
        {
            MshFile f(file_name);
            f.parse();
        },
        "Unsupported version 1");
}

TEST(MshFileTest, unsupported_data_size_v2)
{
    std::string file_name =
        std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/unsupported-data-size-v2.msh");
    EXPECT_THROW_MSG(
        {
            MshFile f(file_name);
            f.parse();
        },
        "Unexpected data size found: 4");
}

TEST(MshFileTest, unsupported_data_size_v4)
{
    std::string file_name =
        std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/unsupported-data-size-v4.msh");
    EXPECT_THROW_MSG(
        {
            MshFile f(file_name);
            f.parse();
        },
        "Unexpected data size found: 4");
}

TEST(MshFileTest, quad_v4_asc)
{
    std::string file_name = std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/quad-v4.asc.msh");
    MshFile f(file_name);
    EXPECT_NO_THROW({ f.parse(); });
    EXPECT_EQ(f.get_version(), 4.1);
    EXPECT_TRUE(f.is_ascii());

    std::vector<MshFile::PhysicalName> test_phys_names = { MshFile::PhysicalName(1, 5, "bottom"),
                                                           MshFile::PhysicalName(1, 6, "left"),
                                                           MshFile::PhysicalName(1, 7, "top"),
                                                           MshFile::PhysicalName(1, 8, "right") };
    auto phys_names = f.get_physical_names();
    EXPECT_EQ(phys_names.size(), 4);
    for (std::size_t i = 0; i < phys_names.size(); i++) {
        EXPECT_EQ(phys_names[i].dimension, test_phys_names[i].dimension);
        EXPECT_EQ(phys_names[i].tag, test_phys_names[i].tag);
        EXPECT_STREQ(phys_names[i].name.c_str(), test_phys_names[i].name.c_str());
    }

    auto nodes = f.get_nodes();
    EXPECT_EQ(nodes.size(), 9);

    auto el_blks = f.get_element_blocks();
    EXPECT_EQ(el_blks.size(), 9);
}

TEST(MshFileTest, quad_v4_bin)
{
    std::string file_name = std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/quad-v4.bin.msh");
    MshFile f(file_name);
    EXPECT_NO_THROW({ f.parse(); });
    EXPECT_EQ(f.get_version(), 4.1);
    EXPECT_FALSE(f.is_ascii());

    std::vector<MshFile::PhysicalName> test_phys_names = { MshFile::PhysicalName(1, 5, "bottom"),
                                                           MshFile::PhysicalName(1, 6, "left"),
                                                           MshFile::PhysicalName(1, 7, "top"),
                                                           MshFile::PhysicalName(1, 8, "right") };
    auto phys_names = f.get_physical_names();
    EXPECT_EQ(phys_names.size(), 4);
    for (std::size_t i = 0; i < phys_names.size(); i++) {
        EXPECT_EQ(phys_names[i].dimension, test_phys_names[i].dimension);
        EXPECT_EQ(phys_names[i].tag, test_phys_names[i].tag);
        EXPECT_STREQ(phys_names[i].name.c_str(), test_phys_names[i].name.c_str());
    }

    auto nodes = f.get_nodes();
    EXPECT_EQ(nodes.size(), 8);

    auto el_blks = f.get_element_blocks();
    EXPECT_EQ(el_blks.size(), 4);
}

TEST(MshFileTest, quad_v2_asc)
{
    std::string file_name = std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/quad-v2.asc.msh");
    MshFile f(file_name);
    EXPECT_NO_THROW({ f.parse(); });
    EXPECT_EQ(f.get_version(), 2.2);
    EXPECT_TRUE(f.is_ascii());

    auto nodes = f.get_nodes();
    EXPECT_EQ(nodes.size(), 4);

    auto el_blks = f.get_element_blocks();
    EXPECT_EQ(el_blks.size(), 4);
}

TEST(MshFileTest, quad_v2_bin)
{
    std::string file_name = std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/quad-v2.bin.msh");
    MshFile f(file_name);
    EXPECT_NO_THROW({ f.parse(); });
    EXPECT_EQ(f.get_version(), 2.2);
    EXPECT_FALSE(f.is_ascii());

    auto nodes = f.get_nodes();
    EXPECT_EQ(nodes.size(), 4);

    auto el_blks = f.get_element_blocks();
    EXPECT_EQ(el_blks.size(), 4);
}

TEST(MshFileTest, two_blk)
{
    std::string file_name = std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/2blk.msh");
    MshFile f(file_name);
    EXPECT_NO_THROW({ f.parse(); });
    EXPECT_EQ(f.get_version(), 4.1);
    EXPECT_TRUE(f.is_ascii());

    std::vector<MshFile::PhysicalName> test_phys_names = {
        MshFile::PhysicalName(1, 200, "left"),  MshFile::PhysicalName(1, 201, "top"),
        MshFile::PhysicalName(1, 202, "right"), MshFile::PhysicalName(1, 203, "bottom"),
        MshFile::PhysicalName(2, 100, "blk1"),  MshFile::PhysicalName(2, 101, "blk2")
    };
    auto phys_names = f.get_physical_names();
    EXPECT_EQ(phys_names.size(), 6);
    for (std::size_t i = 0; i < phys_names.size(); i++) {
        EXPECT_EQ(phys_names[i].dimension, test_phys_names[i].dimension);
        EXPECT_EQ(phys_names[i].tag, test_phys_names[i].tag);
        EXPECT_STREQ(phys_names[i].name.c_str(), test_phys_names[i].name.c_str());
    }

    std::vector<MshFile::PointEntity> test_pt_ents = {
        MshFile::PointEntity(1, 0, 0, 0, {}),     MshFile::PointEntity(2, 0.5, 0, 0, {}),
        MshFile::PointEntity(3, 1, 0, 0, {}),     MshFile::PointEntity(4, 0, 0.5, 0, {}),
        MshFile::PointEntity(5, 0.5, 0.5, 0, {}), MshFile::PointEntity(6, 0, 1, 0, {}),
        MshFile::PointEntity(7, 1, 1, 0, {}),
    };
    auto point_entities = f.get_point_entities();
    EXPECT_EQ(point_entities.size(), 7);
    for (std::size_t i = 0; i < point_entities.size(); i++) {
        EXPECT_EQ(point_entities[i].tag, test_pt_ents[i].tag);
        EXPECT_EQ(point_entities[i].x, test_pt_ents[i].x);
        EXPECT_EQ(point_entities[i].y, test_pt_ents[i].y);
        EXPECT_EQ(point_entities[i].z, test_pt_ents[i].z);
        EXPECT_THAT(point_entities[i].physical_tags,
                    ElementsAreArray(test_pt_ents[i].physical_tags));
    }

    std::vector<MshFile::MultiDEntity> test_crv_ents = {
        MshFile::MultiDEntity(1, 0, 0, 0, 0.5, 0, 0, { 203 }, { 1, -2 }),
        MshFile::MultiDEntity(2, 0.5, 0, 0, 0.5, 0.5, 0, {}, { 2, -5 }),
        MshFile::MultiDEntity(3, 0, 0.5, 0, 0.5, 0.5, 0, {}, { 5, -4 }),
        MshFile::MultiDEntity(4, 0, 0, 0, 0, 0.5, 0, { 200 }, { 4, -1 }),
        MshFile::MultiDEntity(5, 0.5, 0, 0, 1, 0, 0, { 203 }, { 2, -3 }),
        MshFile::MultiDEntity(6, 1, 0, 0, 1, 1, 0, { 202 }, { 3, -7 }),
        MshFile::MultiDEntity(7, 0, 1, 0, 1, 1, 0, { 201 }, { 7, -6 }),
        MshFile::MultiDEntity(8, 0, 0.5, 0, 0, 1, 0, { 200 }, { 6, -4 })
    };
    auto curve_entities = f.get_curve_entities();
    EXPECT_EQ(curve_entities.size(), 8);
    for (std::size_t i = 0; i < curve_entities.size(); i++) {
        EXPECT_EQ(curve_entities[i].tag, test_crv_ents[i].tag);
        EXPECT_EQ(curve_entities[i].min_x, test_crv_ents[i].min_x);
        EXPECT_EQ(curve_entities[i].min_y, test_crv_ents[i].min_y);
        EXPECT_EQ(curve_entities[i].min_z, test_crv_ents[i].min_z);
        EXPECT_THAT(curve_entities[i].physical_tags,
                    ElementsAreArray(test_crv_ents[i].physical_tags));
        EXPECT_THAT(curve_entities[i].bounding_tags,
                    ElementsAreArray(test_crv_ents[i].bounding_tags));
    }

    std::vector<MshFile::MultiDEntity> test_surf_ents = {
        MshFile::MultiDEntity(1, 0, 0, 0, 0.5, 0.5, 0, { 100 }, { 1, 2, 3, 4 }),
        MshFile::MultiDEntity(2, 0, 0, 0, 1, 1, 0, { 101 }, { 5, 6, 7, 8, -3, -2 })
    };
    auto surface_entities = f.get_surface_entities();
    EXPECT_EQ(surface_entities.size(), 2);
    for (std::size_t i = 0; i < surface_entities.size(); i++) {
        EXPECT_EQ(surface_entities[i].tag, test_surf_ents[i].tag);
        EXPECT_EQ(surface_entities[i].min_x, test_surf_ents[i].min_x);
        EXPECT_EQ(surface_entities[i].min_y, test_surf_ents[i].min_y);
        EXPECT_EQ(surface_entities[i].min_z, test_surf_ents[i].min_z);
        EXPECT_THAT(surface_entities[i].physical_tags,
                    ElementsAreArray(test_surf_ents[i].physical_tags));
        EXPECT_THAT(surface_entities[i].bounding_tags,
                    ElementsAreArray(test_surf_ents[i].bounding_tags));
    }

    auto volume_entities = f.get_volume_entities();
    EXPECT_EQ(volume_entities.size(), 0);

    auto nodes = f.get_nodes();
    EXPECT_EQ(nodes.size(), 15);

    auto el_blks = f.get_element_blocks();
    EXPECT_EQ(el_blks.size(), 8);
}

TEST(MshFileTest, element_dim)
{
    std::vector<gmshparsercpp::ElementType> d0 = { POINT };
    for (auto & d : d0)
        EXPECT_EQ(MshFile::get_element_dimension(d), 0);

    std::vector<gmshparsercpp::ElementType> d1 = { LINE2, LINE3, LINE4, LINE5, LINE6 };
    for (auto & d : d1)
        EXPECT_EQ(MshFile::get_element_dimension(d), 1);

    std::vector<gmshparsercpp::ElementType> d2 = { TRI3,  QUAD4,  TRI6,  QUAD9,  QUAD8, ITRI9,
                                                   TRI10, ITRI12, TRI15, ITRI15, TRI21 };
    for (auto & d : d2)
        EXPECT_EQ(MshFile::get_element_dimension(d), 2);

    std::vector<gmshparsercpp::ElementType> d3 = { TET4,  HEX8,    PRISM6,    PYRAMID5,
                                                   TET10, HEX27,   PRISM18,   PYRAMID14,
                                                   HEX20, PRISM15, PYRAMID13, TET20,
                                                   TET35, TET56,   HEX64,     HEX125 };
    for (auto & d : d3)
        EXPECT_EQ(MshFile::get_element_dimension(d), 3);

    EXPECT_THROW_MSG(MshFile::get_element_dimension(NONE), "Unknown element type '-1'");
}

TEST(MshFileTest, num_element_nodes)
{
    std::vector<std::pair<ElementType, int>> nodes = {
        { LINE2, 2 },  { TRI3, 3 },     { QUAD4, 4 },    { TET4, 4 },       { HEX8, 8 },
        { PRISM6, 6 }, { PYRAMID5, 5 }, { LINE3, 3 },    { TRI6, 6 },       { QUAD9, 9 },
        { TET10, 10 }, { HEX27, 27 },   { PRISM18, 18 }, { PYRAMID14, 14 }, { POINT, 1 },
        { QUAD8, 8 },  { HEX20, 20 },   { PRISM15, 15 }, { PYRAMID13, 13 }, { ITRI9, 9 },
        { TRI10, 10 }, { ITRI12, 12 },  { TRI15, 15 },   { ITRI15, 15 },    { TRI21, 21 },
        { LINE4, 4 },  { LINE5, 5 },    { LINE6, 6 },    { TET20, 20 },     { TET35, 35 },
        { TET56, 56 }, { HEX64, 64 },   { HEX125, 125 }
    };
    for (auto & n : nodes)
        EXPECT_EQ(MshFile::get_nodes_per_element(n.first), n.second);

    EXPECT_THROW_MSG(MshFile::get_nodes_per_element(NONE), "Unknown element type '-1'");
}
