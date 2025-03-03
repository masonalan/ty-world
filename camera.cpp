//
// Created by James Pickering on 9/1/24.
//

#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glfw/glfw3.h>

Camera::Camera()
    : _pos{0.f, 0.f, 3.f},
      _front{0.f, 0.f, -1.f},
      _up{0.f, 1.f, 0.f},
      _fov{45.f},
      _lastX{},
      _lastY{},
      _pitch{45.f},
      _yaw{-90.f},
      _cursorMovedOnce{false},
      _aspectRatio{} {
}

auto Camera::setAspectRatio(float ratio) noexcept -> void {
    _aspectRatio = ratio;
}

auto Camera::lookAt(float x, float y) noexcept -> void {
    if (!_cursorMovedOnce) {
        _lastX = x;
        _lastY = y;
        _cursorMovedOnce = true;
    }

    // First, calculate the cursor's offset from the last frame. The calculation for yOffset is reversed because y
    // coords go from bottom to top. The sensitivity makes the mouse movements much less abrupt.
    const auto sensitivity = 0.1f;
    const auto xOffset = (x - _lastX) * sensitivity;
    const auto yOffset = (_lastY - y) * sensitivity;

    _lastX = x;
    _lastY = y;

    _yaw += xOffset;
    _pitch += yOffset;

    if (_pitch > 89.f) {
        _pitch = 89.f;
    } else if (_pitch < -89.f) {
        _pitch = -89.f;
    }

    const auto direction = glm::vec3{
            cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)),
            sin(glm::radians(_pitch)),
            sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))};

    _front = glm::normalize(direction);
}

auto Camera::move(GLFWwindow *window) noexcept -> void {
    const auto currentFrame = static_cast<float>(glfwGetTime());
    _deltaTime = currentFrame - _lastFrame;
    _lastFrame = currentFrame;

    const auto cameraSpeed = 1.3f * _deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        _pos += cameraSpeed * _front;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        _pos -= cameraSpeed * _front;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        _pos -= glm::normalize(glm::cross(_front, _up)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        _pos += glm::normalize(glm::cross(_front, _up)) * cameraSpeed;
    }
}

auto Camera::zoom(float amount) noexcept -> void {
    _fov -= static_cast<float>(amount);
    if (_fov < 1.f) {
        _fov = 1.f;
    } else if (_fov > 45.f) {
        _fov = 45.f;
    }
}

auto Camera::view() const noexcept -> glm::mat4 {
    return glm::lookAt(_pos, _pos + _front, _up);
}

auto Camera::projection() const noexcept -> glm::mat4 {
    return glm::perspective(glm::radians(_fov), _aspectRatio, 0.1f, 100.f);
}
auto Camera::position() const noexcept -> glm::vec3 {
    return _pos;
}

auto Camera::front() const noexcept -> glm::vec3 {
    return _front;
}
