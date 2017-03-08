#pragma once
#include <irrlicht.h>
using namespace irr;

class MyEventReceiver : public IEventReceiver
{
public:

    MyEventReceiver(scene::ISceneNode* fpsWorld, IrrlichtDevice *deviceFPS);

    bool OnEvent(const SEvent& event);

private:
    scene::ISceneNode* FPSWorld;
	IrrlichtDevice *DeviceFPS;
    bool showDebug;
	bool pause;
	
};