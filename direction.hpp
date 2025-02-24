//
// Created by James Pickering on 2/19/25.
//

#pragma once

#include <cstdint>

namespace util {

enum class Direction : std::uint16_t {
    Left = 1,
    Top = 2,
    Right = 4,
    Bottom = 8
};

}