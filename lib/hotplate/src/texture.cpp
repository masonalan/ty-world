//
// Created by James Pickering on 9/1/24.
//

#include <hotplate/texture.hpp>

#include <glad/glad.h>
#include <stb_image.h>

auto Texture::fromPng(const char* path, bool flip) -> unsigned int {
    auto width = int{};
    auto height = int{};
    auto numChannels = int{};

    stbi_set_flip_vertically_on_load(flip);

    const auto data = stbi_load(path, &width, &height, &numChannels, 0);

    auto id = (unsigned int){};

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return id;
}
