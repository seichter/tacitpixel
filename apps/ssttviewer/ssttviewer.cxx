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

/* core */
#include <tpGlobal.h>
#include <tpLog.h>
#include <tpString.h>
#include <tpTypes.h>
#include <tpMatrix.h>
#include <tpFixed.h>
#include <tpDynamicLibrary.h>
#include <tpTimer.h>
#include <tpModule.h>
#include <tpSystem.h>

/* sg */
#include <tpNode.h>
#include <tpGroup.h>
#include <tpMesh.h>
#include <tpRenderer.h>
#include <tpSphere.h>
#include <tpLight.h>
#include <tpCamera.h>
#include <tpRenderSurface.h>
#include <tpTransform.h>
#include <tpProjection.h>


#include <sstt/sstt.h>


/* GLOBALS */
tpRefPtr<tpImage> g_bgimg;
tpRefPtr<tpTransform> g_target;

sstt_capture* capture(0);
sstt_image image;
sstt_calibration* calibration(0);
sstt_tracker* tracker(0);
sstt_target* target(0);


tpUInt g_imagecopysize = 0;


tpString loadConfig()
{
	tpString res;
	tpString cfg = tpSystem::get()->findFile("twisted.cfg");
	if (!cfg.isEmpty())
	{
		FILE* f = fopen(cfg.c_str(),"r");
		if (f)
		{
			while (!feof(f))
			{
				tpChar c = fgetc(f);
				if (c != EOF) res << c;
			}
			fclose(f);
		}
	}
	return res;
}


tpNode* createVideoBackground(int& img_width, int& img_height)
{

	sstt_capture_create(&capture, SSTT_CAPTURE_DEFAULT );

	sstt_capture_setting cap_setting;

	memset(&cap_setting,0,sizeof(sstt_capture_setting));

	cap_setting.minFPS = 30;
	cap_setting.minWidth = 400;
	cap_setting.minHeight = 240;
	cap_setting.flip_control = -2;

	sstt_capture_open(capture, &cap_setting);

	/* start the capture device */
	sstt_capture_start( capture );

	sstt_capture_get_image( capture, &image, 0 );

	tpLogMessage("%s - %dx%d",__FUNCTION__,image.width,image.height);

	sstt_capture_get_image( capture, 0, 0 );

	// yikes
	g_imagecopysize = image.width * image.height * 3;


	img_width = image.width; img_height = image.height;

	tpMesh* mesh = new tpMesh( "Sprite", TP_MESH_TRIANGLE_STRIP);

	g_bgimg = new tpImage();
	g_bgimg->create( image.data, image.width, image.height, TP_RGB);

	tpReal potH, potV;

	g_bgimg->getPowerOfTwoRatio(&potH,&potV);


	tpReal x = 1, y = 1, z = -5; 

	mesh->addVertex( -x,	y,		z); 
	mesh->addNormal(0,0,tpReal(1.0)); //1
	
	mesh->addVertex( -x,	-y,		z); 
	mesh->addNormal(0,0,tpReal(1.0)); //2

	mesh->addVertex( x,	 	 y,		z); 
	mesh->addNormal(0,0,tpReal(1.0)); //3

	mesh->addVertex( x , 	-y,		z); 
	mesh->addNormal(0,0,tpReal(1.0)); //4


	mesh->addTexCoord(0, 0 );
	mesh->addTexCoord(0, potV);
	mesh->addTexCoord(potH , 0);
	mesh->addTexCoord(potH , potV);


	

	tpTexture* tex = new tpTexture("Tex");
		
	tex->setImage(g_bgimg.get());
	//tex->setWrap(TP_TEXTURE_CLAMP,TP_TEXTURE_CLAMP);
	
	mesh->setTexture(tex);

	tpProjection* transform = new tpProjection();


	tpReal aspect = image.width / image.height;

	tpMatrix<tpReal> p;
	p.setOrtho(-aspect/4,aspect/4, -1,1, 1,10);

	//p.dump(TP_LOG_WARNING);

	transform->setProjectionMatrix(p);
	
	transform->addNode(mesh);
	transform->setDepthTest( false );

	return transform;
}


tpNode* createForeground( int width, int height )
{
	sstt_calibration_create( &calibration );
	sstt_calibration_load( calibration, 0 );	
	sstt_calibration_set_size( calibration, width, height );

	tpMatrix<tpFloat> p;

	sstt_calibration_get_projection( calibration, p.getData(), 0 );

	tpProjection* proj = new tpProjection;
	proj->setProjectionMatrix(p);


	// now the model view
	g_target = new tpTransform();
	proj->addNode( g_target.get() );	


	tpTransform* scale = new tpTransform();

	// load a file
	tpNode* model = tpNode::load("huedde2_3ds/huette.3ds");
	if (model) { 
		scale->addNode(model);
	} else {
		//getchar();
		tpLogError("File not existing!");
		//exit(-1);
	}

	scale->setScale( tpVec3f( 4,4,4 ) ); 

	g_target->addNode( scale );

	return proj;	
	
}


