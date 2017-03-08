#include "FillMetaSelector.h"

void fillMetaSelector(IrrlichtDevice* device, scene::IMetaTriangleSelector* metaSelector, scene::ISceneNode* node) {

	if (!node) return;
  
	scene::ISceneManager* smgr = device->getSceneManager();
  
	if (node != smgr->getRootSceneNode()) {
    
		scene::ITriangleSelector* selector = NULL;
    
		switch (node->getType()) {
      
			// Create an oct tree triangle selector
			case scene::ESNT_OCT_TREE: {
				io::IAttributes* attribs = device->getFileSystem()->createEmptyAttributes(); 
				if (attribs)  {// get the mesh name out 
					node->serializeAttributes(attribs); 
					core::stringc name = attribs->getAttributeAsString("Mesh"); 
					attribs->drop(); 
					// get the animated mesh for the object 
					scene::IAnimatedMesh* mesh = smgr->getMesh(name.c_str()); 
					// create the triangle selector
					if (mesh) selector = smgr->createOctTreeTriangleSelector(mesh->getMesh(0), node, 128); 
				} 
				break;
			}
        
			// Create a terrain triangle selector
			case scene::ESNT_TERRAIN: {
				selector = smgr->createTerrainTriangleSelector((scene::ITerrainSceneNode*)node, 0);
				break;
			}
      
			// Create mesh-based triangle selectors for these nodes
			case scene::ESNT_MESH: {
				selector = smgr->createTriangleSelector(((scene::IMeshSceneNode*)node)->getMesh(), node);      
				break;
			}
			case scene::ESNT_ANIMATED_MESH:
			case scene::ESNT_MD3_SCENE_NODE: /*{
				io::IAttributes* attribs = device->getFileSystem()->createEmptyAttributes(); 
				if (attribs)  {// get the mesh name out 
					node->serializeAttributes(attribs); 
					core::stringc name = attribs->getAttributeAsString("Mesh"); 
					attribs->drop();
					// get the animated mesh for the object 
					scene::IAnimatedMesh* mesh = smgr->getMesh(name.c_str()); 
					// create the triangle selector
					if (mesh) selector = smgr->createTriangleSelector(mesh->getMesh(0), node); 
				}
				break;      
			}*/
           
			// Create boudning box triangle selectors from these nodes
			case scene::ESNT_SPHERE:
			case scene::ESNT_CUBE:
			case scene::ESNT_WATER_SURFACE: {
				selector = smgr->createTriangleSelectorFromBoundingBox(node);
				break;
			}
			// Ignore these and any others not listed here
			case scene::ESNT_BILLBOARD:
			case scene::ESNT_TEXT:
			case scene::ESNT_PARTICLE_SYSTEM:
			case scene::ESNT_SKY_BOX:
			case scene::ESNT_SHADOW_VOLUME:
			case scene::ESNT_LIGHT:
			case scene::ESNT_EMPTY:
			case scene::ESNT_DUMMY_TRANSFORMATION:
			case scene::ESNT_CAMERA:
			case scene::ESNT_CAMERA_MAYA:
			case scene::ESNT_CAMERA_FPS:
			case scene::ESNT_UNKNOWN:
			default:
				break;    
		}
    
		// If we made a triangle selector then add it to the node and the meta selector
		if (selector) {
			node->setTriangleSelector(selector); 
			metaSelector->addTriangleSelector(selector); 
			selector->drop(); // we're done with it now
		}
    
	}
  
	// recurse through the children
	core::list<scene::ISceneNode*> children = node->getChildren();
	core::list<scene::ISceneNode*>::Iterator iter = children.begin();
	while (iter != children.end()) {
		fillMetaSelector(device, s, *iter);
		++iter;      
	}
  
}
