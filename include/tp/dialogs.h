/*
 * Copyright (C) 1999-2012 Hartmut Seichter
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
#ifndef TP_DIALOGS_H
#define TP_DIALOGS_H

#include <tp/globals.h>
#include <tp/types.h>
#include <tp/string.h>

struct TP_API tpDialogs {

	enum {
		kNo		= 0,
		kYes	= 1,
		kCancel = 2,
		kOk = kYes
	};

	enum {
		kStyleError				= (1 << 1),
		kStyleAlert				= (1 << 2),
		kStyleInfo				= (1 << 3),
		kButtonsOk				= (1 << 4),
		kButtonsYesNo			= (1 << 5),
		kButtonsOkCancel		= (1 << 6),
		kButtonsYesNoCancel		= (1 << 7)
	};

	static tpUInt showMessage(const tpString& caption,
		const tpString& message,
		tpUInt flags);

	static tpString fileSelector(const tpString& caption,
		const tpString& directory,
		const tpString& filter,
		tpUInt flags);
};


#endif
