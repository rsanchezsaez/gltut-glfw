//
//  Scene.h
//  gltut-glfw
//
//  Created by Ricardo Sánchez-Sáez on 16/08/2013.
//  Copyright (c) 2013 rsanchezsaez. All rights reserved.
//

#ifndef __gltut_glfw__Scene__
#define __gltut_glfw__Scene__

#include <OpenGL/gl3.h>

#include <vector>

class Scene {
    GLuint _shaderProgram;
    GLuint _positionBufferObject;
    GLuint _vertexArrayObject;

  public:
    Scene();
    ~Scene();
    
    void init();
    void draw();
};


#endif /* defined(__circular__Scene__) */
