
#include <tp/string.h>
#include <tp/arguments.h>
#include <tp/log.h>
#include <tp/logutils.h>
#include <tp/module.h>
#include <tp/sort.h>
#include <tp/stringtokenizer.h>
#include <tp/node.h>
#include <tp/renderer.h>
#include <tp/window.h>
#include <tp/thread.h>
#include <tp/timer.h>
#include <tp/primitive.h>
#include <tp/vec.h>
#include <tp/light.h>
#include <tp/viewer.h>

#include <GL/glut.h>



tpRefPtr<tpPrimitive> prim;


struct tpGL_TestRig {
    tpRefPtr<tpCamera> camera;

    tpGL_TestRig()
    {

        camera = new tpCamera;
        camera->setProjectionPerspective(45,1.3,1,100);
        camera->setViewLookAt(tpVec3r(2,2,2),tpVec3r(0,0,0),tpVec3r(0,1,0));

        tpLog::get() << "tp (p) " << camera->getProjection() << "\n";
        tpLog::get() << "tp (v) " << camera->getView() << "\n";
        tpLog::get() << "tp (vi) " << camera->getViewInverse() << "\n";
    }


    void setup(unsigned int v)
    {
        switch (v) {

        case 0:
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45,1.3,1,100);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(2,2,2,
                      0,0,0,
                      0,1,0);

            break;

       case 1:
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glLoadMatrixf(camera->getProjection().data());


            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(2,2,2,
                      0,0,0,
                      0,1,0);

            break;

        case 2:

        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glLoadMatrixf(camera->getProjection().data());


            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glLoadMatrixf(camera->getViewInverse().data());
        }
            break;
        case 3:

        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            tpMat44f mvp = tpMat44f::Identity();
            mvp *= camera->getProjection();
            mvp *= camera->getViewInverse();

            glLoadMatrixf(mvp.data());

        }

        }

    }

};


tpGL_TestRig tpgl_testrig;

void display() {

    tpVec4f c(.2,.3,.4,1);
    glClearColor(c[0],c[1],c[2],c[3]);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    tpgl_testrig.setup(3);

    glutSolidTeapot(1);

    glutSwapBuffers();

    tpMat44f m;
    glGetFloatv(GL_PROJECTION_MATRIX,m.data());
    tpLog::get() << "gl (p) " << m << "\n";

    glGetFloatv(GL_MODELVIEW_MATRIX,m.data());
    tpLog::get() << "gl (mv) " << m << "\n";

}


int main(int argc, char* argv[])
{
    tpModuleManager::get()->load("3ds,obj");

    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("tacit pixel + GLUT");


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_DEPTH_TEST);


    glutDisplayFunc(display);

    glutMainLoop();


    return 0;

}
