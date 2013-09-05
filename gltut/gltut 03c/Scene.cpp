//
//  Scene.cpp
//  gltut-glfw
//
// Copyright (C) 2010-2012 by Jason L. McKesson
// Xcode and glfw adaptation: Ricardo Sánchez-Sáez.
//
// This file is licensed under the MIT License.
//

#include "Scene.h"

#include <string>
#include <vector>

#include "debug.h"
#include "glhelpers.h"
#include "GLFW/glfw3.h"

#include <math.h>

const float vertexPositions[] = {
	0.25f, 0.25f, 0.0f, 1.0f,
	0.25f, -0.25f, 0.0f, 1.0f,
	-0.25f, -0.25f, 0.0f, 1.0f,
};

GLuint elapsedTimeUniform;

Scene::Scene()
{
}

void Scene::init()
{
    _shaderProgram = createShaderProgramWithFilenames("calcOffset.vert", "standard.frag");
    glUseProgram(_shaderProgram);
    printOpenGLError();
    
    // Initialize Vertex Buffer
    glGenBuffers(1, &_vertexBufferObject);
    
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    printOpenGLError();
    
    // Vertex array object
    glGenVertexArrays(1, &_vertexArrayObject);
	glBindVertexArray(_vertexArrayObject);
    printOpenGLError();

    // Uniforms
    elapsedTimeUniform = glGetUniformLocation(_shaderProgram, "time");
    
	GLuint loopDurationUniform = glGetUniformLocation(_shaderProgram, "loopDuration");
	glUniform1f(loopDurationUniform, 5.0f);
    printOpenGLError();
}

Scene::~Scene()
{
    glDeleteProgram(_shaderProgram);
    glDeleteBuffers(1, &_vertexBufferObject);
    printOpenGLError();
}

const float aspectRatio = 1024.0f / 640.0f;
void Scene::reshape(int width, int height)
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

    this->draw();
}

void Scene::draw()
{
    glClearColor(0.2f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
	glUniform1f(elapsedTimeUniform, (float)glfwGetTime());
    printOpenGLError();

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    
	glDrawArrays(GL_TRIANGLES, 0, 3);
    printOpenGLError();
    
	glDisableVertexAttribArray(0);
    printOpenGLError();
}

void Scene::keyStateChanged(int key, int action)
{
}