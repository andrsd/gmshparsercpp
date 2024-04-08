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
