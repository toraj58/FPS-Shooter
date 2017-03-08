#pragma once

/*
By: Touraj Ebrahimi
Game Render Constants By Touraj Ebrahimi
*/

struct RenderConsts
{
	static const bool fullscreen = true; // Should be set to true if the device should run in fullscreen. Otherwise the device runs in windowed mode. 
	static const bool stencilBuffer = true; // Specifies if the stencil buffer should be enabled. Set this to true, if you want the engine be able to draw stencil buffer shadows. Note that not all devices are able to use the stencil buffer. If they don't no shadows will be drawn. 
	static const bool vSync = true; // Specifies vertical syncronisation: If set to true, the driver will wait for the vertical retrace period, otherwise not. 

	static const int pixelBits = 32; //Bits per pixel in fullscreen mode. Ignored if windowed mode. 
	static const int width = 1366; //// 1366, 768
	static const int height = 768; //// 1366, 768

	// Following Constants are used in BeginScene: driver->beginScene

	static const bool backBuffer = true; // backBuffer Specifies if the back buffer should be cleared, which means that the screen is filled with a color specified with the parameter color. If this parameter is false, the back buffer will not be cleared and the color parameter is ignored.
	static const bool zBuffer = true; // Speciefies if the depth or z buffer should be cleared. It is not nesesarry to do so, if only 2d drawing is used.

};
