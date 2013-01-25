/*
 * Copyright (C) 1999-2013 Hartmut Seichter
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

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
