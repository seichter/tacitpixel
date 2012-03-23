#include "tp/dialogs.h"

#include "tp/config.h"

#if defined(_WIN32)
#include "_impl/dialogs_win32.h"
#elif defined(TP_USE_COCOA)
#include "_impl/dialogs_cocoa.h"
#endif


tpUInt tpDialogs::showMessage( const tpString& caption, const tpString& message, tpUInt flags )
{
#if defined(_WIN32)
	return tpDialogsWin32::showMessage(caption,message,flags);
#elif defined(TP_USE_COCOA)
	return tpDialogsCocoa::showMessage(caption,message,flags);
#endif
	return 0;
}

tpString tpDialogs::fileSelector( const tpString& caption, const tpString& directory, const tpString& filter, tpUInt flags )
{
#if defined(_WIN32)
	return tpDialogsWin32::fileSelector(caption,directory,filter,flags);
#elif defined(TP_USE_COCOA)
	return tpDialogsCocoa::fileSelector(caption,directory,filter,flags);
#endif
	return tpString();
}
