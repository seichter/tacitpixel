#include <tp/scene.h>


tpScene::tpScene()
	: tpReferenced()
	, mCameras(tpRefCameraArray())
	, mActiveCamera(0)
{
}

tpCamera *
tpScene::getActiveCamera()
{
	if (mCameras.isEmpty())
	{
		tpCamera* camera = new tpCamera();
		addCamera(camera,true);
		mActiveCamera = 0;
	}

	return mCameras[mActiveCamera].get();
}

void
tpScene::setActiveCamera(tpUInt camera)
{
	if (camera < mCameras.getSize())
	{
		mActiveCamera = camera;
	}
}

void
tpScene::addCamera(tpCamera *camera,
						   bool makeActive /*= true*/)
{
	if (makeActive) mActiveCamera = mCameras.getSize();
	mCameras.add(camera);
}

void
tpScene::removeCamera(tpCamera *camera)
{
	mCameras.erase(mCameras.find(camera));
}

void
tpScene::render(tpRenderer& renderer)
{
	renderer(this);
}




