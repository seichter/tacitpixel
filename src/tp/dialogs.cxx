#include "tp/dialogs.h"

#if defined(_WIN32)
#include "_impl/dialogs_win32.h"
#endif


tpUInt tpDialogs::showMessage( const tpString& caption, const tpString& message, tpUInt flags )
{
#if defined(_WIN32)
	return tpDialogsWin32::showMessage(caption,message,flags);
#endif
	return 0;
}

tpString tpDialogs::fileSelector( const tpString& caption, const tpString& directory, const tpString& filter, tpUInt flags )
{
#if defined(_WIN32)
	return tpDialogsWin32::fileSelector(caption,directory,filter,flags);
#endif
	return tpString();
}
