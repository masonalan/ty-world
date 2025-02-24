#include "benchmark.hpp"

#include <chrono>

#include "chunk_entity.hpp"
#include "graph.hpp"
#include "render.hpp"

#include <hotplate/camera.hpp>
#include <hotplate/enum_math.hpp>
#include <hotplate/shader.hpp>
#include <hotplate/shapes.hpp>

#include <fractal/fractal.hpp>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <memory>

constexpr auto VIEWPORT_SCALE = 2;
constexpr long long nsInS = 1000000000;

auto isFirstMovement = true;

auto framebufferSizeCallback(GLFWwindow* window, int width, int height) -> void;
auto cursorCallback(GLFWwindow* window, double xPos, double yPos) -> void;
auto scrollCallback(GLFWwindow* window, double xOffset, double yOffset) -> void;
auto processInput(GLFWwindow* window) -> void;

auto camera = Camera{};

long long renderRunTime{};

auto main() -> int {
    // ---------------------------------------------------------------------------------------------------------------
    // Initialize GLFW
    //
    // - the first two hints require a certain OpenGL version
    // - the third hint specifies we want to use the core profile
    // - the final one is required for macOS (not sure why)
    // ---------------------------------------------------------------------------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    const auto monitor = glfwGetPrimaryMonitor();
    int xPos, yPos, width, height;
    float xScale, yScale;
    glfwGetMonitorWorkarea(monitor, &xPos, &yPos, &width, &height);
    glfwGetMonitorContentScale(monitor, &xScale, &yScale);
    width = static_cast<int>(width * xScale / 2.f);
    height = static_cast<int>(height * yScale / 2.f);

    // ---------------------------------------------------------------------------------------------------------------
    // Create a window object
    //
    // First we request a window to be created. After that, we make the window we just created the current context.
    // In this project we want to hide the mouse like in Minecraft so we disable the cursor. Then we want to setup
    // all the relevant callbacks.
    // ---------------------------------------------------------------------------------------------------------------
    const auto window = glfwCreateWindow(width, height, "Lighting", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // ---------------------------------------------------------------------------------------------------------------
    // Initialize GLAD
    //
    // This must be done before any OpenGL functions are called. GLFW provides a function to get the address of OpenGL
    // function pointers, and GLAD then loads them all. This address is OS-specific.
    // ---------------------------------------------------------------------------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // --------------------------------------------------------------------------------------------------------------
    // Setup viewport
    //
    // Let OpenGL know how we want to display data with respect to the window. My MBP w/ retina display has a 2:1
    // viewport to window ratio.
    // --------------------------------------------------------------------------------------------------------------
    //glViewport(0, 0, width * xScale, height * yScale);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // --------------------------------------------------------------------------------------------------------------
    // Setup all vertex buffers
    // --------------------------------------------------------------------------------------------------------------
    VertexBuffer::init();

    // --------------------------------------------------------------------------------------------------------------
    // Set up shaders
    // --------------------------------------------------------------------------------------------------------------
    const auto instancedShader = Shader{
        "lib/hotplate/instanced_shader.vert",
        "lib/hotplate/main_shader.frag",
        ShaderComponent::DirectionalLight | ShaderComponent::PointLight | ShaderComponent::Flashlight};

    // --------------------------------------------------------------------------------------------------------------
    // Set up camera
    // --------------------------------------------------------------------------------------------------------------
    camera.setAspectRatio(static_cast<float>(width) / static_cast<float>(height));

    // --------------------------------------------------------------------------------------------------------------
    // Set up levels
    // --------------------------------------------------------------------------------------------------------------
    auto scene = render::Scene{};
    scene.createGraph();
    scene.getCurrentGraph().loadAllChunks();

    // --------------------------------------------------------------------------------------------------------------
    // Render state
    // --------------------------------------------------------------------------------------------------------------
    auto z = camera.position();
    auto numFrames = 0;

    instancedShader.use();
    instancedShader.setFloat("scale", 1.f);// TODO: get rid of this uniform

    scene.updateBounds();

    camera.setSpeed(fractal::ReloadGraphThreshold);



    // ---------------------------------------------------------------------------------------------------------------
    // Render loop
    //
    // Check to see if GLFW has been instructed to close the window, and if so break. An iteration of a render loop is
    // called a frame. Thus, a frame rate is how many times the render loop is executed in some period.
    // ---------------------------------------------------------------------------------------------------------------
    auto lastPrint = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window)) {
        auto start = std::chrono::high_resolution_clock::now();
        ++numFrames;
        processInput(window);

        // ------------------------------------------------------------------------------------------------------------
        // Clear screen with a specified color. Otherwise, we would see the results from the last frame render as well.
        // This is usually not desired.
        // ------------------------------------------------------------------------------------------------------------
        glClearColor(0.f, 0.007843137255f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ------------------------------------------------------------------------------------------------------------
        // Handle camera movements
        // ------------------------------------------------------------------------------------------------------------
        if (scene.isPointPastMaxZ(camera.position())) {
            auto& graph = scene.createGraph();
            graph.loadAllChunks();
            scene.zoomInOnce();
        } else {
            if (const auto dirs = scene.getBounds().isPointOutside(camera.position()); !dirs.empty()) {
                for (const auto dir : dirs) {
                    scene.getCurrentGraph().shift(dir);
                    scene.getCurrentGraph().loadAllChunks();
                }
                scene.updateBounds();
            }
        }

        // ------------------------------------------------------------------------------------------------------------
        // Set uniforms
        // ------------------------------------------------------------------------------------------------------------
        instancedShader.setMat4("view", camera.view());
        instancedShader.setMat4("proj", camera.projection());

        // ------------------------------------------------------------------------------------------------------------
        // Render
        // ------------------------------------------------------------------------------------------------------------
        for (const auto& [pos, chunk] : scene.getCurrentGraph().getChunks()) {
            glBindVertexArray(chunk->getVao());
            glDrawArraysInstanced(GL_TRIANGLES, 0, 36, fractal::PointsInChunk);

            switch (const auto error = glGetError()) {
            case GL_INVALID_ENUM:
                std::cerr << "[Error] Invalid enum" << std::endl;
                break;
            case GL_INVALID_VALUE:
                std::cerr << "[Error] Invalid value" << std::endl;
                break;
            case GL_INVALID_OPERATION:
                std::cerr << "[Error] Invalid operation" << std::endl;
                break;
            case 0:
                break;
            default:
                std::cerr << "[Error] Unknown error " << error << std::endl;
                break;
            }
        }

        // -------------------------------------------------------------------------------------------------------
        // Swap the color buffer that is used to render to during this render and show it as output on the screen.
        // -------------------------------------------------------------------------------------------------------
        glfwSwapBuffers(window);

        // -----------------------------------------------
        // Poll for any events, like keyboard/mouse events
        // -----------------------------------------------
        glfwPollEvents();

        auto end = std::chrono::high_resolution_clock::now();
        renderRunTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - lastPrint)
                .count() > 2000) {
            lastPrint = std::chrono::high_resolution_clock::now();

            numFrames = 0;

            Benchmark::printAll();
        }
    }

    glfwTerminate();
    return 0;
}

auto framebufferSizeCallback(GLFWwindow*, int width, int height) -> void {
    glViewport(0, 0, width, height);
}

auto cursorCallback(GLFWwindow*, double xPos, double yPos) -> void {
    camera.lookAt(static_cast<float>(xPos), static_cast<float>(yPos));
}

auto scrollCallback(GLFWwindow*, double, double yOffset) -> void {
    camera.zoom(static_cast<float>(yOffset));
}

auto processInput(GLFWwindow* window) -> void {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    camera.move(window);
}
