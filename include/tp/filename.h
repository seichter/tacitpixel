#ifndef TP_FILENAME_H
#define TP_FILENAME_H 1

#include <tp/string.h>

class TP_API tpFilename : public tpString {
public:

	tpFilename();

	tpFilename(const tpFilename& c);

	tpFilename getExtension() const;

	tpFilename& operator = (const tpString& rhs) {}

};




#endif
