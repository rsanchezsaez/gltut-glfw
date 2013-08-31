//
//  main.cpp
//  gltut-glfw
//
//  Created by Ricardo Sánchez-Sáez on 15/08/2013.
//  Copyright (c) 2013 rsanchezsaez. All rights reserved.
//

#include <iostream>

#include "GLFW/glfw3.h"

#include "Scene.h"

const char* gltutglfwName = "gltut-glfw";


GLFWwindow* createWindow()
{
    glfwInit();
    
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    
    return glfwCreateWindow(1024, 640, gltutglfwName, NULL, NULL);
}

Scene scene = Scene();

const float aspectRatio = 1024.0f / 640.0f;
void onFramebufferResize(GLFWwindow* window, int width, int height)
{
    int finalWidth = width;
    int finalHeight = height;

    int derivedHeight = width * (1/aspectRatio);
    int derivedWidth = height * aspectRatio;
    
    if (derivedHeight <= height)
    {
        finalHeight = derivedHeight;
    }
    else if (derivedWidth <= width)
    {
        finalWidth = derivedWidth;
    }
    
    glViewport( (width-finalWidth)  / 2,
                (height-finalHeight) / 2,
                finalWidth,
                finalHeight);
    scene.draw();
    glfwSwapBuffers(window);

}

int main(int argc, const char * argv[])
{
    
    GLFWwindow* window = createWindow();
    glfwMakeContextCurrent(window);

    scene.init();
    glfwSetFramebufferSizeCallback(window, &onFramebufferResize);

    while (!glfwWindowShouldClose(window))
    {
        scene.draw();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}

