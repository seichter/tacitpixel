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

#include "lib3ds/types.h"
#include "lib3ds/vector.h"
#include "lib3ds/mesh.h"
#include "lib3ds/file.h"

#include "lib3ds/matrix.h"
#include "lib3ds/camera.h"
#include "lib3ds/light.h"
#include "lib3ds/material.h"
#include "lib3ds/node.h"



//tpRefCache<tpTexture> g_texturecache;



tpTransform* createMeshNew( Lib3dsFile* file, Lib3dsMesh* mesh )
{

	Lib3dsMaterial* mat = file->materials;

	// we loop through the materials
	for (;mat;mat = mat->next)
	{
		for (tpUInt i_fcs = 0; i_fcs < mesh->faces; i_fcs++) 
		{
			Lib3dsFace *face = &(mesh->faceL[i_fcs]);
			Lib3dsMaterial *face_mat = lib3ds_file_material_by_name (file, face->material);
			if (face_mat && 0 == tpStrCmp(face_mat->name,mat->name))
			{
				Lib3dsWord *points = face->points;
			}
		}
	}

	return 0;
}



tpTransform* createMesh( Lib3dsFile* file, Lib3dsMesh* mesh )
{

	// get the translation of the mesh
	tpTransform *_transform = new tpTransform(mesh->name);
	tpString mesh_name = "Unnamed";


	tpLogNotify("%s - 0x%x - 0x%x",__FUNCTION__,file,mesh);


	Lib3dsMatrix M;
	
	lib3ds_matrix_copy(M, mesh->matrix);
    lib3ds_matrix_inv(M);

	tpMat44r _mat;
	//_mat.setFromRaw((float*)&mesh->matrix[0][0]);

	_transform->getMatrix() = _mat;

	Lib3dsVector *normalL = 0;
	normalL = (Lib3dsVector*)malloc(3 * sizeof(Lib3dsVector) * mesh->faces);

	if (!normalL || !_transform ) 
	{
		tpLogError("Memory Allocation problem!");
		return NULL;
	}

	lib3ds_mesh_calculate_normals(mesh, normalL);


#if 0

	if ( mesh->texels )
	{
		// Texture Coords
		for (tpUInt i = 0; i < mesh->texels; i++)
		{
			Lib3dsTexel *texel = &(mesh->texelL[i]);
			_mesh->addTexCoord( (*texel)[0], (*texel)[1] );
		}
	}


	for (tpUInt i_fcs = 0; i_fcs < mesh->faces; i_fcs++) 
	{
		Lib3dsFace *face = &(mesh->faceL[i_fcs]);
		Lib3dsWord *points = face->points;
		Lib3dsMaterial *mat = lib3ds_file_material_by_name (file, face->material);

		tpLogNotify("%s - Vertices: %d",__FUNCTION__,mesh->points);

		// Vertices
		for (tpUInt i = 0; i < mesh->points; i++) 
		{
			Lib3dsPoint *point = &(mesh->pointL[i]);

			tpVec3r vertice(
				point->pos[0],
				point->pos[1],
				point->pos[2]
			);

			_mesh->addVertex(vertice);
		}

		// Normal
		Lib3dsVector *normals = (Lib3dsVector *)
			malloc (3 * mesh->faces * sizeof (Lib3dsVector));
		lib3ds_mesh_calculate_normals (mesh, normals);
		for (tpUInt i = 0; i < (mesh->faces * 3); i++) {
			//-normals[i][0], -normals[i][1], -normals[i][2]
			_mesh->addNormal(normals[i][0], normals[i][1], normals[i][2]);
		}
		free (normals);

		// Materials

		if (mat && mat->texture1_map.name) 
		{
			tpString fname = mat->texture1_map.name;

			tpLogMessage("%s image '%s'",__FUNCTION__,fname.c_str());

			tpImage* img = tpImage::load(fname);

			if (img) {

				tpTexture* txture = new tpTexture();

				txture->setName(mat->texture1_map.name);

				tpUInt wrapmode = ( (mat->texture1_map.flags)&LIB3DS_NO_TILE ) ?
TP_TEXTURE_CLAMP : TP_TEXTURE_REPEAT;

				txture->setImage(img);

				txture->setWrap(wrapmode,wrapmode);

				_mesh->setTexture(txture);

			}

		}


		
	}


#else

	tpPrimitive	*_mesh = new tpPrimitive(tpPrimitive::kTriangles);
	for (tpUInt p = 0; p < mesh->faces; ++p)
	{

		Lib3dsFace *f = &mesh->faceL[p];
		Lib3dsMaterial *mat=0;
		Lib3dsTextureMap *_texture=0;

		if (f->material[0]) {
			mat=lib3ds_file_material_by_name(file, f->material);
		}

		if ( mat ) {

			tpMaterial *_material = new tpMaterial(f->material);
			//_material->setName(tpString(mat->name,64));

			tpLogNotify("%s - %s",__FUNCTION__,mat->name);

			_material->setAmbientColor(tpVec4f(0.0f,0.0f,0.0f,1.0f));
			_material->setDiffuseColor(tpVec4f(mat->diffuse[0],
				mat->diffuse[1],
				mat->diffuse[2],
				mat->diffuse[3]));
			_material->setSpecularColor(tpVec4f(mat->specular[0],
				mat->specular[1],
				mat->specular[2],
				mat->specular[3]));

			tpFloat s = (tpFloat)pow(2.0, (double)10.0f * mat->shininess);

			s = tpClamp(s,0.0f,128.0f);

			_material->setShininess(s);

			_mesh->setMaterial(_material);

			if (mat->texture1_map.name) 
			{

				tpLogNotify("%s Face:%d image '%s'",__FUNCTION__,p,mat->texture1_map.name);

				tpString txture_name = mat->texture1_map.name;

				tpTexture* txture = 0;

				//txture = g_texturecache.retrieve(txture_name);

				if (0 == txture)
				{
					txture = new tpTexture();
					//g_texturecache.add(txture_name,txture);

					tpString fname = mat->texture1_map.name;

					tpImage* img = tpImage::read(fname);

					if (img && txture) {					
					
						txture->setName(mat->texture1_map.name);

						tpUInt wrapmode = ( (mat->texture1_map.flags)&LIB3DS_NO_TILE ) ?
							TP_TEXTURE_CLAMP : TP_TEXTURE_REPEAT;

						txture->setImage(img);
						txture->setWrap(wrapmode,wrapmode);

						_mesh->setTexture(txture);

					}

				}

				

			}

		}

		tpVec3r normal(f->normal[0],f->normal[1],f->normal[2]);

		for (tpUInt i = 0; i < 3; i++) 
		{
	
			tpVec3r vertice(
				mesh->pointL[f->points[i]].pos[0],
				mesh->pointL[f->points[i]].pos[1],
				mesh->pointL[f->points[i]].pos[2]
			);
			
			_mesh->addVertex(vertice);
#if 0
			_mesh->addNormal(normal);

			//for (tpUInt i = 0; i < mesh->texels; i++)
			//{
			//	_mesh->addTexCoord(mesh->texelL[f->points[i]][0],mesh->texelL[f->points[i]][1]);
			//}

			if (mesh->texels) _mesh->addTexCoord(mesh->texelL[f->points[i]][0],mesh->texelL[f->points[i]][1]);

#endif
		
		}

		//tpLogMessage("%s - created mesh %d",__FUNCTION__,_mesh->getVertexCount());

		//tpLogNotify("%s %.1f / %.1f / %.f",__FUNCTION__,normal[0],normal[1],normal[2]);
		
	}

	_transform->addChild(_mesh);
		
	if (normalL) {
		free(normalL);
		normalL = 0;
	};

#endif


	return _transform;

}

