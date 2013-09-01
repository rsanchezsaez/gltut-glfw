//
//  main.cpp
//  gltut-glfw
//
// Copyright (C) 2010-2012 by Jason L. McKesson
// Xcode and glfw adaptation: Ricardo Sánchez-Sáez.
//
// This file is licensed under the MIT License.
//

#include <iostream>

#include "GLFW/glfw3.h"

#include "Scene.h"

const char* gltutglfwName = "gltut-glfw";

static void onError(int error, const char* description)
{
    std::cout << "Error: " << description << std::endl;
}

GLFWwindow* createWindow()
{
    if (!glfwInit()) {
        return NULL;
    }
    
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    
    return glfwCreateWindow(1024, 640, gltutglfwName, NULL, NULL);
}

Scene scene = Scene();

void onFramebufferResize(GLFWwindow* window, int width, int height)
{
    scene.reshape(width, height);
    glfwSwapBuffers(window);

}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static bool depthClampingActive = false;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        if(depthClampingActive) {
            glDisable(GL_DEPTH_CLAMP);
        } else {
            glEnable(GL_DEPTH_CLAMP);
        }
        depthClampingActive = !depthClampingActive;
    }
}

int main(int argc, const char * argv[])
{
    glfwSetErrorCallback(onError);
    
    GLFWwindow* window = createWindow();
    if (!window)
        return 0;

    glfwMakeContextCurrent(window);

    scene.init();
    
    int windowWidth = 0;
    int windowHeight = 0;
    
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    onFramebufferResize(window, windowWidth, windowHeight);
    glfwSetFramebufferSizeCallback(window, &onFramebufferResize);
    
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window))
    {
        scene.draw();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}

