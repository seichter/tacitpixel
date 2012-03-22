#ifndef TP_DIALOGSCOCOA_H
#define TP_DIALOGSCOCOA_H


#include "tp/config.h"

#if defined(TP_USE_COCOA)

#include <tp/dialogs.h>

struct tpDialogsCocoa {

	static tpUInt showMessage(const tpString& caption,
		const tpString& message,
		tpUInt flags);

	static tpString fileSelector(const tpString& caption,
		const tpString& directory,
		const tpString& filter,
		tpUInt flags);
};

#endif

#endif
