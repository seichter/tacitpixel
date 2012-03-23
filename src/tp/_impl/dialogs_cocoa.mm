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
