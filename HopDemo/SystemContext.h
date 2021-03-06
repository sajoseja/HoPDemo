#pragma once

#include <vector>

#include "Leap.h"
#include "Object.h"

using namespace Leap;

class SystemContext {

private:
    int wndId;                  ///< window id.
    int wndH, wndW;             ///< window size.
    
    int motionFrameId;          ///< motion frame id.
    Controller motionCtrl;      ///< motion controller.

    std::vector<Object> objs;   ///< objects to draw.

public:
    SystemContext();

    int getWndId(void) const;
    void setWndId(int _wndId);

    int getWndH(void) const;
    void setWndH(int _wndH);
    int getWndW(void) const;
    void setWndW(int _wndW);

    int getMotionFrameId(void) const;
    void setMotionFrameId(int _motionFrameId);

    Controller& getMotionCtrl(void);

    int getNumOfObjs(void) const;
    Object& getObj(int idx);
    void pushObj(const Object& obj);
};