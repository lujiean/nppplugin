//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "PluginDefinition.h"
#include "menuCmdID.h"

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];
char* g_seltext;

//w
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE /*hModule*/)
{
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
    setCommand(0, TEXT("Hello Notepad++"), hello, NULL, false);
    setCommand(1, TEXT("Hello (with dialog)"), helloDlg, NULL, false);

	ShortcutKey* sk = new ShortcutKey;
	setShortcutKey(sk, false, false, false, VK_F9);
	//setCommand(2, TEXT("myPlugin"), myplugin, sk, false);
	setCommand(2, TEXT("myPlugin"), myplugin1, sk, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//
void hello()
{
    // Open a new document
    ::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);

    // Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

    // Say hello now :
    // Scintilla control has no Unicode mode, so we use (char *) here
    ::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)"Hello, Notepad++!");
}

void helloDlg()
{
    ::MessageBox(NULL, TEXT("Hello, Notepad++!"), TEXT("Notepad++ Plugin Template"), MB_OK);
}

void myplugin() {
	// Get the current scintilla
	int which = -1;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
	if (which == -1)
		return;
	HWND curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

	//do job
	int doc_len = ::SendMessage(curScintilla, SCI_GETLENGTH, NULL, NULL);
	::MessageBox(curScintilla, int2LPTSRT(doc_len), TEXT("doc_len"), MB_OK);

	//SCI_GETSELTEXT
	char* seltext = new char;
	::SendMessage(curScintilla, SCI_GETSELTEXT, NULL, (LPARAM)seltext);
	if (strlen(seltext) == 0)
		seltext = g_seltext;
	else
		g_seltext = seltext;
	//char* str_of = "of";
}

void myplugin1() {
	SearchText(FALSE);
}
void SearchText(bool isSearchForward) {
	// Get the current scintilla
	int which = -1;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
	if (which == -1)
		return;
	HWND curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

	//do job

	//::MessageBox(curScintilla, int2LPTSRT(doc_len), TEXT("doc_len"), MB_OK);

	//SCI_GETSELTEXT
	char* seltext = new char;
	::SendMessage(curScintilla, SCI_GETSELTEXT, NULL, (LPARAM)seltext);
	if (strlen(seltext) == 0)
		seltext = g_seltext;
	else
		g_seltext = seltext;

	//SCI_FINDTEXT
	//::MessageBox(curScintilla, TEXT("SCI_FINDTEXT"), TEXT("SCI_FINDTEXT"), MB_OK);
	int curpos = (int)::SendMessage(curScintilla, SCI_GETCURRENTPOS, NULL, NULL);
	CharacterRange rc;

	if (isSearchForward) {
		rc.cpMin = curpos;
		int doc_len = ::SendMessage(curScintilla, SCI_GETLENGTH, NULL, NULL);
		rc.cpMax = doc_len;
	}
	else {
		rc.cpMin = curpos - strlen(seltext);
		rc.cpMax = 0;
	}

	TextToFind *ft = new TextToFind;
	ft->chrg = rc;
	ft->lpstrText = seltext;

	int pos = (int)::SendMessage(curScintilla, SCI_FINDTEXT, SCFIND_MATCHCASE, (LPARAM)ft);
	int line = 0;

	if (pos != -1) {
		line = (int)::SendMessage(curScintilla, SCI_LINEFROMPOSITION, pos, NULL);

		//::SendMessage(curScintilla, SCI_GOTOLINE, line, NULL);
		//::SendMessage(curScintilla, SCI_GOTOPOS, pos, NULL);
		//::SendMessage(curScintilla, SCI_SETSEL, pos+strlen(seltext), pos);
		int anc = pos + strlen(seltext);
		//::SendMessage(curScintilla, SCI_SETSEL, anc, pos);
		::SendMessage(curScintilla, SCI_SETSEL, pos, anc);
	}
	else
		::MessageBox(curScintilla, TEXT("Notfound"), TEXT("Alter"), MB_OK);


	//char desc[200];
	wchar_t desc[200];
	//wchar_t format
	swprintf(desc, 200, TEXT("curpos: %d. findpos: %d. line: %d"), curpos, pos, line);
	::MessageBox(curScintilla, desc, TEXT("desc"), MB_OK);

	delete ft;
	//delete rc;
}

//int2LPTSTR - convert int to LPTSTR
// https://bbs.csdn.net/topics/380151025
LPTSTR int2LPTSRT(int i) {

	////int to LPCWSTR  
	//int itemp = 123;
	//wchar_t str[10];
	//_itow_s(itemp, str, 10);

	//wchar_t str[10];
	//_itow_s(doc_len, str, 2);

	LPTSTR lpsz = new TCHAR[100];
	_itot(i, lpsz, 10);

	return lpsz;
}

// https://blog.csdn.net/u012669183/article/details/38347557
LPTSTR charp2LPTSRT(char* pchar) {

	// ansi to unicode 
	//char* szAnsi = "abcd1234你我他";
	char* szAnsi = pchar;
	//预转换，得到所需空间的大小 
	int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), NULL, 0);
	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间 
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	//转换 
	::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), wszString, wcsLen);
	//最后加上'\0' 
	wszString[wcsLen] = '\0';
	//unicode版的MessageBox API 
	//::MessageBoxW(GetSafeHwnd(), wszString, wszString, MB_OK);
	return wszString;
}

void setShortcutKey(ShortcutKey* sk, bool isCtrl, bool isAlt, bool isShift, UCHAR key) {
	sk->_isCtrl = isCtrl;
	sk->_isAlt = isAlt;
	sk->_isShift = isShift;
	sk->_key = key;
}