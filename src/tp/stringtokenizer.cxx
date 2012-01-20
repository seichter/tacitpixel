#include <tp/stringtokenizer.h>


tpStringTokenizer::tpStringTokenizer()
{
}


tpStringTokenizer::tpStringTokenizer(const tpString& str, const tpString& delim)
	: mCopy(str)
	, mDelim(delim)
{
}

tpString
tpStringTokenizer::next()
{
	tpString res;
	tpInt pos = mCopy.find(*mDelim.c_str(),false);

	if (pos != tpString::kNotFound)
	{
		res = mCopy.substr(0,pos);
		mCopy = mCopy.substr( pos + mDelim.getLength(), mCopy.getLength() - pos - mDelim.getLength() );
	} else {
		res = mCopy;
		mCopy.empty();
	}

	return res;
}

bool
tpStringTokenizer::finished() const
{
	return (0 == mCopy.getLength());
}


/*static */
tpStringArray
tpStringTokenizer::split(const tpString& str, const tpString& delim)
{
    tpStringTokenizer tkz(str,delim);
    tpStringArray res;
    while (!tkz.finished())
    {
        res.add(tkz.next());
    }
    return res;
}
