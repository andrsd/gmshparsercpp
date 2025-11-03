// SPDX-FileCopyrightText: 2024 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#ifdef GMSHPARSERCPP_WITH_FMT
    #include <fmt/core.h>
    #include <fmt/format.h>
#else
    #include <format>
#endif

namespace gmshparsercpp {

/// See [1] for details
///
/// [1] https://gmsh.info/doc/texinfo/gmsh.html#MSH-file-format
enum ElementType {
    NONE = -1,
    LINE2 = 1,
    TRI3 = 2,
    QUAD4 = 3,
    TET4 = 4,
    HEX8 = 5,
    PRISM6 = 6,
    PYRAMID5 = 7,
    LINE3 = 8,
    TRI6 = 9,
    QUAD9 = 10,
    TET10 = 11,
    HEX27 = 12,
    PRISM18 = 13,
    PYRAMID14 = 14,
    POINT = 15,
    QUAD8 = 16,
    HEX20 = 17,
    PRISM15 = 18,
    PYRAMID13 = 19,
    ITRI9 = 20,
    TRI10 = 21,
    ITRI12 = 22,
    TRI15 = 23,
    ITRI15 = 24,
    TRI21 = 25,
    LINE4 = 26,
    LINE5 = 27,
    LINE6 = 28,
    TET20 = 29,
    TET35 = 30,
    TET56 = 31,
    HEX64 = 92,
    HEX125 = 93
};

} // namespace gmshparsercpp

#ifdef GMSHPARSERCPP_WITH_FMT

template <>
struct fmt::formatter<gmshparsercpp::ElementType> : fmt::formatter<std::string> {
    auto
    format(gmshparsercpp::ElementType e, fmt::format_context & ctx) const
    {
        std::string name;
        switch (e) {
        case gmshparsercpp::ElementType::NONE:
            return fmt::formatter<std::string>::format("NONE", ctx);

        case gmshparsercpp::ElementType::POINT:
            return fmt::formatter<std::string>::format("POINT", ctx);

        case gmshparsercpp::ElementType::LINE2:
            return fmt::formatter<std::string>::format("LINE2", ctx);
        case gmshparsercpp::ElementType::LINE3:
            return fmt::formatter<std::string>::format("LINE3", ctx);
        case gmshparsercpp::ElementType::LINE5:
            return fmt::formatter<std::string>::format("LINE5", ctx);
        case gmshparsercpp::ElementType::LINE6:
            return fmt::formatter<std::string>::format("LINE6", ctx);

        case gmshparsercpp::ElementType::TRI3:
            return fmt::formatter<std::string>::format("TRI3", ctx);
        case gmshparsercpp::ElementType::TRI6:
            return fmt::formatter<std::string>::format("TRI6", ctx);
        case gmshparsercpp::ElementType::TRI10:
            return fmt::formatter<std::string>::format("TRI10", ctx);
        case gmshparsercpp::ElementType::TRI15:
            return fmt::formatter<std::string>::format("TRI15", ctx);
        case gmshparsercpp::ElementType::TRI21:
            return fmt::formatter<std::string>::format("TRI21", ctx);

        case gmshparsercpp::ElementType::ITRI9:
            return fmt::formatter<std::string>::format("ITRI9", ctx);
        case gmshparsercpp::ElementType::ITRI12:
            return fmt::formatter<std::string>::format("ITRI12", ctx);
        case gmshparsercpp::ElementType::ITRI15:
            return fmt::formatter<std::string>::format("ITRI15", ctx);

        case gmshparsercpp::ElementType::QUAD4:
            return fmt::formatter<std::string>::format("QUAD4", ctx);
        case gmshparsercpp::ElementType::QUAD8:
            return fmt::formatter<std::string>::format("QUAD8", ctx);
        case gmshparsercpp::ElementType::QUAD9:
            return fmt::formatter<std::string>::format("QUAD9", ctx);

        case gmshparsercpp::ElementType::TET4:
            return fmt::formatter<std::string>::format("TET4", ctx);
        case gmshparsercpp::ElementType::TET10:
            return fmt::formatter<std::string>::format("TET10", ctx);
        case gmshparsercpp::ElementType::TET20:
            return fmt::formatter<std::string>::format("TET20", ctx);
        case gmshparsercpp::ElementType::TET35:
            return fmt::formatter<std::string>::format("TET35", ctx);
        case gmshparsercpp::ElementType::TET56:
            return fmt::formatter<std::string>::format("TET56", ctx);

        case gmshparsercpp::ElementType::HEX8:
            return fmt::formatter<std::string>::format("HEX8", ctx);
        case gmshparsercpp::ElementType::HEX20:
            return fmt::formatter<std::string>::format("HEX20", ctx);
        case gmshparsercpp::ElementType::HEX27:
            return fmt::formatter<std::string>::format("HEX27", ctx);
        case gmshparsercpp::ElementType::HEX64:
            return fmt::formatter<std::string>::format("HEX64", ctx);
        case gmshparsercpp::ElementType::HEX125:
            return fmt::formatter<std::string>::format("HEX125", ctx);

        case gmshparsercpp::ElementType::PRISM6:
            return fmt::formatter<std::string>::format("PRISM6", ctx);
        case gmshparsercpp::ElementType::PRISM18:
            return fmt::formatter<std::string>::format("PRISM18", ctx);
        case gmshparsercpp::ElementType::PRISM15:
            return fmt::formatter<std::string>::format("PRISM15", ctx);

        case gmshparsercpp::ElementType::PYRAMID5:
            return fmt::formatter<std::string>::format("PYRAMID5", ctx);
        case gmshparsercpp::ElementType::PYRAMID13:
            return fmt::formatter<std::string>::format("PYRAMID13", ctx);
        case gmshparsercpp::ElementType::PYRAMID14:
            return fmt::formatter<std::string>::format("PYRAMID14", ctx);

        default:
            return fmt::formatter<std::string>::format("Unknown", ctx);
        }
    }
};

