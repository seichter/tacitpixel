#include <tp/ioutils.h>



tpString
tpIOUtility::asString(tpIO* io, tpSizeT bufferSize)
{
	tpString res;

	if (io) {
		tpArray<tpChar> buffer;
		buffer.resize(bufferSize);

		while(io->isGood()) {
			io->read(buffer.getData(),buffer.getSize());

			tpString tmp; tmp.set(buffer.getData(),io->getCount());

			res += tmp;
		}
	}
	return res;
}