tpVoid addMesh(Lib3dsFile* file, Lib3dsMesh* mesh, tpNode* group)
{
	tpTransform* _mesh = createMesh(file,mesh);
	if (_mesh && group) group->addChild(_mesh);
}



tpVoid addNodes(Lib3dsFile *file,
					   Lib3dsNode *node,
					   tpNode* group)
{


	Lib3dsNode *p = node->childs;

	while (p) {
		addNodes(file, p, group);
		p = p->next;
	};

	if (node->type == LIB3DS_OBJECT_NODE)
	{

		if (tpStrCmp(node->name,"$$$DUMMY") == 0) return;

		if (!node->user.d)
		{
            tpLogMessage("%s - %s",__FUNCTION__,node->name);

			Lib3dsMesh *mesh = lib3ds_file_mesh_by_name(file, node->name);

			Lib3dsObjectData* object= &node->data.object;

			//tpLogMessage("%s pivot %.1f,%.1f,%.1f",__FUNCTION__,
			//	object->pivot[0],object->pivot[1],object->pivot[2]);

			if (mesh) node->user.d = (tpULong)createMesh(file,mesh);

		}


		if (node->user.d)
		{

            //tpLogMessage("Node: %s",node->name);

			tpTransform *_mesh = (tpTransform*)node->user.d;

			Lib3dsObjectData *d;
			d = &node->data.object;

			tpReal x = (tpReal)-d->pivot[0];
			tpReal y = (tpReal)-d->pivot[1];
			tpReal z = (tpReal)-d->pivot[2];

			tpTransform *_temp = (tpTransform*)_mesh->clone();
			_temp->getMatrix().setTranslation(x,y,z);

			group->addChild(_temp);
		}
	}
}


tpNode* load(const tpString& filename)
{
	tpNode* _root = 0;

	Lib3dsFile *_3dsfile = 0;
	_3dsfile = lib3ds_file_load(filename.c_str());
	

	if (_3dsfile)
	{

		lib3ds_file_eval(_3dsfile,0.0f); 

		_root = new tpNode();

		Lib3dsNode *node = 0;

		if (_3dsfile->nodes)
		{
			for (node = _3dsfile->nodes; node; node = node->next)
				addNodes(_3dsfile, node,_root);

		} else {

			Lib3dsMesh *mesh = _3dsfile->meshes;

			while(mesh)
			{
				addMesh(_3dsfile, mesh,_root);
				mesh = mesh->next;
			}
		}

		lib3ds_file_free(_3dsfile);

	} else {

		tpLogMessage("%s could not load %s",__FUNCTION__,filename.c_str());
	}
	
	return _root;
}

#if 0

class tpNodeFactory3DS : public tpNodeFactory {
public:

	TP_TYPE_DECLARE;

	tpNodeFactory3DS() : tpNodeFactory() 
	{
		tpLogNotify("Twisted Pairs 3DS loader 1.0");
	}

	tpBool getCapabilities(tpUInt capability, const tpString& name)
	{
		switch (capability)
		{
			case TP_NODE_CAN_READ:
				return name.afterLast('.') == "3ds";
				break;

		}

		return false;
	}

	~tpNodeFactory3DS() 
	{
	}

	tpNode* read(const tpString& name) 
	{
		tpLogNotify("tp3DS Loader loading '%s'",name.c_str());
		return load(name.c_str());
	}

};

TP_TYPE_REGISTER(tpNodeFactory3DS,tpNodeFactory,NodeFactory3DS);
TP_MODULE_REGISTER(3ds,tpNodeFactory3DS)

#endif


