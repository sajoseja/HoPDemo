#include "SystemContext.h"

SystemContext::SystemContext() {
    wndId = 0;
    wndH = wndW = 500;

    motionFrameId = -1;
    while (!motionCtrl.isConnected());

    motionCtrl.enableGesture(Gesture::TYPE_SWIPE);
    motionCtrl.enableGesture(Gesture::TYPE_CIRCLE);
    motionCtrl.config().setFloat("Gesture.Swipe.MinLength", 1.0f);
    motionCtrl.config().setFloat("Gesture.Swipe.MinVelocity", 5.0f);
    motionCtrl.config().save();
}

int SystemContext::getWndId(void) const { return wndId; }
void SystemContext::setWndId(int _wndId) { wndId = _wndId; }

int SystemContext::getWndH(void) const { return wndH; }
void SystemContext::setWndH(int _wndH) { wndH = _wndH; }
int SystemContext::getWndW(void) const { return wndW; }
void SystemContext::setWndW(int _wndW) { wndW = _wndW; }

int SystemContext::getMotionFrameId(void) const { return motionFrameId; }
void SystemContext::setMotionFrameId(int _motionFrameId) { motionFrameId = _motionFrameId; }

Controller& SystemContext::getMotionCtrl(void) { return motionCtrl; }

const Object& SystemContext::getObj(int idx) const { return objs[idx]; }
void SystemContext::pushObj(const Object& obj) { objs.push_back(obj); }

Camera& SystemContext::getCam(int idx) { return cams[idx]; }
void SystemContext::pushCam(const Camera& cam) { cams.push_back(cam); }