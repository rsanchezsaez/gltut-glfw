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
    GLuint _positionBufferObject;
    GLuint _vertexArrayObject;
    
  public:
    Scene();
    ~Scene();
    
    void init();
    void draw();
    
    void computePositionOffsets(float &fXOffset, float &fYOffset);
    void adjustVertexData(float fXOffset, float fYOffset);
};


#endif /* defined(__circular__Scene__) */
