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

#include "dialogs_cocoa.h"

#if defined(TP_USE_COCOA)

#import <Cocoa/Cocoa.h>

/*static*/
tpUInt
tpDialogsCocoa::showMessage(const tpString& caption,
	const tpString& message,
	tpUInt flags)
{
	NSAlert *alert = [[NSAlert alloc] init];
	NSString *nsmessage = [[NSString alloc] initWithUTF8String:message.c_str()];
	NSString *nscaption = [[NSString alloc] initWithUTF8String:caption.c_str()];

	[alert setInformativeText: nsmessage];
	[alert setMessageText:nscaption];
	[alert runModal];

	[nscaption release];
	[nsmessage release];
	[alert release];


	return 0;
}

/*static*/
tpString
tpDialogsCocoa::fileSelector(const tpString& caption,
	const tpString& directory,
	const tpString& filter,
	tpUInt flags)
{
	NSOpenPanel* opendlg = [NSOpenPanel openPanel];
	[opendlg setCanChooseFiles:YES];

	NSString *nsdir = nil;
	NSString *nsfile = nil;
	if (!directory.isEmpty()) nsdir = [[NSString alloc] initWithUTF8String:directory.c_str()];
	if (!filter.isEmpty()) nsfile = [[NSString alloc] initWithUTF8String:filter.c_str()];

	[opendlg setDirectoryURL:[[NSURL alloc] initFileURLWithPath:nsdir]];

	if (nsfile) [opendlg setNameFieldStringValue:nsfile];

	if (NSOKButton == [opendlg runModal]) {

		NSArray* urls = [opendlg URLs];

		for (size_t i = 0; i < [urls count]; i++) {

			NSString* filename = [urls objectAtIndex:i];

			return tpString([filename UTF8String]);

		}

	}


	return tpString();
}

#endif
