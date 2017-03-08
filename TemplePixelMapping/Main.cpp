/** 
Touraj Ebrahimi
FPS 0.1
*/
#include <irrlicht.h>
#include <iostream>
#include <fstream>

// Touraj Developed Header Files
 #include "RenderConstants.h"
#include "Tools.h"
#include "MyEventReceiver.h"
//#include "CCloudSceneNode.h";


using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


//#ifdef _MSC_VER
//#pragma comment(lib, "Irrlicht.lib")
//#endif

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup") //Touraj: Hide console of Windows Application
#endif


int main()
{
	enum EObjectsPositions
	{
		CameraPosition = 1001,
	
	};

	// Touraj: I have Put rendering constants in the RenderConsts Struct
	RenderConsts Rconsts; // Touraj: SIrrlichtCreationParameters deviceParam; also Can Be Used
	
	IrrlichtDevice *device =
		createDevice( video::EDT_DIRECT3D9, 
			dimension2d<u32>(Rconsts.width, Rconsts.height), 
			Rconsts.pixelBits,
			Rconsts.fullscreen,
			Rconsts.stencilBuffer,
			Rconsts.vSync,
			0); // 1366, 768

	if (!device)
		return 1;

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* env = device->getGUIEnvironment();

	//Touraj: Should be 32 bits for paralex mapping
	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

	//// add irrlicht logo
	//env->addImage(driver->getTexture("../../../media/irrlichtlogo3.png"),
	//	core::position2d<s32>(100,10));

	// Add Camera
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(
		NULL, // Parrent
		75.0f, // Rotation Speed
		0.25f, // Movement Speed
		-1, // Id of the node
		NULL, // KeyMap
		0, //KeyMap Size
		false, // No Vertical Movement
		0.0f, // Jump Speed
		false, // invertMouse
		true // Make Active
		);

	camera->setPosition(core::vector3df(0,200,0)); //-200,200,-200
	camera->setTarget(core::vector3df(0,200,200));
	camera->setID(EObjectsPositions::CameraPosition);
	//smgr->createCollisionResponseAnimator(

	/* Touraj: Useful Info for Collision Detection from irrlicht engine
	ellipsoidRadius,:

	Radius of the ellipsoid with which collision detection and response is done. If you have got a scene node, and you are unsure about how big the radius should be, you could use the following code to determine it: 
        const core::aabbox3d<f32>& box = yourSceneNode->getBoundingBox();
        core::vector3df radius = box.MaxEdge - box.getCenter();
	*/
 

	// camera->setTarget(core::vector3df(-70,730,45));

	// disable mouse cursor
	device->getCursorControl()->setVisible(false);
	
	// Touraj: Fog
	driver->setFog(video::SColor(0,138,125,81), video::EFT_FOG_EXP, 1000, 2300, .001f, true, false);

	scene::IMesh* roomMesh = smgr->getMesh("Objects/Temple4.obj");// ../../../media/room.3ds
	scene::IMeshSceneNode* room = 0; // Touraj: ISceneNode Revised to IMeshSceneNode
	scene::IMeshSceneNode* earth = 0;

	if (roomMesh)
	{
		// The Room mesh doesn't have proper Texture Mapping on the
		// floor, so we can recreate them on runtime
		//smgr->getMeshManipulator()->makePlanarTextureMapping(roomMesh->getMesh(0), 0.05f);


		video::ITexture* normalMap =
			driver->getTexture("Objects/normal.tga"); // ../../../media/rockwall_height.bmp

		//if (normalMap)
		//	driver->makeNormalMapTexture(normalMap, 9.0f);
/*
		// The Normal Map and the displacement map/height map in the alpha channel
		video::ITexture* normalMap =
			driver->getTexture("../../../media/rockwall_NRM.tga");
*/

		scene::IMesh* tangentMesh = smgr->getMeshManipulator()->
				createMeshWithTangents(roomMesh); // roomMesh->getMesh(0)

// Log Tof file
		
			//std::ofstream myfile;
			//myfile.open ("log.txt", std::ios::out | std::ios::app); // ,std::ios_base
			//myfile << "Log [Begin].\n";
			//myfile << "Frame Count: " << roomMesh->getFrameCount() << std::endl;
			//myfile << "Log [End].\n";
			//myfile.close();
			//int farmeCount = roomMesh->getFrameCount(); // irr::u32 to int
			//Tools::WriteToFile("outLog.txt","Farme Count is: " + std::to_string(farmeCount));


		room = smgr->addMeshSceneNode(tangentMesh);
		// room->setScale(vector3df(10,10,10));
		room->setPosition(vector3df(0,300,0));
		//room->setMaterialTexture(1,
		//		driver->getTexture("../../../media/rockwall.jpg"));
		room->setMaterialTexture(1, normalMap);

		// TourajL: if Stones coz don't glitter make these params to zero..
		room->getMaterial(0).SpecularColor.set(255,0,0,255);
		room->getMaterial(0).Shininess = 120.f;

		room->setMaterialFlag(video::EMF_LIGHTING, false);
		room->setMaterialFlag(video::EMF_FOG_ENABLE, true);
		room->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
		room->setMaterialType(video::EMT_NORMAL_MAP_SOLID); //EMT_PARALLAX_MAP_SOLID //EMT_NORMAL_MAP_SOLID
		// adjust height for parallax effect
		//room->getMaterial(0).MaterialTypeParam = 1.f / 64.f;

		// drop mesh because we created it with a create.. call.
		
		tangentMesh->drop();
	}

	// add earth sphere

	scene::IMesh* earthMesh = smgr->getMesh("../../../media/earth.x");
	if (earthMesh)
	{
		//perform various task with the mesh manipulator
		scene::IMeshManipulator *manipulator = smgr->getMeshManipulator();

		// create mesh copy with tangent informations from original earth.x mesh
		scene::IMesh* tangentSphereMesh =
			manipulator->createMeshWithTangents(earthMesh); // earthMesh->getMesh(0) should be used for IAnimatedMesh

		// set the alpha value of all vertices to 200
		//manipulator->setVertexColorAlpha(tangentSphereMesh, 200);

		// scale the mesh by factor 50
		core::matrix4 m;
		m.setScale ( core::vector3df(50,50,50) );
		manipulator->transform( tangentSphereMesh, m );

		earth = smgr->addMeshSceneNode(tangentSphereMesh);

		earth->setPosition(core::vector3df(-270,110,0));

		// load heightmap, create normal map from it and set it
		video::ITexture* earthNormalMap = driver->getTexture("../../../media/earthbump.jpg");
		if (earthNormalMap)
		{
			driver->makeNormalMapTexture(earthNormalMap, 20.0f);
			earth->setMaterialTexture(1, earthNormalMap);
			earth->setMaterialType(video::EMT_NORMAL_MAP_SOLID); //EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA
		}

		// adjust material settings
		earth->setMaterialFlag(video::EMF_FOG_ENABLE, true);
		earth->setMaterialFlag(video::EMF_LIGHTING, true);
		earth->setMaterialFlag(video::EMF_GOURAUD_SHADING, true);
		//earth->setScale(vector3df(2,2,2));

		// add Real time shadow Casting To Earth
			 earth->addShadowVolumeSceneNode();
			earth->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);


		// add rotation animator
		scene::ISceneNodeAnimator* anim =
			smgr->createRotationAnimator(core::vector3df(0,0.1f,0));
		earth->addAnimator(anim);
		anim->drop();

		// drop mesh because we created it with a create.. call.
		tangentSphereMesh->drop();
	}

