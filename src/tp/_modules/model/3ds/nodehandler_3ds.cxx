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
#include <tp/map.h>
#include <tp/stringformater.h>

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
//	if (0 == m3ds) return tpDefaultMaterial;

	if (0 == m3ds) return 0L;

	tpLogNotify("created material %s",m3ds->name);

	tpMaterial* material = new tpMaterial(m3ds->name);
	material->setAmbientColor(tpVec4f(m3ds->ambient[0],m3ds->ambient[1],m3ds->ambient[2],1));
	material->setDiffuseColor(tpVec4f(m3ds->diffuse[0],m3ds->diffuse[1],m3ds->diffuse[2],1));
	material->setSpecularColor(tpVec4f(m3ds->specular[0] * m3ds->shin_strength,m3ds->specular[1] * m3ds->shin_strength,m3ds->specular[2] * m3ds->shin_strength,1));

	material->setShininess(m3ds->shininess*128.f);

	return material;
}

typedef tpArray<tpRefPtr<tpMaterial> > tpMaterialCache;

void
cacheMaterials(Lib3dsFile* file,tpMaterialCache& cache) {
	for (int i = 0; i < file->nmaterials;++i) {
		tpMaterial* m = createMaterial(file->materials[i]);
		cache.add(m);
	}
}


tpNode*
tpNodeHandler_3DS::read(const tpString& name)
{
	tpMaterialCache materialcache;

	tpLogNotify("%s loading '%s'",tpGetVersionString(),name.c_str());

	Lib3dsFile* file3ds = lib3ds_file_open(name.c_str());

	if (0 == file3ds) return 0;

	lib3ds_file_eval(file3ds, 0);
	//
	//lib3ds_file_eval(file3ds, file3ds->frames);
	//lib3ds_file_create_nodes_for_meshes(file3ds);

	tpLogNotify("\texporter %s",file3ds->name);
	tpLogNotify("\tmesh format %d",file3ds->mesh_version);
	tpLogNotify("\tkeyframe format %d",file3ds->keyf_revision);
	tpLogNotify("\tmaterials %d",file3ds->nmaterials);
	tpLogNotify("\tmeshes %d",file3ds->nmeshes);
	tpLogNotify("\tnodes %d",0 != file3ds->nodes);
	tpLogNotify("\tframes %d",0 != file3ds->frames);

	cacheMaterials(file3ds,materialcache);


	tpNode* node = new tpNode();

	typedef float Lib3dsMatrix[4][4];
	typedef float Lib3dsVector[3];
	typedef void Lib3dsUserData;


	for (int meshIdx = 0;
		 meshIdx < file3ds->nmeshes;
		 ++meshIdx)
	{
		if(meshIdx > 0 ) continue;

		Lib3dsMesh* mesh3ds = file3ds->meshes[meshIdx];

		tpLogNotify("\t\tmesh %d %s %d faces %d vertices",
					meshIdx,
					mesh3ds->name,
					mesh3ds->nfaces / 2,
					mesh3ds->nvertices
					);

		tpTransform* mesh = new tpTransform();


		tpMat44r m;
		Lib3dsMatrix m3ds;
		lib3ds_matrix_copy(m3ds,mesh3ds->matrix);
		lib3ds_matrix_inv(m3ds);

		m.copyFrom(&m3ds[0][0]);

		tpString out; out << m;
//		tpLogMessage("T: %s",out.c_str());

		//m.invert();
		mesh->setMatrix(m);

		tpPrimitive* prim = new tpPrimitive(tpPrimitive::kTriangles);

		tpPrimitive* pn = new tpPrimitive();

		mesh->addChild(prim);
		node->addChild(mesh);

#if 1
		Lib3dsVector* normals3ds = new Lib3dsVector[mesh3ds->nfaces];
        lib3ds_mesh_calculate_face_normals(mesh3ds,&normals3ds[0]);
#else
        Lib3dsVector* normals3ds = new Lib3dsVector[mesh3ds->nfaces * 3];
        lib3ds_mesh_calculate_vertex_normals(mesh3ds,&normals3ds[0]);
#endif
		for (size_t faceIdx = 0;
			 faceIdx < mesh3ds->nfaces;
			 ++faceIdx)
		{
			Lib3dsFace* face3ds = &mesh3ds->faces[faceIdx];

			if (0 == prim->getMaterial()) {
				tpRefPtr<tpMaterial> m = materialcache[face3ds->material];
				if (m.isValid()) prim->setMaterial(m.get());
			}


            tpVec3r normal(normals3ds[faceIdx][0],normals3ds[faceIdx][1],normals3ds[faceIdx][2]);

            normal.normalize();

			for (int i = 0; i < 3; ++i) {


				//mesh3ds->vertices[ face3ds->index[ i ] ];

				tpVec3r vtx(mesh3ds->vertices[ face3ds->index[ i ] ][0],
							mesh3ds->vertices[ face3ds->index[ i ] ][1],
							mesh3ds->vertices[ face3ds->index[ i ] ][2]
							);

				prim->addVertexNormal(vtx,normal);

			}
		}

		delete [] normals3ds;

	}

	materialcache.clear();

	lib3ds_file_free(file3ds);

	return node;
}


TP_TYPE_REGISTER(tpNodeHandler_3DS,tpNodeHandler,NodeHandler_3ds);
tpModuleInitializer<tpNodeHandler_3DS> g_nodefactory_3ds;



