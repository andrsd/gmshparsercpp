#include <gmock/gmock.h>
#include "TestConfig.h"
#include "gmshparsercpp.h"

using namespace gmshparsercpp;

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
    std::string file_name =
        std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/header-only.msh");
    MshFile f(file_name);
    EXPECT_NO_THROW({
        f.parse();
    });
    EXPECT_EQ(f.get_version(), 4.1);
    EXPECT_TRUE(f.is_ascii());
}

TEST(MshFileTest, quad)
{
    std::string file_name =
        std::string(GMSHPARSERCPP_ASSETS_DIR) + std::string("/quad.msh");
    MshFile f(file_name);
    EXPECT_NO_THROW({
        f.parse();
    });
    EXPECT_EQ(f.get_version(), 4.1);
    EXPECT_TRUE(f.is_ascii());
}
