
#include <tp/mat.h>
#include <tp/log.h>
#include <tp/logutils.h>

template <tpUInt components,typename T>
class tpVector : public tpMat<components,1,T>
{
public:
    tpVector() : tpMat<components,1,T>() {}

    tpVector(const tpVector& other) : tpMat<components,1,T>(other) {}

    tpVector& operator = (const tpVector<components,T>& rhs)
    {
        if (this != &rhs)
            for (register tpUInt i = 0; i < tpMat<components,1,T>::cells; ++i) this->m[i] = rhs.m[i];

        return *this;
    }

};


typedef tpVector<3,float> tpVector3f;

int main(int argc, char* argv[])
{
    tpVector3f vf3;
    vf3.setIdentity();
    vf3 = tpVector3f::Identity();



    //(tpVector3f::All(0.f));


    tpLog::get() << "v " << vf3 << "\n";
    tpLog::get() << "v " << tpVector3f::Identity() << "\n";

    return 0;
}
