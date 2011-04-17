/*
 * Twisted Pair Visualization Engine
 *
 * Copyright (c) 1999-2009 Hartmut Seichter 
 * 
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the Twisted Pair License (TPL) version 1.0 or (at your option) 
 * any later version. The full license text is available in the LICENSE file 
 * included with this distribution, and on the technotecture.com website.
 *
 */

// includes from libTAP

#include <tp/node.h>
#include <tp/module.h>
#include <tp/system.h>
#include <tp/primitive.h>
#include <tp/log.h>

// include OBJ loader
#include "glm.h"
      
#define TRIS(x) (model->triangles[(x)])

tpNode* tpOBJ(const tpString& filename)
{
	
	GLMgroup* group(0);
	GLMtriangle* triangle(0);
	GLMmodel* model = glmReadOBJ((char*)filename.c_str());


	// the loader needs to set the working directory 
	if (!model) 
	{
		tpLogError("tpOBJSceneLoader::getScene(%s) : can not load this file!",
			filename.c_str());
		return NULL;
	}


	glmUnitize(model);
	glmFacetNormals(model);

	tpNode* root = new tpNode();

	group = model->groups;
	
	while (group)
	{
		tpLogNotify("%s found group",__FUNCTION__);

		if (group->numtriangles > 0)
		{
			tpPrimitive* mesh = new tpPrimitive(tpPrimitive::kTriangles);
			
			tpLogMessage("%s found %d triangles",__FUNCTION__,group->numtriangles);

			if (group->name) mesh->setName(group->name);
				
			for (size_t i = 0; i < group->numtriangles; i++)
			{
				triangle = &TRIS(group->triangles[i]); 

				if (0 == triangle) continue;

				tpVec3r cur_vertex;
				tpVec3r cur_normal;
				tpVec3r cur_color;
				tpVec3r cur_tcoord;

				/*
				// WTF using same index?
				for (int i = 0; i < 3; ++i)
				{
					int idx = triangle->vindices[i];
					
					cur_vertex[0] = (tpReal)model->vertices[3 * idx + 0];
					cur_vertex[1] = (tpReal)model->vertices[3 * idx + 1];
					cur_vertex[2] = (tpReal)model->vertices[3 * idx + 2];

					mesh->addVertex(v1,v2,v3);
					
					// Normal for this vertex
					idx = triangle->nindices[i];

					if (idx >= 0) {
						
						v1 = (tpReal)model->normals[3 * idx];
						v2 = (tpReal)model->normals[3 * idx + 1];
						v3 = (tpReal)model->normals[3 * idx + 2];

						mesh->addNormal(v1,v2,v3);
					}
					
					// Texture Coordinates for this vertex
					idx = triangle->tindices[i];
					
					if (idx >= 0) {
						
						tpReal t1 = (tpReal)model->texcoords[2 * idx];
						tpReal t2 = (tpReal)model->texcoords[2 * idx + 1];
						
						mesh->addTexCoord(t1, t2);
						
					}
				}
				*/
			}
		
			if (model->nummaterials)
			{
				GLMmaterial* material;

				material = &model->materials[group->material];

				tpMaterial* mat = new tpMaterial();
				
				mat->setSpecularColor(tpVec4f(material->specular[0],material->specular[1],material->specular[2],material->specular[3]));

				mat->setDiffuseColor(tpVec4f(material->diffuse[0],material->diffuse[1],material->diffuse[2],material->diffuse[3]));

				mat->setEmissiveColor(tpVec4f(material->emmissive[0],material->emmissive[1],material->emmissive[2],material->emmissive[3]));				

				mat->setAmbientColor(tpVec4f(material->ambient[0],material->ambient[1],material->ambient[2],material->ambient[3]));

				mat->setTransparency(material->transparency);

				mat->setShininess(material->shininess);

				if (material->name) mat->setName(material->name);

				mesh->setMaterial(mat);
				
				if (material->texturename)
				{
					tpTexture* texture = new tpTexture;
					
					tpImage* img = tpImage::read(material->texturename);
					
					texture->setImage(img);
					
					if (img) mesh->setTexture(texture);
	
					tpLogNotify("%s - added texture '%s'",__FUNCTION__,img);
				}

				tpLogNotify("%s - added material",__FUNCTION__);
				
			};
	
			
			root->addChild(mesh);

			//mesh->dump();
		};

		group = group->next;
	};

	glmDelete(model);

	return root;

};

/*

class tpNodeFactoryOBJ : public tpNodeFactory {
public:

	TP_TYPE_DECLARE;

	tpNodeFactoryOBJ() : tpNodeFactory() 
	{
		tpLogNotify("Twisted Pairs OBJ loader 1.0");
	}

	~tpNodeFactoryOBJ() 
	{
	}

	tpBool getCapabilities(tpUInt capability, const tpString& name)
	{
		switch (capability)
		{
		case TP_NODE_CAN_READ:
			return name.afterLast('.') == "obj";
			break;

		}

		return false;
	}

	tpNode* read(const tpString& name) 
	{
		tpLogNotify("tpOBJ Loader loading %s",name.c_str());

		return tpOBJ(name.c_str());
	}

};

*/

//TP_TYPE_REGISTER(tpNodeFactoryOBJ,tpNodeFactory,NodeFactoryOBJ);
//
//TP_MODULE_HOOK(NodeOBJ)
//
//tpModuleInitializer<tpNodeFactoryOBJ> g_nodefactory_obj;
