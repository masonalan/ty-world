//
// Created by James Pickering on 9/1/24.
//

#pragma once

#include <glm/glm.hpp>

#include <functional>

class GLFWwindow;

struct Camera {
    Camera();

    auto setAspectRatio(float ratio) noexcept -> void;

    auto lookAt(float x, float y) noexcept -> void;
    auto move(GLFWwindow* window) noexcept -> void;
    auto zoom(float amount) noexcept -> void;
    auto setSpeed(float speed) noexcept -> void;

    [[nodiscard]] auto view() const noexcept -> glm::mat4;
    [[nodiscard]] auto projection() const noexcept -> glm::mat4;
    [[nodiscard]] auto position() const noexcept -> glm::vec3;
    [[nodiscard]] auto front() const noexcept -> glm::vec3;

    std::function<void(bool)> onMove;

private:
    auto update() -> void;

    glm::vec3 _pos;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _worldUp;

    glm::mat4 _proj;
    glm::mat4 _lookAt;

    float _deltaTime = 0.f;
    float _lastFrame = 0.f;
    float _aspectRatio;
    float _fov;
    float _lastX;
    float _lastY;
    float _pitch;
    float _yaw;
    bool _cursorMovedOnce;
    float _speed{10.f};
};
