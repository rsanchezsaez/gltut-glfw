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

const float vertexData[] = {
    0.25f,  0.25f, 0.75f, 1.0f,
    0.25f, -0.25f, 0.75f, 1.0f,
	-0.25f,  0.25f, 0.75f, 1.0f,
    
    0.25f, -0.25f, 0.75f, 1.0f,
	-0.25f, -0.25f, 0.75f, 1.0f,
	-0.25f,  0.25f, 0.75f, 1.0f,
    
    0.25f,  0.25f, -0.75f, 1.0f,
	-0.25f,  0.25f, -0.75f, 1.0f,
    0.25f, -0.25f, -0.75f, 1.0f,
    
    0.25f, -0.25f, -0.75f, 1.0f,
	-0.25f,  0.25f, -0.75f, 1.0f,
	-0.25f, -0.25f, -0.75f, 1.0f,
    
	-0.25f,  0.25f,  0.75f, 1.0f,
	-0.25f, -0.25f,  0.75f, 1.0f,
	-0.25f, -0.25f, -0.75f, 1.0f,
    
	-0.25f,  0.25f,  0.75f, 1.0f,
	-0.25f, -0.25f, -0.75f, 1.0f,
	-0.25f,  0.25f, -0.75f, 1.0f,
    
    0.25f,  0.25f,  0.75f, 1.0f,
    0.25f, -0.25f, -0.75f, 1.0f,
    0.25f, -0.25f,  0.75f, 1.0f,
    
    0.25f,  0.25f,  0.75f, 1.0f,
    0.25f,  0.25f, -0.75f, 1.0f,
    0.25f, -0.25f, -0.75f, 1.0f,
    
    0.25f,  0.25f, -0.75f, 1.0f,
    0.25f,  0.25f,  0.75f, 1.0f,
	-0.25f,  0.25f,  0.75f, 1.0f,
    
    0.25f,  0.25f, -0.75f, 1.0f,
	-0.25f,  0.25f,  0.75f, 1.0f,
	-0.25f,  0.25f, -0.75f, 1.0f,
    
    0.25f, -0.25f, -0.75f, 1.0f,
	-0.25f, -0.25f,  0.75f, 1.0f,
    0.25f, -0.25f,  0.75f, 1.0f,
    
    0.25f, -0.25f, -0.75f, 1.0f,
	-0.25f, -0.25f, -0.75f, 1.0f,
	-0.25f, -0.25f,  0.75f, 1.0f,
    
    
    
    
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
    
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
    
	0.8f, 0.8f, 0.8f, 1.0f,
	0.8f, 0.8f, 0.8f, 1.0f,
	0.8f, 0.8f, 0.8f, 1.0f,
    
	0.8f, 0.8f, 0.8f, 1.0f,
	0.8f, 0.8f, 0.8f, 1.0f,
	0.8f, 0.8f, 0.8f, 1.0f,
    
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
    
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
    
	0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f,
    
	0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f,
    
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
    
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
    
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
    
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
    
};

GLuint offsetUniform;

Scene::Scene()
{
}

void Scene::init()
{
    _shaderProgram = createShaderProgramWithFilenames("OrthoWithOffset.vert", "StandardColors.frag");
    glUseProgram(_shaderProgram);
    printOpenGLError();
    
    // Initialize Vertex Buffer
    glGenBuffers(1, &_vertexBufferObject);
    
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    printOpenGLError();
    
    // Vertex array object
    glGenVertexArrays(1, &_vertexArrayObject);
	glBindVertexArray(_vertexArrayObject);
    printOpenGLError();

    // Enable cull facing
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    // Uniforms
	offsetUniform = glGetUniformLocation(_shaderProgram, "offset");
    
}

Scene::~Scene()
{
    glDeleteProgram(_shaderProgram);
    glDeleteBuffers(1, &_vertexBufferObject);
    printOpenGLError();
}

const float aspectRatio = 1.0f;
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
    
    glUniform2f(offsetUniform, 0.5f, 0.25f);

	size_t colorData = sizeof(vertexData) / 2;
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorData);
    printOpenGLError();

	glDrawArrays(GL_TRIANGLES, 0, 36);
    printOpenGLError();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
    printOpenGLError();
}

void Scene::keyStateChanged(int key, int action)
{
}