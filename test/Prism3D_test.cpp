#include <gmock/gmock.h>
#include "TestConfig.h"
#include "ExceptionTestMacros.h"
#include "gmshparsercpp/MshFile.h"

using namespace gmshparsercpp;
using namespace testing;

namespace gold {

std::vector<int> phys_blk_dim = { 2, 2, 2, 2, 2, 2, 3 };
std::vector<int> phys_blk_tag = { 1001, 1002, 1003, 1004, 1005, 1006, 100 };
std::vector<std::string> phys_blk_name = {
    "front", "back", "right", "left", "bottom", "top", "blk1"
};

namespace v2 {

std::vector<std::vector<MshFile::Point>> pts = {
    { { 0, 0, 0 } },     { { 1, 0, 0 } },     { { 1, 1, 0 } },      { { 0, 1, 0 } },
    { { 1, 0, 1 } },     { { 1, 1, 1 } },     { { 0, 1, 1 } },      { { 0, 0, 1 } },
    { { 1, 0, 0.5 } },   { { 1, 1, 0.5 } },   { { 0, 1, 0.5 } },    { { 0, 0, 0.5 } },
    { { 0.5, 0.5, 0 } }, { { 0.5, 0.5, 1 } }, { { 0.5, 0.5, 0.5 } }
};

std::vector<int> block_elem_size = { 4, 4, 2, 2, 2, 2, 8 };
std::vector<int> block_elem_type = { TRI3, TRI3, QUAD4, QUAD4, QUAD4, QUAD4, PRISM6 };
std::vector<std::vector<std::vector<int>>> block_elem_conn = {
    { { 1, 2, 13 }, { 4, 1, 13 }, { 2, 3, 13 }, { 3, 4, 13 } },

    { { 8, 5, 14 }, { 7, 8, 14 }, { 5, 6, 14 }, { 6, 7, 14 } },

    { { 2, 3, 10, 9 }, { 9, 10, 6, 5 } },

    { { 3, 4, 11, 10 }, { 10, 11, 7, 6 } },

    { { 4, 1, 12, 11 }, { 11, 12, 8, 7 } },

    { { 1, 2, 9, 12 }, { 12, 9, 5, 8 } },

    { { 1, 2, 13, 12, 9, 15 },
      { 12, 9, 15, 8, 5, 14 },
      { 4, 1, 13, 11, 12, 15 },
      { 11, 12, 15, 7, 8, 14 },
      { 2, 3, 13, 9, 10, 15 },
      { 9, 10, 15, 5, 6, 14 },
      { 3, 4, 13, 10, 11, 15 },
      { 10, 11, 15, 6, 7, 14 } }
};

} // namespace v2

namespace v4 {

std::vector<int> node_dim = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3 };
std::vector<std::vector<MshFile::Point>> pts = { { { 0, 0, 0 } },
                                                 { { 1, 0, 0 } },
                                                 { { 1, 1, 0 } },
                                                 { { 0, 1, 0 } },
                                                 { { 1, 0, 1 } },
                                                 { { 1, 1, 1 } },
                                                 { { 0, 1, 1 } },
                                                 { { 0, 0, 1 } },
                                                 { { 1, 0, 0.5 } },
                                                 { { 1, 1, 0.5 } },
                                                 { { 0, 1, 0.5 } },
                                                 { { 0, 0, 0.5 } },
                                                 { { 0.5, 0.5, 0 } },
                                                 {},
                                                 {},
                                                 {},
                                                 {},
                                                 { { 0.5, 0.5, 1 } },
                                                 { { 0.5, 0.5, 0.5 } } };

std::vector<int> block_elem_size = { 4, 2, 2, 2, 2, 4, 8 };
std::vector<int> block_elem_type = { TRI3, QUAD4, QUAD4, QUAD4, QUAD4, TRI3, PRISM6 };
std::vector<std::vector<std::vector<int>>> block_elem_conn = {
    { { 1, 2, 13 }, { 4, 1, 13 }, { 2, 3, 13 }, { 3, 4, 13 } },

    { { 2, 3, 10, 9 }, { 9, 10, 6, 5 } },

    { { 3, 4, 11, 10 }, { 10, 11, 7, 6 } },

    { { 4, 1, 12, 11 }, { 11, 12, 8, 7 } },

    { { 1, 2, 9, 12 }, { 12, 9, 5, 8 } },

    { { 8, 5, 14 }, { 7, 8, 14 }, { 5, 6, 14 }, { 6, 7, 14 } },

    { { 1, 2, 13, 12, 9, 15 },
      { 12, 9, 15, 8, 5, 14 },
      { 4, 1, 13, 11, 12, 15 },
      { 11, 12, 15, 7, 8, 14 },
      { 2, 3, 13, 9, 10, 15 },
      { 9, 10, 15, 5, 6, 14 },
      { 3, 4, 13, 10, 11, 15 },
      { 10, 11, 15, 6, 7, 14 } }
};

} // namespace v4

} // namespace gold

