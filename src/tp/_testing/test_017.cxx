
#include <tp/mat.h>
#include <tp/log.h>
#include <tp/logutils.h>

template <tpUInt components,typename T>
class tpVector : public tpMat<components,1,T>
{
public:
    tpVector() : tpMat<components,1,T>() {}

    tpVector(const tpVector& other) : tpMat<components,1,T>(other) {}

    tpVector& operator = (const tpMat<components,1,T>& rhs)
    {
        if (this != &rhs)
            for (register tpUInt i = 0; i < tpMat<components,1,T>::cells; ++i) this->at(i) = rhs.at(i);

        return *this;
    }

    const T& operator()(tpUInt c) const { return this->m[c]; }
    T& operator()(tpUInt c) { return this->m[c]; }

    void
    normalize()
    {
        T norm = getNorm();
        if (norm > 0)
            for (register tpUInt i = 0; i < components; i++) (*this)(i) /= norm;
    }

    T
    getSquaredNorm() const
    {
        T squareLen = 0;
        for (register tpUInt i = 0; i < components; i++) squareLen += (this->m[i] * this->m[i]);
        return squareLen;
    }

    T
    getNorm() const
    {
        return ::sqrt(this->getSquaredNorm());
    }

    T
    dot(const tpVector<components,T>& other) const
    {
        T res = 0;
        for (tpUInt i = 0; i < components; i++) res += (*this)(i) * other(i);
        return res;
    }

    T
    getAngle(const tpVector<components,T>& other) const
    {
        tpVector<components,T> nself(*this); tpVector<components,T> nothr = other;
        nself.normalize(); nothr.normalize();
        return acos( nothr.dot(nself) );
    }

    tpVector<components,T>&
    operator -= (const tpVector& other)
    {
        for (tpUInt i = 0; i < components; ++i) { this->m[i] -= other.m[i]; }
        return *this;
    }

    tpVector<components,T>&
    operator += (const tpVector& other)
    {
        for (tpUInt i = 0; i < components; ++i) { this->m[i] += other.m[i]; }
        return *this;
    }

    tpVector(const tpVector<components-1,T> & small,T v = 0)
    {
        for (tpUInt i = 0; i < components-1;++i) {this->m[i] = small.at(i); }
        (*this)(components-1) = v;
    }

};



typedef tpVector<4,float> tpVector4f;
typedef tpVector<3,float> tpVector3f;
typedef tpVector<2,float> tpVector2f;

int main(int argc, char* argv[])
{
    tpVector3f vf3;
    vf3.setIdentity();
    vf3 = tpVector3f::Identity();

    tpLog::get() << "v " << vf3 << "\n";

    vf3 = tpVector3f::All(1.f);

    tpLog::get() << "v " << vf3 << "\n";

    vf3 = tpVector3f::All(20.f);

    vf3.normalize();

    tpLog::get() << "v " << vf3 << "\n";

    tpVector3f vf3_2; vf3_2(0) = vf3_2(1) = vf3_2(2) = 1;

    vf3_2 -= vf3;

    tpLog::get() << "v " << vf3_2 << "\n";

    vf3 *= 3;

    tpLog::get() << "v " << vf3 << "\n";

    tpVector4f v4f(vf3,1.111);

    tpLog::get() << "v " << v4f << "\n";


    //
    tpMat<3,3,float> A; A.setCellIdValue();
    tpMat<3,3,float> B; B.setCellIdValue();

    tpLog::get() << "A = "<< A << "\n";
    tpLog::get() << "B = "<< B << "\n";


    tpMat<3,3,float> X; X = mul(A,B);
    tpLog::get() << "X = "<< X << "\n";
    X.transpose();
    tpLog::get() << "Xt = "<< X << "\n";


    return 0;
}