///////////////////////////// Add Jessada Sci-FI face [Begin]
	
	scene::IAnimatedMesh* jessada = smgr->getMesh("Objects/jessada.obj");
	if (!jessada)
	{
		device->drop();
		return 1;
	}
	scene::IAnimatedMeshSceneNode* jessadaNode = smgr->addAnimatedMeshSceneNode(jessada);

	if (jessadaNode)
	{
		//jessadaNode->setMaterialTexture( 0, driver->getTexture("Objects/tex.jpg") );
		jessadaNode->setMaterialFlag(EMF_LIGHTING, false);
		jessadaNode->setMaterialType(EMT_SOLID);
		jessadaNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
		jessadaNode->setScale(vector3df(4,4,4)); // Scale of the Sphere

		jessadaNode->setPosition(vector3df(200,200,-30)); // 0,150,0
		//core::vector3df camPos = camera->getPosition();
		//camPos.Z=+150;
		//camPos.Y-=40;
		//jessadaNode->setPosition(camPos);
		//jessadaNode->setParent(camera);

		jessadaNode->setRotation(core::vector3df(0,90,0));
	}

///////////////////////////// Add Jessada Sci-FI face [End]

	///////////////////////////// Add Barzeghan [Begin]
	
	scene::IAnimatedMesh* barzeghan = smgr->getMesh("Objects/Barzeghan.obj");
	if (!barzeghan)
	{
		device->drop();
		return 1;
	}
	//scene::IAnimatedMeshSceneNode* barzeghanNode = smgr->addAnimatedMeshSceneNode(barzeghan);

		scene::IMesh* tangentbarzeghan = smgr->getMeshManipulator()->
				createMeshWithTangents(barzeghan->getMesh(0));

	scene::IMeshSceneNode* barzeghanNode = smgr->addMeshSceneNode(tangentbarzeghan);

	if (barzeghanNode)
	{
		//barzeghanNode->setMaterialTexture( 0, driver->getTexture("Objects/tex.jpg") );
		barzeghanNode->setMaterialFlag(EMF_LIGHTING, false);
		//barzeghanNode->setMaterialType(EMT_SOLID);
		barzeghanNode->setMaterialTexture( 1, driver->getTexture("Objects/BigUrnNormal.png") );//Normal Map
		barzeghanNode->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
		barzeghanNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);

		//barzeghanNode->setScale(vector3df(4,4,4)); 
		room->getPosition();
		barzeghanNode->setPosition(room->getPosition()); // 200,200,-30
		//core::vector3df camPos = camera->getPosition();
		//camPos.Z=+150;
		//camPos.Y-=40;
		//barzeghanNode->setPosition(camPos);
		//barzeghanNode->setParent(camera);

		barzeghanNode->setRotation(core::vector3df(0,90,0));
	}

	// add light for Barzeghan 
	scene::ILightSceneNode* lightBarzeghan =
		smgr->addLightSceneNode(0, core::vector3df(-1000,300,0),
		video::SColorf(0.7f, 0.6f, 0.5f, 1.0f), 500.0f);

		// add Real time shadow Casting To barzeghanNode
	/*		 barzeghanNode->addShadowVolumeSceneNode();
			barzeghanNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);*/

	// attach billboard to the light
	scene::IBillboardSceneNode* billBarzeghan =
		smgr->addBillboardSceneNode(lightBarzeghan, core::dimension2d<f32>(60, 60));

	billBarzeghan->setMaterialFlag(video::EMF_LIGHTING, false);
	billBarzeghan->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	billBarzeghan->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	billBarzeghan->setMaterialTexture(0, driver->getTexture("../../../media/particlegreen.jpg"));

	//
		// add fly circle animator to light 1
	scene::ISceneNodeAnimator* animBarzeghan =
		smgr->createFlyCircleAnimator (core::vector3df(-1000,300,0),190.0f, -0.003f);
	lightBarzeghan->addAnimator(animBarzeghan);
	animBarzeghan->drop();

