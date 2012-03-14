#include "dialogs_win32.h"

#include <tp/globals.h>
#include <tp/string.h>

#include <windows.h>


tpUInt tpDialogsWin32::showMessage(const tpString& caption, const tpString& message, tpUInt flags)
{
	UINT _type = 0;

	if ((flags & tpDialogs::kStyleError) != 0) _type |= MB_ICONSTOP;
	if ((flags & tpDialogs::kStyleAlert) != 0) _type |= MB_ICONEXCLAMATION;
	if ((flags & tpDialogs::kStyleInfo) != 0) _type |= MB_ICONINFORMATION;

	if ((flags & tpDialogs::kButtonsOk) != 0) _type |= MB_OK;
	if ((flags & tpDialogs::kButtonsYesNo) != 0) _type |= MB_YESNO;
	if ((flags & tpDialogs::kButtonsOkCancel) != 0) _type |= MB_OKCANCEL;
	if ((flags & tpDialogs::kButtonsYesNoCancel) != 0) _type |= MB_YESNOCANCEL;

	int _result = MessageBox(NULL,
		message.c_str(),
		caption.c_str(),
		_type);

	switch (_result) {
		case IDOK :
		case IDYES :
			_result = tpDialogs::kOk;
			break;
		case IDNO :
			_result = tpDialogs::kNo;
			break;
		case IDCANCEL :
			_result = tpDialogs::kCancel;
			break;
	}

	return _result;
}

tpString 
tpDialogsWin32::fileSelector(const tpString& caption,
							const tpString& directory,
							const tpString& filter,
							tpUInt flags)
{
	tpString result;

#if !defined(_WIN32_WCE)
	tpArray<tpChar> _filename;
	_filename.resize(1024);


	OPENFILENAME _openfile;

	memset(&_openfile,0,sizeof(OPENFILENAME));

#if defined(_WIN32_NT) && (_WIN32_WINNT >= 0x500)
	_openfile.lStructSize = sizeof(OPENFILENAME) - (sizeof(void*)+2*sizeof(DWORD));
#else
	_openfile.lStructSize = sizeof(OPENFILENAME);
#endif
	_openfile.hwndOwner = NULL;
	_openfile.lpstrFilter = filter.c_str();
	_openfile.lpstrInitialDir = directory.c_str();
	_openfile.lpstrTitle = caption.c_str();

	_filename[0] = '\0';
	_openfile.lpstrFile = &_filename[0];
	_openfile.nMaxFile = _filename.getSize();

	BOOL _ok = GetOpenFileName(&_openfile);

	if (_ok) result.set(_filename.getData());

#endif

	return result;
}




#if defined(TWISTEDPAIRCODE)

template <typename T> tpBool TP_DLLEXPORT tpHasFlag(const tpUInt& flags,const T& flag) {

	return ((flags & flag) == flag);
}



tpUInt tpShowMessageBox(const tpString& caption, const tpString& message, tpUInt flags)
{


	UINT _type = 0;

#if 0
	if (tpHasFlag(flags,TP_DIALOG_ERROR)) _type |= MB_ICONSTOP;
	if (tpHasFlag(flags,TP_DIALOG_ALERT)) _type |= MB_ICONEXCLAMATION;
	if (tpHasFlag(flags,TP_DIALOG_INFO)) _type |= MB_ICONINFORMATION;

	if (tpHasFlag(flags,TP_DIALOG_OK)) _type |= MB_OK;
	if (tpHasFlag(flags,TP_DIALOG_YESNO)) _type |= MB_YESNO;
	if (tpHasFlag(flags,TP_DIALOG_OKCANCEL)) _type |= MB_OKCANCEL;
	if (tpHasFlag(flags,TP_DIALOG_YESNOCANCEL)) _type |= MB_YESNOCANCEL;

#endif

	int _result = MessageBox(NULL,
		message.mb_str(),
		caption.mb_str(),
		_type);

	switch (_result) {
		case IDOK :
		case IDYES :
			_result = TP_YES;
			break;
		case IDNO :
			_result = TP_NO;
			break;
		case IDCANCEL :
			_result = TP_CANCEL;
			break;
	}

	return _result;

}


tpString tpShowFileSelector(const tpString& caption,
							const tpString& directory,
							const tpString& filter,
							tpUInt flags)
{

	tpString result;

#if !defined(WINCE)

	tpCharPtr _filename = new tpChar[TP_MAXBUFSIZE];

	OPENFILENAME _openfile;

	memset(&_openfile,0,sizeof(OPENFILENAME));

#if defined(_WIN32_NT) && (_WIN32_WINNT >= 0x500)
	_openfile.lStructSize = sizeof(OPENFILENAME) - (sizeof(void*)+2*sizeof(DWORD));
#else
	_openfile.lStructSize = sizeof(OPENFILENAME);
#endif
	_openfile.hwndOwner = NULL;
	_openfile.lpstrFilter = filter.c_str();
	_openfile.lpstrInitialDir = directory.c_str();
	_openfile.lpstrTitle = caption.c_str();

	*_filename = '\0';
	_openfile.lpstrFile = _filename;
	_openfile.nMaxFile = TP_MAXBUFSIZE;


	BOOL _ok = GetOpenFileName(&_openfile);

	if (_ok) result = _filename;

#endif

	return result;
}

#endif