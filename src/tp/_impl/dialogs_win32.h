#ifndef TP_DIALOGSWIN32_H
#define TP_DIALOGSWIN32_H


#include <tp/dialogs.h>

struct tpDialogsWin32 {

	static tpUInt showMessage(const tpString& caption, 
		const tpString& message, 
		tpUInt flags);

	static tpString fileSelector(const tpString& caption, 
		const tpString& directory, 
		const tpString& filter, 
		tpUInt flags);
};

#endif
