//
// Created by James Pickering on 9/1/24.
//

#pragma once

struct Texture {
    static auto fromPng(const char* path, bool flip = true) -> unsigned int;
};
