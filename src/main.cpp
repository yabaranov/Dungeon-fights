#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <iostream>
#include <chrono>

#include "Game/Game.h"
#include "Resources/ResourceManager.h"

glm::ivec2 g_windowSize(640, 480);
Game g_game(g_windowSize);


void glfwWidowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSize.x = width;
    g_windowSize.y = height;
    glViewport(0, 0, width, height);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    g_game.setKey(key, action);
}

int main(int argc, char** argv)
{
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cerr << "glfwInit failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "Battle City", nullptr, nullptr);
    if (!pWindow)
    {
        std::cerr << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWidowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

	if(!gladLoadGL())
	{
        std::cerr << "Can't load GLAD!" << std::endl;
		return -1;
	}
	
    std::cout << "Renderer " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;

    glClearColor(0, 0, 0, 1);

    {
        ResourceManager::setExecutablePath(argv[0]);
        g_game.init();

        auto lastTime = std::chrono::high_resolution_clock::now();


        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {

            auto currentTime = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
            lastTime = currentTime;

            g_game.update(duration);
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            g_game.render();

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
        ResourceManager::unloadAllResourses();
    }
    glfwTerminate();
    return 0;
}