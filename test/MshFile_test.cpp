#include <gmock/gmock.h>
#include "TestConfig.h"
#include "gmshparsercpp.h"

using namespace gmshparsercpp;
using namespace testing;

TEST(MshFileTest, empty)
{
    EXPECT_THROW(
        {
            std::string file_name =
                std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/empty.msh");
            MshFile f(file_name);
            f.parse();
        },
        std::runtime_error);
}

TEST(MshFileTest, header)
{
    std::string file_name = std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/header-only.msh");
    MshFile f(file_name);
    EXPECT_NO_THROW({ f.parse(); });
    EXPECT_EQ(f.get_version(), 4.1);
    EXPECT_TRUE(f.is_ascii());
}

TEST(MshFileTest, quad)
{
    std::string file_name = std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/quad.msh");
    MshFile f(file_name);
    EXPECT_NO_THROW({ f.parse(); });
    EXPECT_EQ(f.get_version(), 4.1);
    EXPECT_TRUE(f.is_ascii());

    std::vector<MshFile::PhysicalName> test_phys_names = {
        MshFile::PhysicalName(1, 5, "bottom"),
        MshFile::PhysicalName(1, 6, "left"),
        MshFile::PhysicalName(1, 7, "top"),
        MshFile::PhysicalName(1, 8, "right")
    };
    auto phys_names = f.get_physical_names();
    EXPECT_EQ(phys_names.size(), 4);
    for (std::size_t i = 0; i < phys_names.size(); i++) {
        EXPECT_EQ(phys_names[i].dimension, test_phys_names[i].dimension);
        EXPECT_EQ(phys_names[i].tag, test_phys_names[i].tag);
        EXPECT_STREQ(phys_names[i].name.c_str(), test_phys_names[i].name.c_str());
    }
}
