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
#include <tp/version.h>

// include OBJ loader
#include <glm/glm.h>

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
		return 0L;
	}

	// regenerate the model structure
	glmUnitize(model);
	glmFacetNormals(model);
	// should make the crease angle an option
	glmVertexNormals(model,89);
	glmLinearTexture(model);

	tpRefNode root = new tpNode();

	group = model->groups;

	while (group)
	{
		//tpLogNotify("%s found group",__FUNCTION__);

		if (group->numtriangles > 0)
		{
			tpPrimitive* mesh = new tpPrimitive(tpPrimitive::kTriangles);

			//tpLogNotify("%s found %d triangles (%s)",__FUNCTION__,group->numtriangles,group->name);

			if (group->name) mesh->setName(group->name);

			for (int tri_idx = 0; tri_idx < group->numtriangles; ++tri_idx)
			{

				// get the triangle
				triangle = &TRIS(group->triangles[tri_idx]);

				if (triangle) {

					tpVec3r cur_vertex;
					tpVec3r cur_normal;
					tpVec3r cur_color;
					tpVec2r cur_tcoord;

					// loop through vertices
					for (int v_idx = 0; v_idx < 3; v_idx++)
					{
						// vertex
						cur_vertex[0] = model->vertices[3 * triangle->vindices[v_idx] + 0];
						cur_vertex[1] = model->vertices[3 * triangle->vindices[v_idx] + 1];
						cur_vertex[2] = model->vertices[3 * triangle->vindices[v_idx] + 2];

						// normal
						cur_normal[0] = model->normals[3 * triangle->nindices[v_idx] + 0];
						cur_normal[1] = model->normals[3 * triangle->nindices[v_idx] + 1];
						cur_normal[2] = model->normals[3 * triangle->nindices[v_idx] + 2];

						// tcoord
						cur_tcoord[0] = model->texcoords[2 * triangle->tindices[v_idx] + 0];
						cur_tcoord[1] = model->texcoords[2 * triangle->tindices[v_idx] + 1];

						// add to mesh
						mesh->addVertex(cur_vertex,cur_normal,cur_tcoord);

						//tpLogNotify("%s found add vertex no %d of triangle %d",__FUNCTION__,v_idx,tri_idx);

					}
				}
			}


			if (1)//group->material)
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

				tpLogNotify("%s - Ambient: %3.3f %3.3f %3.3f %3.3f",
						__FUNCTION__,
						material->ambient[0],material->ambient[1],material->ambient[2],material->ambient[3]);

				tpLogNotify("%s - Diffuse %3.3f %3.3f %3.3f %3.3f",
						__FUNCTION__,
						material->diffuse[0],material->diffuse[1],material->diffuse[2],material->diffuse[3]);

				tpLogNotify("%s - Specular %3.3f %3.3f %3.3f %3.3f",
						__FUNCTION__,
						material->specular[0],material->specular[1],material->specular[2],material->specular[3]);

				tpLogNotify("%s - Shininess %3.3f",
						__FUNCTION__,
						material->shininess);

				if (material->texturename)
				{
					tpTexture* texture = new tpTexture;

					tpImage* img = tpImage::read(material->texturename);

					texture->setImage(img);

					if (img) mesh->setTexture(texture);

					tpLogNotify("%s - added texture '%s'",__FUNCTION__,img);
				}

				tpLogNotify("%s - added material '%s'\n",
						__FUNCTION__,mat->getName().c_str()
						);

			};


			root->addChild(mesh);
		}

		group = group->next;
	}

	//exit(0);

	glmDelete(model);

	return root.release();

}

class tpOBJHandler : public tpNodeHandler {
public:

	TP_TYPE_DECLARE;

	tpOBJHandler() : tpNodeHandler()
	{
		tpLogNotify("%s OBJ model file support",tpGetVersionString());
	}

	~tpOBJHandler()
	{
	}

	tpUByte getCapabilities(const tpString& name)
	{
		tpString extension = name.afterLast('.');
		if (extension == "obj") {
			return kRead;
		}
		return 0;
	}

	tpNode* read(const tpString& name)
	{
		tpLogNotify("tpOBJ Loader loading %s",name.c_str());

		return tpOBJ(name.c_str());
	}

};


TP_TYPE_REGISTER(tpOBJHandler,tpNodeHandler,NodeHandler_OBJ);

//TP_MODULE_HOOK(NodeOBJ)

tpModuleInitializer<tpOBJHandler> g_nodefactory_obj;
