#include <gl/glut.h>
#include <cmath>
#include <cstdio>
#include "Camera.h"

#define PI 3.141592653589793f

Camera::Camera()
    : Camera(60.0f, 60.0f, 20.0f)
{}

Camera::Camera(float x, float y, float z) {

    GLfloat _mat[16];

    pos[0] = x;
    pos[1] = y;
    pos[2] = z;

    radius = sqrt(
        pos[0] * pos[0] +
        pos[1] * pos[1] +
        pos[2] * pos[2]);
    pi = (float)atan2(pos[1], pos[0]);
    theta = (float)acos(pos[2] / radius);
    up = 1.0f;

    fovy = 45.0f;
    aspect = 1.0f;
    nearC = 1.0f;
    farC = 1000.0f;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    gluLookAt(
        (double)pos[0], (double)pos[1], (double)pos[2],
        (double)0.0f, (double)0.0f, (double)0.0f,
        (double)0.0f, (double)0.0f, (double)up);
    glGetFloatv(GL_MODELVIEW_MATRIX, _mat);
    glPopMatrix();

    uAxis[0] = _mat[0];
    uAxis[1] = _mat[4];
    uAxis[2] = _mat[8];
    vAxis[0] = _mat[1];
    vAxis[1] = _mat[5];
    vAxis[2] = _mat[9];
    nAxis[0] = _mat[2];
    nAxis[1] = _mat[6];
    nAxis[2] = _mat[10];

#define M(row,col) mat[col * 4 + row]
    M(0, 0) = uAxis[0]; M(0, 1) = uAxis[1]; M(0, 2) = uAxis[2]; M(0, 3) = 0.0f;
    M(1, 0) = vAxis[0]; M(1, 1) = vAxis[1]; M(1, 2) = vAxis[2]; M(1, 3) = 0.0f;
    M(2, 0) = nAxis[0]; M(2, 1) = nAxis[1]; M(2, 2) = nAxis[2]; M(2, 3) = 0.0f;
    M(3, 0) = 0.0f;     M(3, 1) = 0.0f;     M(3, 2) = 0.0f;     M(3, 3) = 1.0f;
}

const float * Camera::getPos(void) const { return pos; }
void Camera::setPos(const float *_pos) { pos[0] = _pos[0]; pos[1] = _pos[1]; pos[2] = _pos[2]; }

const float * Camera::getUAxis(void) const { return uAxis; }
void Camera::setUAxis(const float *_uAxis) { uAxis[0] = _uAxis[0]; uAxis[1] = _uAxis[1]; uAxis[2] = _uAxis[2]; }
const float * Camera::getVAxis(void) const { return vAxis; }
void Camera::setVAxis(const float *_vAxis) { vAxis[0] = _vAxis[0]; vAxis[1] = _vAxis[1]; vAxis[2] = _vAxis[2]; }
const float * Camera::getNAxis(void) const { return nAxis; }
void Camera::setNAxis(const float *_nAxis) { nAxis[0] = _nAxis[0]; nAxis[1] = _nAxis[1]; nAxis[2] = _nAxis[2]; }

float Camera::getRadius(void) const { return radius; }
void Camera::setRadius(float _radius) { radius = _radius; }
float Camera::getPi(void) const { return pi; }
void Camera::setPi(float _pi) { pi = _pi; }
float Camera::getTheta(void) const { return theta; }
void Camera::setTheta(float _theta) { theta = _theta; }
float Camera::getUp(void) const { return up; }
void Camera::setUp(float _up) { up = _up; }

const GLfloat * Camera::getMat(void) const { return mat; }
void Camera::setMat(const GLfloat *_mat) {
    for (int i = 0; i < 16; ++i)
        mat[i] = _mat[i];
}

GLdouble Camera::getFovy(void) const { return fovy; }
void Camera::setFovy(GLdouble _fovy) { fovy = (MIN_FOVY <= _fovy && _fovy <= MAX_FOVY) ? _fovy : fovy; }
GLdouble Camera::getAspect(void) const { return aspect; }
void Camera::setAspect(GLdouble _aspect) { aspect = _aspect; }
GLdouble Camera::getNearC(void) const { return nearC; }
void Camera::setNearC(GLdouble _nearC) { nearC = _nearC; }
GLdouble Camera::getFarC(void) const { return farC; }
void Camera::setFarC(GLdouble _farC) { farC = _farC; }

void Camera::globalRotate(float _pi, float _theta) {

    GLfloat _mat[16];

    pi += _pi;
    pi += (pi < 0.0f) ? 2.0f * PI : 0.0f;
    pi -= (pi > 2.0f * PI) ? 2.0f * PI : 0.0f;

    float tmp = theta + up * _theta;
    if (tmp < 0.0f) {
        up *= -1.0f;
        theta = -tmp;
    }
    else if (tmp > PI) {
        up *= -1.0f;
        theta = 2 * PI - tmp;
    }
    else
        theta = tmp;
  
    pos[0] = radius * sin(theta) * cos(pi);
    pos[1] = radius * sin(theta) * sin(pi);
    pos[2] = radius * cos(theta);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    gluLookAt(
        (double)pos[0], (double)pos[1], (double)pos[2],
        (double)0.0f, (double)0.0f, (double)0.0f,
        (double)0.0f, (double)0.0f, (double)up);
    glGetFloatv(GL_MODELVIEW_MATRIX, _mat);
    glPopMatrix();

    uAxis[0] = _mat[0];
    uAxis[1] = _mat[4];
    uAxis[2] = _mat[8];
    vAxis[0] = _mat[1];
    vAxis[1] = _mat[5];
    vAxis[2] = _mat[9];
    nAxis[0] = _mat[2];
    nAxis[1] = _mat[6];
    nAxis[2] = _mat[10];

#define M(row,col) mat[col * 4 + row]
    M(0, 0) = uAxis[0]; M(0, 1) = uAxis[1]; M(0, 2) = uAxis[2]; M(0, 3) = 0.0f;
    M(1, 0) = vAxis[0]; M(1, 1) = vAxis[1]; M(1, 2) = vAxis[2]; M(1, 3) = 0.0f;
    M(2, 0) = nAxis[0]; M(2, 1) = nAxis[1]; M(2, 2) = nAxis[2]; M(2, 3) = 0.0f;
    M(3, 0) = 0.0f;     M(3, 1) = 0.0f;     M(3, 2) = 0.0f;     M(3, 3) = 1.0f;
}