#else

template <>
struct std::formatter<gmshparsercpp::ElementType> : std::formatter<std::string_view> {
    auto
    format(gmshparsercpp::ElementType e, auto & ctx) const
    {
        switch (e) {
        case gmshparsercpp::ElementType::NONE:
            return std::formatter<std::string_view>::format("NONE", ctx);

        case gmshparsercpp::ElementType::POINT:
            return std::formatter<std::string_view>::format("POINT", ctx);

        case gmshparsercpp::ElementType::LINE2:
            return std::formatter<std::string_view>::format("LINE2", ctx);
        case gmshparsercpp::ElementType::LINE3:
            return std::formatter<std::string_view>::format("LINE3", ctx);
        case gmshparsercpp::ElementType::LINE5:
            return std::formatter<std::string_view>::format("LINE5", ctx);
        case gmshparsercpp::ElementType::LINE6:
            return std::formatter<std::string_view>::format("LINE6", ctx);

        case gmshparsercpp::ElementType::TRI3:
            return std::formatter<std::string_view>::format("TRI3", ctx);
        case gmshparsercpp::ElementType::TRI6:
            return std::formatter<std::string_view>::format("TRI6", ctx);
        case gmshparsercpp::ElementType::TRI10:
            return std::formatter<std::string_view>::format("TRI10", ctx);
        case gmshparsercpp::ElementType::TRI15:
            return std::formatter<std::string_view>::format("TRI15", ctx);
        case gmshparsercpp::ElementType::TRI21:
            return std::formatter<std::string_view>::format("TRI21", ctx);

        case gmshparsercpp::ElementType::ITRI9:
            return std::formatter<std::string_view>::format("ITRI9", ctx);
        case gmshparsercpp::ElementType::ITRI12:
            return std::formatter<std::string_view>::format("ITRI12", ctx);
        case gmshparsercpp::ElementType::ITRI15:
            return std::formatter<std::string_view>::format("ITRI15", ctx);

        case gmshparsercpp::ElementType::QUAD4:
            return std::formatter<std::string_view>::format("QUAD4", ctx);
        case gmshparsercpp::ElementType::QUAD8:
            return std::formatter<std::string_view>::format("QUAD8", ctx);
        case gmshparsercpp::ElementType::QUAD9:
            return std::formatter<std::string_view>::format("QUAD9", ctx);

        case gmshparsercpp::ElementType::TET4:
            return std::formatter<std::string_view>::format("TET4", ctx);
        case gmshparsercpp::ElementType::TET10:
            return std::formatter<std::string_view>::format("TET10", ctx);
        case gmshparsercpp::ElementType::TET20:
            return std::formatter<std::string_view>::format("TET20", ctx);
        case gmshparsercpp::ElementType::TET35:
            return std::formatter<std::string_view>::format("TET35", ctx);
        case gmshparsercpp::ElementType::TET56:
            return std::formatter<std::string_view>::format("TET56", ctx);

        case gmshparsercpp::ElementType::HEX8:
            return std::formatter<std::string_view>::format("HEX8", ctx);
        case gmshparsercpp::ElementType::HEX20:
            return std::formatter<std::string_view>::format("HEX20", ctx);
        case gmshparsercpp::ElementType::HEX27:
            return std::formatter<std::string_view>::format("HEX27", ctx);
        case gmshparsercpp::ElementType::HEX64:
            return std::formatter<std::string_view>::format("HEX64", ctx);
        case gmshparsercpp::ElementType::HEX125:
            return std::formatter<std::string_view>::format("HEX125", ctx);

        case gmshparsercpp::ElementType::PRISM6:
            return std::formatter<std::string_view>::format("PRISM6", ctx);
        case gmshparsercpp::ElementType::PRISM18:
            return std::formatter<std::string_view>::format("PRISM18", ctx);
        case gmshparsercpp::ElementType::PRISM15:
            return std::formatter<std::string_view>::format("PRISM15", ctx);

        case gmshparsercpp::ElementType::PYRAMID5:
            return std::formatter<std::string_view>::format("PYRAMID5", ctx);
        case gmshparsercpp::ElementType::PYRAMID13:
            return std::formatter<std::string_view>::format("PYRAMID13", ctx);
        case gmshparsercpp::ElementType::PYRAMID14:
            return std::formatter<std::string_view>::format("PYRAMID14", ctx);

        default:
            return std::formatter<std::string_view>::format("Unknown", ctx);
        }
    }
};

#endif