tpNode* buildScene()
{
	
	int tracker_used = SSTT_TRACKER_FRAME;
	
	
	tpGroup* root = new tpGroup();
	int img_width(0), img_height(0);

	root->addNode(createVideoBackground(img_width,img_height));
	root->addNode(createForeground(img_width,img_height));
	
	tpLogMessage("%s now initializing tracker",__FUNCTION__);

	sstt_tracker_create( &tracker, SSTT_TRACKER_FRAME );
	sstt_tracker_init( tracker, img_width, img_height );
	
	sstt_tracker_set_parameter_i( tracker, SSTT_TRACKER_PREPROCESS, 1 );
	
	if (tracker_used == SSTT_TRACKER_FRAME) {
		
			// create an image
			sstt_tracker_create_target( tracker, &target );

			sstt_target_init(target);

			sstt_target_set_size( target, 60.0, 60.0);
			
			sstt_target_set_parameter_i( target, SSTT_TARGET_ID, 0xe0050e );
				
			sstt_target_set_parameter_f( target, SSTT_TARGET_CONFIDENCE_THRESHOLD, 0.5 );
			sstt_target_set_parameter_i( target, SSTT_TARGET_KALMANCORNER, 1 );
			sstt_target_set_parameter_i( target, SSTT_TARGET_KALMANPOSE, 0 );
			sstt_target_set_parameter_i( target, SSTT_TARGET_SUBPIXREFINE, 1 );

			sstt_target_attach( target, tracker );	

			tpLogMessage("%s - frame id target created",__FUNCTION__); 
		
		
		
	} else {
		
		tpRefPtr<tpImage> target_image = tpImage::load("simple.jpg");

		if (target_image.isValid() && target_image->isValid())
		{


			// create an image
			sstt_tracker_create_target( tracker, &target );
			sstt_target_init(target);

			// Target image
			sstt_image in_img;
			in_img.data = target_image->getData();
			in_img.width = target_image->getWidth();
			in_img.height = target_image->getHeight();
			in_img.channels = 3; 
			in_img.stride = in_img.width * in_img.channels;

			sstt_target_set_image( target, &in_img);
			sstt_target_set_size( target, 36.0, 22.0);

			sstt_target_set_parameter_f( target, SSTT_TARGET_CONFIDENCE_THRESHOLD, 0.5 );
			sstt_target_set_parameter_i( target, SSTT_TARGET_KALMANCORNER, 0 );
			sstt_target_set_parameter_i( target, SSTT_TARGET_KALMANPOSE, 0 );
			sstt_target_set_parameter_i( target, SSTT_TARGET_SUBPIXREFINE, 1 );


			sstt_target_attach( target, tracker );	

			tpLogMessage("%s - target '%s' loaded",__FUNCTION__,target_image->getFileName().c_str()); 
		}
	}
	
	


	return root;
}


#if defined(WIN32) || defined(WINCE)
int main(int argc, tpChar* argv[]) {
#else
int main(int argc, char* argv[]) {
#endif


	tpTimer timer; 
	float blue(0.0f);	
	tpUInt frames(0);

	tpString filename = "";
	tpRefPtr<tpNode> file_node;

	if (argc > 1) {
		/* we need the cast somehow */
		//filename = (const tpWChar*)argv[1];
		filename = argv[1];
		tpLogNotify("%s - looking for %s",__FUNCTION__,filename.c_str());
	}

#if defined(_WIN32_WCE)
	filename = loadConfig();
#endif


	tpLog::get().setLevel(TP_LOG_WARNING);


	tpDynamicLibrary::load("tpMX_OpenGL");
	tpDynamicLibrary::load("tpMX_3DS");
	tpDynamicLibrary::load("tpMX_OBJ");
	tpDynamicLibrary::load("tpMX_BMP");
	tpDynamicLibrary::load("tpMX_JPG");
	tpDynamicLibrary::load("tpMX_Lua");

	tpRenderSurfaceTraits traits;
	traits.setSize(tpVec2i(640,480));
	traits.setTitle("Twisted Pair");

	tpRefPtr<tpRenderSurface> glsurface = tpRenderSurface::create(&traits);
	
	if (!glsurface.isValid()) 
	{
		tpLogError("Need a render surface!");		
		return -1;
	}
	
	glsurface->show(true);
	glsurface->setSceneNode(buildScene());
		
	tpTimer t;
	
	tpInt video_frame(0);

	while ( !glsurface->isDone() ) {

		glsurface->frame();

		frames++;

		// no error when capturing	
		if ( 0 == sstt_capture_update( capture ) )
		{
			sstt_capture_get_image( capture, &image, 0 );
			
			if (video_frame != image.frame)
			{
				g_bgimg->copyData( image.data, g_imagecopysize );

				sstt_target_info* target_info(0);
				sstt_tracker_update( tracker, &image, 0 );
				
				sstt_tracker_detect( tracker );
				
				sstt_tracker_pose( tracker, calibration );

				sstt_target_get_info(target , &target_info );

				tpLogMessage("%s - %3.1f / %3.1f",__FUNCTION__,target_info->confidence,target_info->confidence_threshold);

				if (target_info->confidence > target_info->confidence_threshold)
				{
					tpMatrix<tpReal> mv;				
					mv.setFromRaw( target_info->modelview );				
					g_target->setTransform( mv );		

					//mv.dump(TP_LOG_WARNING);

				}
				
				video_frame = image.frame;
			} 
			
			sstt_capture_get_image( capture, 0, 0 );
		}

		if (frames > 50)
		{
			tpString caption;
			caption.format("Twisted! %3.0f",tpFloat(frames)/timer.getElapsed());
			glsurface->setCaption(caption);
			
			tpLogError("Frames %3.0f",tpFloat(frames)/timer.getElapsed());
			
			//tpImage::save("test.jpg",g_bgimg.get());
			
			timer.start();
			frames = 0;
		}

		tpYield();
	}


	return 0;
}

