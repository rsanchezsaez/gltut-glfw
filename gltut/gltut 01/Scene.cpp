//
//  Scene.cpp
//  gltut-glfw
//
//  Created by Ricardo Sánchez-Sáez on 16/08/2013.
//  Copyright (c) 2013 rsanchezsaez. All rights reserved.
//

#include "Scene.h"

#include <string>
#include <vector>

#include "debug.h"
#include "glhelpers.h"

const std::string vertexShaderString(
                                     "#version 330\n"
                                     " "
                                     "layout(location = 0) in vec4 inputPosition; "
                                     " "
                                     "void main() "
                                     "{ "
                                     "   gl_Position = inputPosition; "
                                     "} "
                                     );

const std::string fragmentShaderString(
                                       "#version 330\n"
                                       " "
                                       "out vec4 outputColor; "
                                       " "
                                       "void main() "
                                       "{ "
                                       "   outputColor = vec4( 1.0f, 1.0f, 1.0f, 1.0f ); "
                                       "} "
                                       );

const float vertexPositions[] =
{
	0.75f, 0.75f, 0.0f, 1.0f,
	0.75f, -0.75f, 0.0f, 1.0f,
	-0.75f, -0.75f, 0.0f, 1.0f
};

Scene::Scene()
{
}

void Scene::init()
{
    _shaderProgram = createShaderProgram(vertexShaderString, fragmentShaderString);
    glUseProgram(_shaderProgram);
    printOpenGLError();
    
    // Initialize Vertex Buffer
    glGenBuffers(1, &_positionBufferObject);
    
	glBindBuffer(GL_ARRAY_BUFFER, _positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    printOpenGLError();
    
    // Vertex array object
    glGenVertexArrays(1, &_vertexArrayObject);
	glBindVertexArray(_vertexArrayObject);
    printOpenGLError();
}

Scene::~Scene()
{
    glDeleteProgram(_shaderProgram);
    glDeleteBuffers(1, &_positionBufferObject);
    printOpenGLError();
}

void Scene::draw()
{
    glClearColor(0.2f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
	glBindBuffer(GL_ARRAY_BUFFER, _positionBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    
	glDrawArrays(GL_TRIANGLES, 0, 3);
    printOpenGLError();
    
	glDisableVertexAttribArray(0);
    printOpenGLError();
}