TEST(Prism3DTest, v4_asc)
{
    std::string file_name =
        std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/prism-v4.asc.msh");
    MshFile f(file_name);
    EXPECT_NO_THROW({ f.parse(); });
    EXPECT_EQ(f.get_version(), 4.1);
    EXPECT_TRUE(f.is_ascii());

    auto phys_names = f.get_physical_names();
    EXPECT_EQ(phys_names.size(), gold::phys_blk_name.size());
    for (std::size_t i = 0; i < phys_names.size(); i++) {
        EXPECT_EQ(phys_names[i].dimension, gold::phys_blk_dim[i]);
        EXPECT_EQ(phys_names[i].tag, gold::phys_blk_tag[i]);
        EXPECT_EQ(phys_names[i].name, gold::phys_blk_name[i]);
    }

    auto nodes = f.get_nodes();
    for (int i = 0; i < nodes.size(); i++) {
        EXPECT_EQ(nodes[i].dimension, gold::v4::node_dim[i]);
        EXPECT_EQ(nodes[i].coordinates.size(), gold::v4::pts[i].size());
        for (int j = 0; j < nodes[i].coordinates.size(); j++) {
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].x, gold::v4::pts[i][j].x);
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].y, gold::v4::pts[i][j].y);
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].z, gold::v4::pts[i][j].z);
        }
    }

    auto el_blks = f.get_element_blocks();
    for (int i = 0; i < el_blks.size(); i++) {
        EXPECT_EQ(el_blks[i].elements.size(), gold::v4::block_elem_size[i]);
        EXPECT_EQ(el_blks[i].element_type, gold::v4::block_elem_type[i]);
        for (int j = 0; j < el_blks[i].elements.size(); j++) {
            for (int k = 0; k < el_blks[i].elements[j].node_tags.size(); k++)
                EXPECT_EQ(el_blks[i].elements[j].node_tags[k], gold::v4::block_elem_conn[i][j][k]);
        }
    }
}

TEST(Prism3DTest, v4_bin)
{
    std::string file_name =
        std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/prism-v4.bin.msh");
    MshFile f(file_name);
    EXPECT_NO_THROW({ f.parse(); });
    EXPECT_EQ(f.get_version(), 4.1);
    EXPECT_FALSE(f.is_ascii());

    auto phys_names = f.get_physical_names();
    EXPECT_EQ(phys_names.size(), gold::phys_blk_name.size());
    for (std::size_t i = 0; i < phys_names.size(); i++) {
        EXPECT_EQ(phys_names[i].dimension, gold::phys_blk_dim[i]);
        EXPECT_EQ(phys_names[i].tag, gold::phys_blk_tag[i]);
        EXPECT_EQ(phys_names[i].name, gold::phys_blk_name[i]);
    }

    auto nodes = f.get_nodes();
    for (int i = 0; i < nodes.size(); i++) {
        EXPECT_EQ(nodes[i].dimension, gold::v4::node_dim[i]);
        EXPECT_EQ(nodes[i].coordinates.size(), gold::v4::pts[i].size());
        for (int j = 0; j < nodes[i].coordinates.size(); j++) {
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].x, gold::v4::pts[i][j].x);
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].y, gold::v4::pts[i][j].y);
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].z, gold::v4::pts[i][j].z);
        }
    }

    auto el_blks = f.get_element_blocks();
    for (int i = 0; i < el_blks.size(); i++) {
        EXPECT_EQ(el_blks[i].elements.size(), gold::v4::block_elem_size[i]);
        EXPECT_EQ(el_blks[i].element_type, gold::v4::block_elem_type[i]);
        for (int j = 0; j < el_blks[i].elements.size(); j++) {
            for (int k = 0; k < el_blks[i].elements[j].node_tags.size(); k++)
                EXPECT_EQ(el_blks[i].elements[j].node_tags[k], gold::v4::block_elem_conn[i][j][k]);
        }
    }
}

