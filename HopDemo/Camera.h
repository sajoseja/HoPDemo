#pragma once

#include <gl/glut.h>

class Camera {

private:
    float pos[3];
    float uAxis[3];
    float vAxis[3];
    float nAxis[3];

    float radius, pi, theta, up;
    GLfloat mat[16];
    GLdouble fovy, aspect, nearC, farC;

    const GLdouble MIN_FOVY = 10.0;
    const GLdouble MAX_FOVY = 90.0;

public:
    Camera();
    Camera(float x, float y, float z);

    const float * getPos(void) const;
    void setPos(const float *_pos);

    const float * getUAxis(void) const;
    void setUAxis(const float *_uAxis);
    const float * getVAxis(void) const;
    void setVAxis(const float *_vAxis);
    const float * getNAxis(void) const;
    void setNAxis(const float *_nAxis);

    float getRadius(void) const;
    void setRadius(float _radius);
    float getPi(void) const;
    void setPi(float _pi);
    float getTheta(void) const;
    void setTheta(float _theta);
    float getUp(void) const;
    void setUp(float _up);

    const GLfloat * getMat(void) const;
    void setMat(const GLfloat *_mat);

    GLdouble getFovy(void) const;
    void setFovy(GLdouble _fovy);
    GLdouble getAspect(void) const;
    void setAspect(GLdouble _aspect);
    GLdouble getNearC(void) const;
    void setNearC(GLdouble _nearC);
    GLdouble getFarC(void) const;
    void setFarC(GLdouble _farC);

    void globalRotate(float _pi, float _theta);
};