//
//  Scene.h
//  gltut-glfw
//
// Copyright (C) 2010-2012 by Jason L. McKesson
// Xcode and glfw adaptation: Ricardo Sánchez-Sáez.
//
// This file is licensed under the MIT License.
//

#ifndef __gltut_glfw__Scene__
#define __gltut_glfw__Scene__

#include <OpenGL/gl3.h>

#include <vector>

class Scene {
    GLuint _shaderProgram;
    GLuint _vertexBufferObject;
    GLuint _vertexArrayObject;
    
  public:
    Scene();
    ~Scene();
    
    void init();
    void draw();

    void reshape(int width, int height);
    
    // Onlt used in Chapter 2
    void computePositionOffsets(float &fXOffset, float &fYOffset);
    void adjustVertexData(float fXOffset, float fYOffset);
};


#endif /* defined(__circular__Scene__) */
