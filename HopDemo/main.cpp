#include <gl/glut.h>

#include "SystemContext.h"

// context.
SystemContext SC;

// callbacks.
void idleCallback(void);
void displayCallback(void);
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
    SC.pushObj(Object("cube.txt"));
    SC.pushCam(Camera(15.0f, 15.0f, 10.0f));

    // register callbacks.
    glutIdleFunc(idleCallback);
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);

    // rendering setting.
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(SC.getCam(0).getMat());
    glTranslatef(
        -SC.getCam(0).getPos()[0],
        -SC.getCam(0).getPos()[1],
        -SC.getCam(0).getPos()[2]);

    // main loop.
    glutMainLoop();
}

// idle callback.
void idleCallback(void) {
    
    Frame frame = SC.getMotionCtrl().frame();
    if (frame.id() == SC.getMotionFrameId())
        return;
    SC.setMotionFrameId(frame.id());

    const float SWIPE_SENSE = 0.01;
    const float CIRCLE_SENSE = 0.05;

    GLdouble startFovy = SC.getCam(0).getFovy();

    GestureList::const_iterator it;
    GestureList gestures = frame.gestures();
    for (it = gestures.begin(); it != gestures.end(); ++it) {

        if ((*it).type() == Gesture::TYPE_SWIPE) {
            SwipeGesture swipe = (*it);
            Vector cur = swipe.position();
            Vector pre = swipe.startPosition();

            float delx = cur.x - pre.x;
            float dely = cur.y - pre.y;

            SC.getCam(0).globalRotate(
                SWIPE_SENSE * (delx * PI) / 180.0,
                SWIPE_SENSE * (dely * PI) / 180.0);
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glMultMatrixf(SC.getCam(0).getMat());
            glTranslatef(
                -SC.getCam(0).getPos()[0],
                -SC.getCam(0).getPos()[1],
                -SC.getCam(0).getPos()[2]);
            glutPostRedisplay();
        }
        else if (
            (*it).type() == Gesture::TYPE_SWIPE &&
            (*it).state() == Gesture::STATE_UPDATE) {
            SwipeGesture swipe = (*it);
            Vector currentPosition = swipe.position();
            std::cout << currentPosition.x << " " << currentPosition.y << " " << currentPosition.z << std::endl;
        }
        else if (
            (*it).type() == Gesture::TYPE_CIRCLE &&
            (*it).state() == Gesture::STATE_START) {
            startFovy = SC.getCam(0).getFovy();
        }
        else if (
            (*it).type() == Gesture::TYPE_CIRCLE &&
            (*it).state() == Gesture::STATE_UPDATE) {
            CircleGesture circle = (*it);
            if (circle.pointable().direction().angleTo(circle.normal()) <= PI / 2)
                SC.getCam(0).setFovy(startFovy + CIRCLE_SENSE * circle.progress());
            else
                SC.getCam(0).setFovy(startFovy - CIRCLE_SENSE * circle.progress());

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(
                SC.getCam(0).getFovy(), SC.getCam(0).getAspect(),
                SC.getCam(0).getNearC(), SC.getCam(0).getFarC());
            glutPostRedisplay();
        }
    }
}

// display callback.
void displayCallback(void) {

    const float colors[6][3] = {
        { 1.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 0.0f },
        { 1.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f }
    };

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    Object obj = SC.getObj(0);
    for (int i = 0; i < obj.getNFace(); ++i) {
        Polygon face = obj.getFace(i);
        glLineWidth(3.0f);
        glColor3f(colors[i][0], colors[i][1], colors[i][2]);
        
        glBegin(GL_LINE_LOOP);
        for (int j = 0; j < face.nVtx; ++j)
            glVertex3f(face.polygon[j][0], face.polygon[j][1], face.polygon[j][2]);
        glEnd();
    }
    
    glutSwapBuffers();
}

// reshape callback.
void reshapeCallback(int x, int y) {

    SC.setWndW(x);
    SC.setWndH(y);

    glViewport(0, 0, x, y);

    SC.getCam(0).setAspect((GLdouble)x / y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        SC.getCam(0).getFovy(), SC.getCam(0).getAspect(),
        SC.getCam(0).getNearC(), SC.getCam(0).getFarC());
}