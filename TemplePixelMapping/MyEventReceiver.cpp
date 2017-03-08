#include "MyEventReceiver.h"

   MyEventReceiver::MyEventReceiver(scene::ISceneNode* fpsWorld, IrrlichtDevice *deviceFPS) :
        FPSWorld(fpsWorld), showDebug(false),pause(false),DeviceFPS(deviceFPS)
    {

    }

   bool MyEventReceiver::OnEvent(const SEvent& event)
    {
        // check if user presses the key 'W' or 'D'
        if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
        {
            switch (event.KeyInput.Key)
            {
            case irr::KEY_F1: // switch wire frame mode
                FPSWorld->setMaterialFlag(video::EMF_WIREFRAME,
                        !FPSWorld->getMaterial(0).Wireframe);
                FPSWorld->setMaterialFlag(video::EMF_POINTCLOUD, false);
                return true;
            case irr::KEY_F2: // switch PointCloud mode
                FPSWorld->setMaterialFlag(video::EMF_POINTCLOUD,
                        !FPSWorld->getMaterial(0).PointCloud);
                FPSWorld->setMaterialFlag(video::EMF_WIREFRAME, false);
                return true;
			//Touraj: Pausing The Game
			case irr::KEY_KEY_P:
				pause =!pause;
				DeviceFPS->getTimer()->setSpeed(pause?0.0f:1.0f);
				return true;
          
            case irr::KEY_KEY_X: // toggle debug information
                showDebug=!showDebug;
                FPSWorld->setDebugDataVisible(showDebug?scene::EDS_BBOX_ALL:scene::EDS_OFF);
				//DeviceFPS->getTimer()->setSpeed(0);
                return true;
            default:
                break;
            }
        }

        return false;
    }