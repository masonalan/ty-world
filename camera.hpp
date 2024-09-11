//
// Created by James Pickering on 9/1/24.
//

#pragma once

#include <glm/glm.hpp>

class GLFWwindow;

struct Camera {
    Camera();

    auto setAspectRatio(float ratio) noexcept -> void;

    auto lookAt(float x, float y) noexcept -> void;
    auto move(GLFWwindow* window) noexcept -> void;
    auto zoom(float amount) noexcept -> void;

    [[nodiscard]] auto view() const noexcept -> glm::mat4;
    [[nodiscard]] auto projection() const noexcept -> glm::mat4;
    [[nodiscard]] auto position() const noexcept -> glm::vec3;
    [[nodiscard]] auto front() const noexcept -> glm::vec3;

private:
    glm::vec3 _pos;
    glm::vec3 _front;
    glm::vec3 _up;

    float _deltaTime = 0.f;
    float _lastFrame = 0.f;
    float _aspectRatio;
    float _fov;
    float _lastX;
    float _lastY;
    float _pitch;
    float _yaw;
    bool _cursorMovedOnce;
};
