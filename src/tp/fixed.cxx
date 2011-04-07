#include "tp/fixed.h" 


/*

class TP_CORE_API tpSystem : public tpReference 
{
	tpArray< tpString > m_datapaths;
	tpArray< tpString > m_modulepaths;

	tpSystem() 
	{
	}

	tpVoid initialize()
	{
	}

public:

	tpString findFile(const tpString& name) { return ""; }

	static tpSystem* get(bool destroy = false)
	{
		static tpRefPtr<tpSystem> s_assetmanager = new tpSystem();

		if (destroy) {s_assetmanager = 0;}

		return s_assetmanager.get();
	}

};

*/


tpFixed::tpFixed() : val(0x00000000)
{

}

tpFixed::tpFixed( const tpFixed& f )
{
	val = f.val;
}

tpFixed::tpFixed( int v ) : val(tpFixedFromInt(v))
{
}

int tpFixed::toInt( bool roundUp /*= false*/ ) const
{
	return tpFixedToInt( val );
}

double tpFixed::toDouble() const
{
	return tpFixedToDouble( val );
}

float tpFixed::toFloat() const
{
	return tpFixedToFloat( val );
}

tpFixed tpFixed::mult( tpFixed f )
{
	return tpMultiplyFixed(f.val, val);
}

tpFixed tpFixed::abs() const
{
	tpFixed t; 
	( val < 0 ) ? t.val = -val : t.val = val; 
	return t;
}