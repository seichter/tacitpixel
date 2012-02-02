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

#include <tp/node.h>
#include <tp/module.h>
#include <tp/system.h>
#include <tp/primitive.h>
#include <tp/transform.h>
#include <tp/log.h>
#include <tp/version.h>

#include <lib3ds.h>

class tpNodeHandler_3DS : public tpNodeHandler {
public:

	TP_TYPE_DECLARE;

	tpNodeHandler_3DS()
		: tpNodeHandler()
	{
		tpLogNotify("%s 3DS model file support",tpGetVersionString());
	}

	tpUByte getCapabilities(const tpString& name)
	{
		tpString extension = name.afterLast('.');
		if (extension == "3ds") {
			return kRead;
		}
		return 0;
	}


	~tpNodeHandler_3DS() {}

	tpNode* read(const tpString& name);

};

tpMaterial*
createMaterial(Lib3dsMaterial* m3ds)
{
	if (0 == m3ds) return tpDefaultMaterial;

	tpMaterial* material = new tpMaterial(m3ds->name);
	material->setAmbientColor(tpVec4f(m3ds->ambient[0],m3ds->ambient[1],m3ds->ambient[2],1));
	material->setDiffuseColor(tpVec4f(m3ds->diffuse[0],m3ds->diffuse[1],m3ds->diffuse[2],1));
	material->setSpecularColor(tpVec4f(m3ds->specular[0] * m3ds->shin_strength,m3ds->specular[1] * m3ds->shin_strength,m3ds->specular[2] * m3ds->shin_strength,1));

	material->setShininess(m3ds->shininess*128.f);

}



tpNode*
tpNodeHandler_3DS::read(const tpString& name)
{
	tpLogNotify("%s loading '%s'",tpGetVersionString(),name.c_str());

	Lib3dsFile* file3ds = lib3ds_file_open(name.c_str());

	if (0 == file3ds) return 0;

	lib3ds_file_eval(file3ds, file3ds->frames);
	lib3ds_file_create_nodes_for_meshes(file3ds);

	tpLogNotify("\texporter %s",file3ds->name);
	tpLogNotify("\tmesh format %d",file3ds->mesh_version);
	tpLogNotify("\tkeyframe format %d",file3ds->keyf_revision);
	tpLogNotify("\tmaterials %d",file3ds->nmaterials);
	tpLogNotify("\tmeshes %d",file3ds->nmeshes);
	tpLogNotify("\tnodes %d",0 != file3ds->nodes);
	tpLogNotify("\tframes %d",0 != file3ds->frames);

	if (true) {
//		Lib3dsVector min, max;
//		lib3ds_object_bounding_box(file3ds,min,max);

//		tpLogNotify("\tMin %3.3f %3.3f %3.3f",min[0],min[1],min[2]);
//		tpLogNotify("\tMax %3.3f %3.3f %3.3f",max[0],max[1],max[2]);
	}

	tpNode* node = new tpNode();


	typedef float Lib3dsMatrix[4][4];
	typedef float Lib3dsVector[3];
	typedef void Lib3dsUserData;


	for (int meshIdx = 0;
		 meshIdx < file3ds->nmeshes;
		 ++meshIdx)
	{
		Lib3dsMesh* mesh3ds = file3ds->meshes[meshIdx];

		tpLogNotify("\t\tmesh %s %d faces %d vertices",mesh3ds->name,mesh3ds->nfaces / 2,mesh3ds->nvertices);

		tpTransform* mesh = new tpTransform();
		tpMat44r m; m.copyFrom(&mesh3ds->matrix[0][0]);
		m.invert();
		mesh->setMatrix(m);

		tpPrimitive* prim = new tpPrimitive(tpPrimitive::kTriangles);

		Lib3dsVector* normals3ds = new Lib3dsVector[mesh3ds->nfaces];

		lib3ds_mesh_calculate_face_normals(mesh3ds,&normals3ds[0]);

		for (size_t faceIdx = 0;
			 faceIdx < mesh3ds->nfaces;
			 ++faceIdx)
		{
			Lib3dsFace* face3ds = &mesh3ds->faces[faceIdx];

			tpVec3r normal(normals3ds[faceIdx][0],normals3ds[faceIdx][1],normals3ds[faceIdx][2]);
			normal.normalize();

			for (int i = 0; i < 3; ++i) {
				mesh3ds->vertices[ face3ds->index[ i ] ];
				tpVec3r vtx(mesh3ds->vertices[ face3ds->index[ i ] ][0],
							mesh3ds->vertices[ face3ds->index[ i ] ][1],
							mesh3ds->vertices[ face3ds->index[ i ] ][2]
							);

				//tpLogMessage("%3.3f %3.3f %3.3f",vtx[0],vtx[1],vtx[2]);

				prim->addVertex(vtx,normal);
			}
		}

		delete [] normals3ds;

		mesh->addChild(prim);
		node->addChild(mesh);
	}

	lib3ds_file_free(file3ds);

	return node;
}


TP_TYPE_REGISTER(tpNodeHandler_3DS,tpNodeHandler,NodeHandler_3ds);
tpModuleInitializer<tpNodeHandler_3DS> g_nodefactory_3ds;