///////////////////////////// Add Barzeghan [End]


///////////////////////////// Add Main Gun [Begin]
	
	scene::IMesh* gun= smgr->getMesh("Objects/rifle.obj"); //M24_R.obj
	if (!gun)
	{
		device->drop();
		return 1;
	}

		scene::IMesh* tangentGunMesh = smgr->getMeshManipulator()->
				createMeshWithTangents(gun);//gun->getMesh(0)

	scene::IMeshSceneNode* gunNode = smgr->addMeshSceneNode(tangentGunMesh);

	if (gunNode)
	{
		gunNode->setMaterialTexture( 0, driver->getTexture("Objects/bakecol.jpg") );
		gunNode->setMaterialTexture( 1, driver->getTexture("Objects/riflenormal.jpg") );//Normal Map
		gunNode->setMaterialFlag(EMF_LIGHTING, false);
		// gunNode->setMaterialType(EMT_SOLID);
		gunNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
		gunNode->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
		gunNode->setScale(vector3df(.7f,.7f,.7f)); // vector3df(.9f,.9f,.9f)

		gunNode->setPosition(vector3df(3,-2.5,5)); // vector3df(3,-2.5,5)
		gunNode->setRotation(vector3df(-10,-15,0));
		//core::vector3df camPos = camera->getPosition();
		//camPos.Z=+150;
		//camPos.Y-=40;
		//gunNode->setPosition(camPos);
		gunNode->setParent(camera);

		//gunNode->setRotation(core::vector3df(0,90,0));
	}

		// attach billboard as GUNCrossHair to the gun
	scene::IBillboardSceneNode* billGUNCrossHair =
		smgr->addBillboardSceneNode(camera, core::dimension2d<f32>(1, 1));

	billGUNCrossHair->setMaterialFlag(video::EMF_LIGHTING, false);
	billGUNCrossHair->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	billGUNCrossHair->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	billGUNCrossHair->setMaterialTexture(0, driver->getTexture("Objects/cross2.jpg"));
	
	core::vector3df camTargetPos = camera->getTarget();
	billGUNCrossHair->setPosition(vector3df(0,0,10));

	/* Touraj: Tutorial: Another way of rendering crosshair...from: irrAI 0.5
				
				s32 imgDim = 64;
				driver->draw2DImage(driver->getTexture("../../Media/crosshair.png"), core::rect<s32>(400-(imgDim/2),300-(imgDim/2), 400+(imgDim/2),300+(imgDim/2)), core::rect<s32>(0,0, imgDim,imgDim), NULL, NULL, true);
		
	*/
	
	// add Real time shadow Casting To Gun
	/*		 gunNode->addShadowVolumeSceneNode();
			gunNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);*/


