#include <gmock/gmock.h>
#include "TestConfig.h"
#include "ExceptionTestMacros.h"
#include "gmshparsercpp/MshFile.h"

using namespace gmshparsercpp;
using namespace testing;

TEST(Quad2DTest, v4_asc)
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
    std::vector<int> dims = { 0, 0, 0, 0, 1, 1, 1, 1, 2 };
    std::vector<std::vector<MshFile::Point>> pts = { { { 0., 0., 0. } },
                                                     { { 1., 0., 0. } },
                                                     { { 1., 1., 0. } },
                                                     { { 0., 1., 0. } },

                                                     {},
                                                     {},
                                                     {},
                                                     {},

                                                     { { 0.5, 0.5, 0. } } };
    for (int i = 0; i < nodes.size(); i++) {
        EXPECT_EQ(nodes[i].dimension, dims[i]);
        ASSERT_EQ(nodes[i].coordinates.size(), pts[i].size());
        for (int j = 0; j < nodes[i].coordinates.size(); j++) {
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].x, pts[i][j].x);
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].y, pts[i][j].y);
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].z, pts[i][j].z);
        }
    }

    auto el_blks = f.get_element_blocks();
    EXPECT_EQ(el_blks.size(), 9);
    std::vector<std::vector<std::vector<int>>> conn = {
        { { 1 } },    { { 2 } },    { { 3 } },
        { { 4 } },    { { 1, 2 } }, { { 2, 3 } },
        { { 3, 4 } }, { { 4, 1 } }, { { 2, 5, 1 }, { 1, 5, 4 }, { 3, 5, 2 }, { 4, 5, 3 } },
    };
    for (int i = 0; i < el_blks.size(); i++) {
        EXPECT_EQ(el_blks[i].elements.size(), conn[i].size());
        for (int j = 0; j < el_blks[i].elements.size(); j++) {
            for (int k = 0; k < el_blks[i].elements[j].node_tags.size(); k++)
                EXPECT_EQ(el_blks[i].elements[j].node_tags[k], conn[i][j][k]);
        }
    }
}

TEST(Quad2DTest, v4_bin)
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
    std::vector<int> dims = { 0, 0, 0, 0, 1, 1, 1, 1 };
    std::vector<std::vector<MshFile::Point>> pts = { { { 0., 0., 0. } },
                                                     { { 1., 0., 0. } },
                                                     { { 1., 1., 0. } },
                                                     { { 0., 1., 0. } },
                                                     {},
                                                     {},
                                                     {},
                                                     {} };
    for (int i = 0; i < nodes.size(); i++) {
        EXPECT_EQ(nodes[i].dimension, dims[i]);
        EXPECT_EQ(nodes[i].coordinates.size(), pts[i].size());
        for (int j = 0; j < nodes[i].coordinates.size(); j++) {
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].x, pts[i][j].x);
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].y, pts[i][j].y);
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].z, pts[i][j].z);
        }
    }

    auto el_blks = f.get_element_blocks();
    EXPECT_EQ(el_blks.size(), 4);
    std::vector<std::vector<std::vector<int>>> conn = {
        { { 1, 2 } },
        { { 2, 3 } },
        { { 3, 4 } },
        { { 4, 1 } },
    };
    for (int i = 0; i < el_blks.size(); i++) {
        EXPECT_EQ(el_blks[i].elements.size(), conn[i].size());
        for (int j = 0; j < el_blks[i].elements.size(); j++) {
            for (int k = 0; k < el_blks[i].elements[j].node_tags.size(); k++)
                EXPECT_EQ(el_blks[i].elements[j].node_tags[k], conn[i][j][k]);
        }
    }
}

TEST(Quad2DTest, v2_asc)
{
    std::string file_name = std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/quad-v2.asc.msh");
    MshFile f(file_name);
    EXPECT_NO_THROW({ f.parse(); });
    EXPECT_EQ(f.get_version(), 2.2);
    EXPECT_TRUE(f.is_ascii());

    auto nodes = f.get_nodes();
    EXPECT_EQ(nodes.size(), 4);
    std::vector<int> dims = { 0, 0, 0, 0, 1, 1, 1, 1 };
    std::vector<std::vector<MshFile::Point>> pts = { { { 0., 0., 0. } },
                                                     { { 1., 0., 0. } },
                                                     { { 1., 1., 0. } },
                                                     { { 0., 1., 0. } },
                                                     {},
                                                     {},
                                                     {},
                                                     {} };
    for (int i = 0; i < nodes.size(); i++) {
        EXPECT_EQ(nodes[i].coordinates.size(), pts[i].size());
        for (int j = 0; j < nodes[i].coordinates.size(); j++) {
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].x, pts[i][j].x);
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].y, pts[i][j].y);
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].z, pts[i][j].z);
        }
    }

    auto el_blks = f.get_element_blocks();
    EXPECT_EQ(el_blks.size(), 4);
    std::vector<std::vector<std::vector<int>>> conn = {
        { { 1, 2 } },
        { { 2, 3 } },
        { { 3, 4 } },
        { { 4, 1 } },
    };
    for (int i = 0; i < el_blks.size(); i++) {
        EXPECT_EQ(el_blks[i].elements.size(), conn[i].size());
        for (int j = 0; j < el_blks[i].elements.size(); j++) {
            for (int k = 0; k < el_blks[i].elements[j].node_tags.size(); k++)
                EXPECT_EQ(el_blks[i].elements[j].node_tags[k], conn[i][j][k]);
        }
    }
}

TEST(Quad2DTest, v2_bin)
{
    std::string file_name = std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/quad-v2.bin.msh");
    MshFile f(file_name);
    EXPECT_NO_THROW({ f.parse(); });
    EXPECT_EQ(f.get_version(), 2.2);
    EXPECT_FALSE(f.is_ascii());

    auto nodes = f.get_nodes();
    EXPECT_EQ(nodes.size(), 4);
    std::vector<int> dims = { 0, 0, 0, 0, 1, 1, 1, 1 };
    std::vector<std::vector<MshFile::Point>> pts = { { { 0., 0., 0. } },
                                                     { { 1., 0., 0. } },
                                                     { { 1., 1., 0. } },
                                                     { { 0., 1., 0. } },
                                                     {},
                                                     {},
                                                     {},
                                                     {} };
    for (int i = 0; i < nodes.size(); i++) {
        EXPECT_EQ(nodes[i].coordinates.size(), pts[i].size());
        for (int j = 0; j < nodes[i].coordinates.size(); j++) {
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].x, pts[i][j].x);
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].y, pts[i][j].y);
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].z, pts[i][j].z);
        }
    }

    auto el_blks = f.get_element_blocks();
    EXPECT_EQ(el_blks.size(), 4);
    std::vector<std::vector<std::vector<int>>> conn = {
        { { 1, 2 } },
        { { 2, 3 } },
        { { 3, 4 } },
        { { 4, 1 } },
    };
    for (int i = 0; i < el_blks.size(); i++) {
        EXPECT_EQ(el_blks[i].elements.size(), conn[i].size());
        for (int j = 0; j < el_blks[i].elements.size(); j++) {
            for (int k = 0; k < el_blks[i].elements[j].node_tags.size(); k++)
                EXPECT_EQ(el_blks[i].elements[j].node_tags[k], conn[i][j][k]);
        }
    }
}
