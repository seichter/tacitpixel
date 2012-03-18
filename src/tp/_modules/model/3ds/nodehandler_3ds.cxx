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
#include <tp/logutils.h>

#include <lib3ds.h>

class tpNodeHandler_3DS : public tpNodeHandler {
public:

	TP_TYPE_DECLARE

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

typedef tpArray< tpRefPtr<tpMaterial> > tpMaterialCache;
typedef tpArray< tpRefPtr<tpTexture> >  tpTextureCache;

tpMaterialCache gsMatCache;
tpTextureCache gsTexCache;


bool hasTexture(const Lib3dsMaterial* m3ds) {
	return (m3ds->texture1_map.name[0] > 0);
}

void parseMaterial(Lib3dsMaterial* m3ds, const tpString& path)
{
    if (0 == m3ds) return;

	tpLogNotify("created material %s",m3ds->name);

	tpMaterial* material = new tpMaterial(m3ds->name);
	material->setAmbientColor(tpVec4f(m3ds->ambient[0],m3ds->ambient[1],m3ds->ambient[2],1));
	material->setDiffuseColor(tpVec4f(m3ds->diffuse[0],m3ds->diffuse[1],m3ds->diffuse[2],1));
	material->setSpecularColor(tpVec4f(m3ds->specular[0] * m3ds->shin_strength,m3ds->specular[1] * m3ds->shin_strength,m3ds->specular[2] * m3ds->shin_strength,1));

	material->setShininess(m3ds->shininess*128.f);

	gsMatCache.add(material);

	if (hasTexture(m3ds)) 
	{
		tpTexture* texture = new tpTexture(tpString(m3ds->texture1_map.name));
		tpImage* image = tpImage::read(path+"/"+texture->getName());

		texture->setFormat(tpTexture::kFormatRGB);
		texture->setImage(image);

		gsTexCache.add(texture);

		//
		//tpLogNotify("Name %s\n\tscale %f",m3ds->texture1_map.name,
		//	m3ds->texture1_map.flags);

	} else {
		// we do that to use the same id from the face
		gsTexCache.add(0);
	}
}


void
cacheAssets(Lib3dsFile* file,const tpString& path) {

	gsTexCache.clear();
	gsMatCache.clear();

	for (int i = 0; i < file->nmaterials;++i) {
		parseMaterial(file->materials[i],path);
	}
}




tpNode*
tpNodeHandler_3DS::read(const tpString& name)
{
	tpString path = name.beforeLast('/');

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

	cacheAssets(file3ds,path);

	tpNode* node = new tpNode();

	typedef float Lib3dsMatrix[4][4];
	typedef float Lib3dsVector[3];
	typedef void Lib3dsUserData;


	for (int meshIdx = 0;
		 meshIdx < file3ds->nmeshes;
		 ++meshIdx)
	{
		//if(meshIdx > 0 ) continue;

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
		prim->addRenderFlag(tpRenderFlag::kLighting);

		mesh->addChild(prim);
		node->addChild(mesh);

		Lib3dsVector* normals3ds = new Lib3dsVector[mesh3ds->nfaces * 3];
		lib3ds_mesh_calculate_vertex_normals(mesh3ds,&normals3ds[0]);


		for (size_t faceIdx = 0;
			 faceIdx < mesh3ds->nfaces;
			 ++faceIdx)
		{
			Lib3dsFace* face3ds = &mesh3ds->faces[faceIdx];

			if (0 == prim->getMaterial()) {
				tpRefPtr<tpMaterial> m = gsMatCache[face3ds->material];
				if (m.isValid()) {
					prim->setMaterial(m.get());
				}
			}

			if (!prim->hasTexture()) {
				tpRefPtr<tpTexture> t = gsTexCache[face3ds->material];
				if (t.isValid()) {
					prim->setTexture(t.get());
				}
			}

			for (int i = 0; i < 3; ++i) {

                tpVec3r nrm(normals3ds[ faceIdx * 3 + i][0] ,
                            normals3ds[ faceIdx * 3 + i][1] ,
                            normals3ds[ faceIdx * 3 + i][2] );


                tpVec3r vtx(mesh3ds->vertices[ face3ds->index[ i ] ][0],
                            mesh3ds->vertices[ face3ds->index[ i ] ][1],
                            mesh3ds->vertices[ face3ds->index[ i ] ][2]
                            );

                if (mesh3ds->texcos)
                {
                    tpVec2r txc(mesh3ds->texcos[ face3ds->index[ i ] ][0],
                                mesh3ds->texcos[ face3ds->index[ i ] ][1]);


                    prim->addVertexNormalTextureCoordinate(vtx,nrm,txc);

                } else {

                    prim->addVertexNormal(vtx,nrm);

                }


			}
		}

		delete [] normals3ds;

	}

	gsMatCache.clear();
	gsTexCache.clear();

	lib3ds_file_free(file3ds);

	return node;
}


TP_TYPE_REGISTER(tpNodeHandler_3DS,tpNodeHandler,NodeHandler_3ds);
tpModuleInitializer<tpNodeHandler_3DS> g_nodefactory_3ds;



