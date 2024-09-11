#include "camera.hpp"
#include "enum_math.hpp"
#include "light.hpp"
#include "material.hpp"
#include "shader.hpp"
#include "shapes.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

constexpr auto WIN_WIDTH = 1500;
constexpr auto WIN_HEIGHT = 1200;
constexpr auto VIEWPORT_SCALE = 2;

auto framebufferSizeCallback(GLFWwindow *window, int width, int height) -> void;
auto cursorCallback(GLFWwindow *window, double xPos, double yPos) -> void;
auto scrollCallback(GLFWwindow *window, double xOffset, double yOffset) -> void;
auto processInput(GLFWwindow *window) -> void;

auto camera = Camera{};

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

    // ---------------------------------------------------------------------------------------------------------------
    // Create a window object
    //
    // First we request a window to be created. After that, we make the window we just created the current context.
    // In this project we want to hide the mouse like in Minecraft so we disable the cursor. Then we want to setup
    // all the relevant callbacks.
    // ---------------------------------------------------------------------------------------------------------------
    const auto window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Lighting", nullptr, nullptr);
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
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // --------------------------------------------------------------------------------------------------------------
    // Setup viewport
    //
    // Let OpenGL know how we want to display data with respect to the window. My MBP w/ retina display has a 2:1
    // viewport to window ratio.
    // --------------------------------------------------------------------------------------------------------------
    glViewport(0, 0, WIN_WIDTH * VIEWPORT_SCALE, WIN_HEIGHT * VIEWPORT_SCALE);
    glEnable(GL_DEPTH_TEST);

    // --------------------------------------------------------------------------------------------------------------
    // Setup all vertex buffers
    // --------------------------------------------------------------------------------------------------------------
    VertexBuffer::init();

    // --------------------------------------------------------------------------------------------------------------
    // Set up shaders
    // --------------------------------------------------------------------------------------------------------------
    const auto shader = Shader{"main_shader.vert",
                               "main_shader.frag",
                               ShaderComponent::DirectionalLight | ShaderComponent::PointLight | ShaderComponent::Flashlight};
    const auto instancedShader = Shader{"instanced_shader.vert",
                                        "main_shader.frag",
                                        ShaderComponent::DirectionalLight | ShaderComponent::PointLight | ShaderComponent::Flashlight};
    const auto lightSourceShader = Shader{"main_shader.vert", "light_shader.frag"};


    const auto container = Container{};
    const auto texturedCube = TexturedCube{shader};
    const auto lightSource = LightSource{};

    camera.setAspectRatio(static_cast<float>(WIN_WIDTH) / static_cast<float>(WIN_HEIGHT));

    const auto light = Light{
            {0.2f, 0.2f, 0.2f},
            {0.5f, 0.5f, 0.5f},
            {1.0f, 1.0f, 1.0f},
            lightSource.pos()};

    const auto directionalLight = DirectionalLight{
            {-0.2f, -1.f, -0.3f},
            {0.2f, 0.2f, 0.2f},
            {0.5f, 0.5f, 0.5f},
            {1.0f, 1.0f, 1.0f}};

    const auto pointLight = PointLight{
            {0.f, 0.f, 0.f},
            {0.2f, 0.2f, 0.2f},
            {0.5f, 0.5f, 0.5f},
            {1.0f, 1.0f, 1.0f},
            Distance::D50};

    auto flashlight = Flashlight{
            {0.f, 0.f, 0.f},
            {0.f, 0.f, 0.f},
            {0.05f, 0.05f, 0.05f},
            {0.5f, 0.5f, 0.5f},
            {1.0f, 1.0f, 1.0f},
            glm::cos(glm::radians(12.5f)),
            glm::cos(glm::radians(17.5f))
    };

    shader.use();
    directionalLight.applyToShader(shader);
    pointLight.applyToShader(shader);
    flashlight.applyToShader(shader);

    instancedShader.use();
    directionalLight.applyToShader(instancedShader);
    pointLight.applyToShader(instancedShader);
    flashlight.applyToShader(instancedShader);

    // ---------------------------------------------------------------------------------------------------------------
    // Render loop
    //
    // Check to see if GLFW has been instructed to close the window, and if so break. An iteration of a render loop is
    // called a frame. Thus, a frame rate is how many times the render loop is executed in some period.
    // ---------------------------------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // ------------------------------------------------------------------------------------------------------------
        // Clear screen with a specified color. Otherwise, we would see the results from the last frame render as well.
        // This is usually not desired.
        // ------------------------------------------------------------------------------------------------------------
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const auto view = camera.view();
        const auto proj = camera.projection();

        // ------
        // Render
        // ------
        instancedShader.use();
        instancedShader.setMat4("view", view);
        instancedShader.setMat4("proj", proj);
        instancedShader.setInt("tex1", 0);
        instancedShader.setInt("tex2", 1);
        instancedShader.setMaterial(Materials::obsidian);

        flashlight.update(instancedShader, camera.position(), camera.front());

        lightSourceShader.use();
        lightSourceShader.setMat4("view", view);
        lightSourceShader.setMat4("proj", proj);
        lightSource.render(lightSourceShader);

        shader.use();
        shader.setInt("tex1", 0);
        shader.setInt("tex2", 1);
        shader.setMat4("view", view);
        shader.setMat4("proj", proj);
        shader.setMaterial(Materials::pearl);

        flashlight.update(shader, camera.position(), camera.front());

        container.render(shader);

        shader.setMaterial(Materials::chrome);
        texturedCube.render(camera, instancedShader);

        // -------------------------------------------------------------------------------------------------------
        // Swap the color buffer that is used to render to during this render and show it as output on the screen.
        // -------------------------------------------------------------------------------------------------------
        glfwSwapBuffers(window);

        // -----------------------------------------------
        // Poll for any events, like keyboard/mouse events
        // -----------------------------------------------
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

auto framebufferSizeCallback(GLFWwindow *, int width, int height) -> void {
    glViewport(0, 0, width, height);
}

auto cursorCallback(GLFWwindow *, double xPos, double yPos) -> void {
    camera.lookAt(static_cast<float>(xPos), static_cast<float>(yPos));
}

auto scrollCallback(GLFWwindow *, double, double yOffset) -> void {
    camera.zoom(static_cast<float>(yOffset));
}

auto processInput(GLFWwindow *window) -> void {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    camera.move(window);
}