TEST(Prism3DTest, v2_asc)
{
    std::string file_name =
        std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/prism-v2.asc.msh");
    MshFile f(file_name);
    EXPECT_NO_THROW({ f.parse(); });
    EXPECT_EQ(f.get_version(), 2.2);
    EXPECT_TRUE(f.is_ascii());

    auto phys_names = f.get_physical_names();
    EXPECT_EQ(phys_names.size(), gold::phys_blk_name.size());
    for (std::size_t i = 0; i < phys_names.size(); i++) {
        EXPECT_EQ(phys_names[i].dimension, gold::phys_blk_dim[i]);
        EXPECT_EQ(phys_names[i].tag, gold::phys_blk_tag[i]);
        EXPECT_EQ(phys_names[i].name, gold::phys_blk_name[i]);
    }

    auto nodes = f.get_nodes();
    for (int i = 0; i < nodes.size(); i++) {
        EXPECT_EQ(nodes[i].coordinates.size(), gold::v2::pts[i].size());
        for (int j = 0; j < nodes[i].coordinates.size(); j++) {
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].x, gold::v2::pts[i][j].x);
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].y, gold::v2::pts[i][j].y);
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].z, gold::v2::pts[i][j].z);
        }
    }

    auto el_blks = f.get_element_blocks();
    for (int i = 0; i < el_blks.size(); i++) {
        EXPECT_EQ(el_blks[i].elements.size(), gold::v2::block_elem_size[i]);
        EXPECT_EQ(el_blks[i].element_type, gold::v2::block_elem_type[i]);
        for (int j = 0; j < el_blks[i].elements.size(); j++) {
            for (int k = 0; k < el_blks[i].elements[j].node_tags.size(); k++)
                EXPECT_EQ(el_blks[i].elements[j].node_tags[k], gold::v2::block_elem_conn[i][j][k]);
        }
    }
}

TEST(Prism3DTest, v2_bin)
{
    std::string file_name =
        std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/prism-v2.bin.msh");
    MshFile f(file_name);
    EXPECT_NO_THROW({ f.parse(); });
    EXPECT_EQ(f.get_version(), 2.2);
    EXPECT_FALSE(f.is_ascii());

    auto phys_names = f.get_physical_names();
    EXPECT_EQ(phys_names.size(), gold::phys_blk_name.size());
    for (std::size_t i = 0; i < phys_names.size(); i++) {
        EXPECT_EQ(phys_names[i].dimension, gold::phys_blk_dim[i]);
        EXPECT_EQ(phys_names[i].tag, gold::phys_blk_tag[i]);
        EXPECT_EQ(phys_names[i].name, gold::phys_blk_name[i]);
    }

    auto nodes = f.get_nodes();
    for (int i = 0; i < nodes.size(); i++) {
        EXPECT_EQ(nodes[i].coordinates.size(), gold::v2::pts[i].size());
        for (int j = 0; j < nodes[i].coordinates.size(); j++) {
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].x, gold::v2::pts[i][j].x);
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].y, gold::v2::pts[i][j].y);
            EXPECT_DOUBLE_EQ(nodes[i].coordinates[j].z, gold::v2::pts[i][j].z);
        }
    }

    auto el_blks = f.get_element_blocks();
    for (int i = 0; i < el_blks.size(); i++) {
        EXPECT_EQ(el_blks[i].elements.size(), gold::v2::block_elem_size[i]);
        EXPECT_EQ(el_blks[i].element_type, gold::v2::block_elem_type[i]);
        for (int j = 0; j < el_blks[i].elements.size(); j++) {
            for (int k = 0; k < el_blks[i].elements[j].node_tags.size(); k++)
                EXPECT_EQ(el_blks[i].elements[j].node_tags[k], gold::v2::block_elem_conn[i][j][k]);
        }
    }
}