///////////////////////////// Add Main Gun [End]

	// add light 1 (more green)
	scene::ILightSceneNode* light1 =
		smgr->addLightSceneNode(0, core::vector3df(0,0,0),
		video::SColorf(0.5f, 1.0f, 0.5f, 0.0f), 100.0f);

	light1->setDebugDataVisible ( scene::EDS_BBOX );
	//light1->enableCastShadow();

	// add fly circle animator to light 1
	scene::ISceneNodeAnimator* anim =
		smgr->createFlyCircleAnimator (core::vector3df(50,300,0),190.0f, -0.003f);
	light1->addAnimator(anim);
	anim->drop();

	// attach billboard to the light
	scene::IBillboardSceneNode* bill =
		smgr->addBillboardSceneNode(light1, core::dimension2d<f32>(60, 60));

	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, driver->getTexture("../../../media/particlegreen.jpg"));

	// add light 2 (red)
	scene::ILightSceneNode* light2 =
		smgr->addLightSceneNode(0, core::vector3df(0,0,0),
		video::SColorf(1.0f, 0.2f, 0.2f, 0.0f), 400.0f);

	// add fly circle animator to light 2
	anim = smgr->createFlyCircleAnimator(core::vector3df(0,150,0), 200.0f,
			0.001f, core::vector3df(0.2f, 0.9f, 0.f));
	//light2->enableCastShadow();
	light2->addAnimator(anim);
	anim->drop();

	// attach billboard to light
	bill = smgr->addBillboardSceneNode(light2, core::dimension2d<f32>(120, 120));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, driver->getTexture("../../../media/particlered.bmp"));

	//// add light 3 (more Blue)
	//scene::ILightSceneNode* light3 =
	//	smgr->addLightSceneNode(0, core::vector3df(0,0,0),
	//	video::SColorf(0.5f, 1.0f, 0.0f, 0.0f), 800.0f);

	//light3->setDebugDataVisible ( scene::EDS_BBOX );


	//// add fly circle animator to light 3
	//scene::ISceneNodeAnimator* anim2 =
	//	smgr->createFlyCircleAnimator (core::vector3df(50,500,0),290.0f, -0.003f);
	//light3->addAnimator(anim2);
	//anim2->drop();

	//// attach billboard to the light 3
	//scene::IBillboardSceneNode* bill2 =
	//	smgr->addBillboardSceneNode(light3, core::dimension2d<f32>(60, 60));

	//bill2->setMaterialFlag(video::EMF_LIGHTING, false);
	//bill2->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	//bill2->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	//bill2->setMaterialTexture(0, driver->getTexture("../../../media/portal1.bmp"));

	//// Ligh 3 End

	// add particle system
	scene::IParticleSystemSceneNode* ps =
		smgr->addParticleSystemSceneNode(false, light2);

	// create and set emitter
	scene::IParticleEmitter* em = ps->createBoxEmitter(
		core::aabbox3d<f32>(-3,0,-3,3,1,3),
		core::vector3df(0.0f,0.03f,0.0f),
		80,100,
		video::SColor(10,255,255,255), video::SColor(10,255,255,255),
		400,1100);
	em->setMinStartSize(core::dimension2d<f32>(30.0f, 40.0f));
	em->setMaxStartSize(core::dimension2d<f32>(30.0f, 40.0f));

	ps->setEmitter(em);
	em->drop();

	// create and set affector
	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();
	ps->addAffector(paf);
	paf->drop();

	// adjust some material settings
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("../../../media/fireball.bmp"));
	ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

	////////////////////////////////////////// Sky box Begin
	// scene::ISceneNode* skybox;
	//scene::ISceneNode* skydome;

	 // create skybox and skydome
	// Touraj: [Important]For the skybox textures, we disable mipmap generation, because we don't need mipmaps on it.
    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

    //scene::ISceneNode* skybox=smgr->addSkyBoxSceneNode(
    //    driver->getTexture("../../media/irrlicht2_up.jpg"),
    //    driver->getTexture("../../media/irrlicht2_dn.jpg"),
    //    driver->getTexture("../../media/irrlicht2_lf.jpg"),
    //    driver->getTexture("../../media/irrlicht2_rt.jpg"),
    //    driver->getTexture("../../media/irrlicht2_ft.jpg"),
    //    driver->getTexture("../../media/irrlicht2_bk.jpg"));

    scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture("Objects/scifidome4.jpg"),16,8,0.95f,2.0f
		,1000.f // Radius
		);

	// ../../media/skydome.jpg"

    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
	skydome->setVisible(true);


	////////////////////////////////////////////// Sky Box end


	//!Test VolueLightNode
