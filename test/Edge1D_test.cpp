#include <gmock/gmock.h>
#include "TestConfig.h"
#include "ExceptionTestMacros.h"
#include "gmshparsercpp/MshFile.h"

using namespace gmshparsercpp;
using namespace testing;

TEST(Edge1DTest, single_block)
{
    std::string file_name = std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/1d.msh");
    MshFile f(file_name);
    EXPECT_NO_THROW({ f.parse(); });
    EXPECT_EQ(f.get_version(), 4.1);
    EXPECT_TRUE(f.is_ascii());

    std::vector<MshFile::PhysicalName> test_phys_names = { MshFile::PhysicalName(0, 2, "left"),
                                                           MshFile::PhysicalName(0, 3, "right"),
                                                           MshFile::PhysicalName(1, 1, "block") };
    auto phys_names = f.get_physical_names();
    EXPECT_EQ(phys_names.size(), 3);
    for (std::size_t i = 0; i < phys_names.size(); i++) {
        EXPECT_EQ(phys_names[i].dimension, test_phys_names[i].dimension);
        EXPECT_EQ(phys_names[i].tag, test_phys_names[i].tag);
        EXPECT_STREQ(phys_names[i].name.c_str(), test_phys_names[i].name.c_str());
    }

    auto nodes = f.get_nodes();
    EXPECT_EQ(nodes.size(), 3);

    std::vector<int> dims = { 0, 0, 1 };
    // clang-format off
    std::vector<std::vector<MshFile::Point>> pts = {
        {
            { 0., 0., 0. }
        },
        {
            { 2., 0., 0. }
        },
        {
            { 0.5, 0., 0. },
            { 1., 0., 0. },
            { 1.5, 0., 0. }
        }
    };
    // clang-format on
    for (int i = 0; i < nodes.size(); i++) {
        EXPECT_EQ(nodes[i].dimension, dims[i]);
        ASSERT_EQ(nodes[i].coordinates.size(), pts[i].size());
        for (int j = 0; j < nodes[i].coordinates.size(); j++) {
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].x, pts[i][j].x);
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].y, pts[i][j].y);
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].z, pts[i][j].z);
        }
    }

#if 0
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
#endif

    // clang-format off
    std::vector<MshFile::PointEntity> test_pt_ents = {
        MshFile::PointEntity(1, 0, 0, 0, { 2 }),
        MshFile::PointEntity(2, 2, 0, 0, { 3 })
    };
    // clang-format on
    auto pt_ents = f.get_point_entities();
    EXPECT_EQ(pt_ents.size(), 2);
    for (auto i = 0; i < pt_ents.size(); i++) {
        EXPECT_EQ(pt_ents[i].tag, test_pt_ents[i].tag);
        EXPECT_DOUBLE_EQ(pt_ents[i].x, test_pt_ents[i].x);
        EXPECT_DOUBLE_EQ(pt_ents[i].y, test_pt_ents[i].y);
        EXPECT_DOUBLE_EQ(pt_ents[i].z, test_pt_ents[i].z);
        EXPECT_THAT(pt_ents[i].physical_tags, ElementsAreArray(test_pt_ents[i].physical_tags));
    }

    // clang-format off
    std::vector<std::vector<std::vector<int>>> conn = {
        {
            { 1 }
        },
        {
            { 2 }
        },
        {
            { 1, 3 },
            { 3, 4 },
            { 4, 5 },
            { 5, 2 }
        }
    };
    // clang-format on
    auto el_blks = f.get_element_blocks();
    EXPECT_EQ(el_blks.size(), 3);

    for (int i = 0; i < el_blks.size(); i++) {
        EXPECT_EQ(el_blks[i].elements.size(), conn[i].size());
        for (int j = 0; j < el_blks[i].elements.size(); j++) {
            for (int k = 0; k < el_blks[i].elements[j].node_tags.size(); k++)
                EXPECT_EQ(el_blks[i].elements[j].node_tags[k], conn[i][j][k]);
        }
    }
}
