#include <time.h>
#include <gl/glut.h>
#include <algorithm>

#include "hop.h"
#include "SystemContext.h"

// context.
SystemContext SC;

// callbacks.
void idleCallback(void);
void displayCallback(HopSide side);
void reshapeCallback(int x, int y);

// main.
void main(int argc, char **argv) {

    // glut initialize.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    
    // system context setting.
    SC.setWndW(700);
    SC.setWndH(700);
    glutInitWindowSize(SC.getWndW(), SC.getWndH());
    SC.setWndId(glutCreateWindow("HopDemo"));
    SC.pushObj(Object(
        "Model/ahri/b_ahri_b.obj",
        "Model/ahri/b_ahri_b.mtl"));

    // register callbacks.
    glutIdleFunc(idleCallback);
    Hop::setDisplayFunc(displayCallback);
    Hop::setReshapeFunc(reshapeCallback);
    Hop::setSize(1000.0f);

    // rendering setting.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // main loop.
    glutMainLoop();
}

// idle callback.
void idleCallback(void) {
    
    Frame frame = SC.getMotionCtrl().frame();
    if (frame.id() == SC.getMotionFrameId())
        return;
    SC.setMotionFrameId(frame.id());

    const float SWIPE_SENSE = 0.001;
    const float CIRCLE_SENSE = 0.05;

    float startSize = Hop::getSize();

    GestureList::const_iterator it;
    GestureList gestures = frame.gestures();
    for (it = gestures.begin(); it != gestures.end(); ++it) {

        if ((*it).type() == Gesture::TYPE_SWIPE) {
            SwipeGesture swipe = (*it);
            Vector cur = swipe.position();
            Vector pre = swipe.startPosition();

            float delx = cur.x - pre.x;
            float dely = cur.y - pre.y;

            for (int i = 0; i < SC.getNumOfObjs(); ++i) {
                Object& obj = SC.getObj(i);

                float _pi = obj.getPi() + SWIPE_SENSE * delx;
                float _theta = obj.getTheta() + SWIPE_SENSE * dely;
                _pi = (_pi >= 360.0f) ? _pi - 360.0f : _pi;
                _theta = (_theta >= 360.0f) ? _theta - 360.0f : _theta;

                obj.setPi(_pi);
                obj.setTheta(_theta);
            }

            glutPostRedisplay();
        }
        else if (
            (*it).type() == Gesture::TYPE_SWIPE &&
            (*it).state() == Gesture::STATE_UPDATE) {
            SwipeGesture swipe = (*it);
            Vector currentPosition = swipe.position();
        }
        else if (
            (*it).type() == Gesture::TYPE_CIRCLE &&
            (*it).state() == Gesture::STATE_START) {
            startSize = Hop::getSize();
        }
        else if (
            (*it).type() == Gesture::TYPE_CIRCLE &&
            (*it).state() == Gesture::STATE_UPDATE) {
            CircleGesture circle = (*it);
            if (circle.pointable().direction().angleTo(circle.normal()) <= PI / 2)
                Hop::setSize(std::max(startSize - CIRCLE_SENSE * circle.progress(), 1.0f));
            else
                Hop::setSize(std::max(startSize + CIRCLE_SENSE * circle.progress(), 1.0f));

            glutPostRedisplay();
        }
    }
}

// display callback.
void displayCallback(HopSide side) {

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    float matProj[16];
    side.getProjectionMatrix(matProj);
    glLoadMatrixf(matProj);

    glMatrixMode(GL_MODELVIEW);
    float matView[16];
    side.getViewMatrix(matView);
    glLoadMatrixf(matView);
    
    for (int i = 0; i < SC.getNumOfObjs(); ++i) {
        Object& obj = SC.getObj(i);
        
        glPushMatrix();
        glRotatef(obj.getPi(), 0.0f, 1.0f, 0.0f);
        glRotatef(obj.getTheta(), 1.0f, 0.0f, 0.0f);
        obj.draw(1.0f);
        glPopMatrix();
    }

    glutSwapBuffers();
}

// reshape callback.
void reshapeCallback(int x, int y) {

    SC.setWndW(x);
    SC.setWndH(y);

    glViewport(0, 0, x, y);
}