scene::IVolumeLightSceneNode* vlight = smgr->addVolumeLightSceneNode (0,
      -1,
      8,
      8,
      video::SColor(51, 255, 100, 100),
      video::SColor(0, 0, 0, 0),
      core::vector3df(0, -250, 0),
      core::vector3df(0, 0, 0),
      core::vector3df(300.f, 700.f, 300.f)
   );

    vlight->setMaterialFlag(video::EMF_LIGHTING, false);
    //vlight->setScale(core::vector3df(-1,-1,-1));
    vlight->setMaterialTexture( 0, driver->getTexture("Objects/lava.jpg"));
   //vlight->setMaterialFlag(EMF_LIGHTING, false);
   //!vlight->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
   //!vlight->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
   //!vlight->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    vlight->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
   vlight->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
   vlight->setMaterialFlag(EMF_FRONT_FACE_CULLING, false);
   //vlight->setPosition(core::vector3df(0.628f,0.965f,2.4f));

//! end volumetric light


//////////////////////////////////////// Real Clouds [Begin]

	//scene::CCloudSceneNode* clouds = new scene::CCloudSceneNode( smgr->getRootSceneNode(), smgr, device->getTimer(), 666); 

 //   video::ITexture * txture = driver->getTexture("Objects/cloud4.png"); 
 //       
 //   // set any old random seed
 //   srand(1928);    
 //       
 //   // set the level of detail
 //   clouds->setLOD(10); 
 //   // set the maximum detail level
 //   clouds->setMaxDepth(3); 
 //   
 //   // we dont use lighting yet. all the normals face the camera and it looks wierd
 //   clouds->setMaterialFlag(video::EMF_LIGHTING, false); 
 //
 //   clouds->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); //makes it transparent
 // 
 //   // set any old random seed
 //   srand(rand()); 
 //   
 //   // call a "here's one I made earlier" function
 //   clouds->makeRandomCloud(300);

 //  /* 
 //   // for loading transparrent TGAs
 //   //    device->getVideoDriver()->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT,true);
 //   //    device->getVideoDriver()->setTextureCreationFlag(irr::video::ETCF_ALWAYS_16_BIT,false);
 //   //    device->getVideoDriver()->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS,false); 
 //  */  

 //   clouds->setMaterialTexture( 0, txture ); 
 //   clouds->setPosition(vector3df(0,0,0));
 //        
 //   f32 rot=1.0;

/////////////////////////////////////// Real Clouds [End]

	
   // create event receiver
    MyEventReceiver receiver(room,device);
    device->setEventReceiver(&receiver);


	int lastFPS = -1;

	while(device->run())
	{
		if (device->isWindowActive())
		{
			driver->beginScene(
				Rconsts.backBuffer,
				Rconsts.zBuffer, 
				0);

			smgr->drawAll();
			env->drawAll();

			driver->endScene();

			// for cloud
		//rot-= 0.02;
  //      clouds->setRotation(vector3df(0,rot,0));
        

			int fps = driver->getFPS();
			irr::u32 timecurr =  device->getTimer()->getTime();
			Tools::WriteToFile("fps.log","at time "+std::to_string(timecurr)+" fps is: "+std::to_string(fps));
			
			if (lastFPS != fps)
			{
				core::stringw str = L"Per pixel lighting example - Irrlicht Engine [";
				str += driver->getName();
				str += "] FPS:";
				str += fps;

				device->setWindowCaption(str.c_str());
				lastFPS = fps;
			}
		}else device->yield();
	}

	device->drop();

	return 0;
}
