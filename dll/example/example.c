/* example.c
 
This is a Win32 C application (ie, no MFC, WTL, nor even any C++ -- just plain C) that demonstrates
how to embed a browser "control" (actually, an OLE object) in your own window (in order to display a
web page, or an HTML file on disk). The bulk of the OLE/COM code is in DLL.c which creates a DLL that
we use in this simple app. Furthermore, we use LoadLibrary and GetProcAddress, so our DLL is not
actually loaded until/unless we need it.

NOTE: The DLL we create does not normally use UNICODE strings. If you compile this example as UNICODE,
then you should do the same with DLL.C.
*/


#include <windows.h>
#include <direct.h> /* for _getcwd, ... */
#include <stdio.h> /* for sprintf, ... */
#include "CWebPage.h"	/* Declarations of the functions in DLL.c */

#ifndef DEF_DLL
//#define DEF_DLL "cwebpage.dll"
#ifndef NDEBUG
#define DEF_DLL "Debug\\CWEBPAGEd.dll"
#else
#define DEF_DLL "Release\\CWEBPAGE.dll"
#endif
#endif



// A running count of how many windows we have open that contain a browser object
unsigned char WindowCount = 0;

// The class name of our Window to host the browser. It can be anything of your choosing.
static const TCHAR	ClassName[] = "Browser Example";

// Where we store the pointers to CWebPage.dll's functions
EmbedBrowserObjectPtr		*lpEmbedBrowserObject;
UnEmbedBrowserObjectPtr		*lpUnEmbedBrowserObject;
DisplayHTMLPagePtr			*lpDisplayHTMLPage;
DisplayHTMLStrPtr			*lpDisplayHTMLStr;






/****************************** WindowProc() ***************************
 * Our message handler for our window to host the browser.
 */

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		// Embed the browser object into our host window. We need do this only
		// once. Note that the browser object will start calling some of our
		// IOleInPlaceFrame and IOleClientSite functions as soon as we start
		// calling browser object functions in EmbedBrowserObject().
		if ((*lpEmbedBrowserObject)(hwnd)) return(-1);

		// Another window created with an embedded browser object
		++WindowCount;

		// Success
		return(0);
	}

	if (uMsg == WM_DESTROY)
	{
		// Detach the browser object from this window, and free resources.
		(*lpUnEmbedBrowserObject)(hwnd);

		// One less window
		--WindowCount;

		// If all the windows are now closed, quit this app
		if (!WindowCount) PostQuitMessage(0);

		return(TRUE);
	}

	// NOTE: If you want to resize the area that the browser object occupies when you
	// resize the window, then handle WM_SIZE and use the IWebBrowser2's put_Width()
	// and put_Height() to give it the new dimensions.

	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

void loudBark()
{
    char buf[264];
    if (_getcwd(buf, sizeof(buf))) {
        char info[1024];
        sprintf(info, "Presently running in\r\n%s\r\nand can't open '%s'!\r\n", buf, DEF_DLL);
        strcat(info, "Either copy the DLL here, or change the DEF_DLL macro\r\nto the DLL name, and recompile.");
        MessageBox(0, info, "ERROR", MB_OK);
    }
    else {
        /* can only output a not very helpful message */
        MessageBox(0, "Can't open " DEF_DLL "!", "ERROR", MB_OK);
    }
}

/****************************** WinMain() ***************************
 * C program entry point.
 *
 * This creates a window to host the web browser, and displays a web
 * page.
 */

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hInstNULL, LPSTR lpszCmdLine, int nCmdShow)
{
	HINSTANCE		cwebdll;
	MSG				msg;
	WNDCLASSEX		wc;

	// Load our DLL containing the OLE/COM code. We do this once-only. It's named "cwebpage.dll"
    cwebdll = LoadLibrary(DEF_DLL);
    if (cwebdll)
	{
		// Get pointers to the EmbedBrowserObject, DisplayHTMLPage, DisplayHTMLStr, and UnEmbedBrowserObject
		// functions, and store them in some globals.

		// Get the address of the EmbedBrowserObject() function. NOTE: Only Reginald has this one
		lpEmbedBrowserObject = (EmbedBrowserObjectPtr *)GetProcAddress((HINSTANCE)cwebdll, "EmbedBrowserObject");

		// Get the address of the UnEmbedBrowserObject() function. NOTE: Only Reginald has this one
		lpUnEmbedBrowserObject = (UnEmbedBrowserObjectPtr *)GetProcAddress((HINSTANCE)cwebdll, "UnEmbedBrowserObject");

		// Get the address of the DisplayHTMLPagePtr() function
		lpDisplayHTMLPage = (DisplayHTMLPagePtr *)GetProcAddress((HINSTANCE)cwebdll, "DisplayHTMLPage");

		// Get the address of the DisplayHTMLStr() function
		lpDisplayHTMLStr = (DisplayHTMLStrPtr *)GetProcAddress((HINSTANCE)cwebdll, "DisplayHTMLStr");

        if (!lpEmbedBrowserObject || !lpUnEmbedBrowserObject || !lpDisplayHTMLPage || !lpDisplayHTMLStr) {
            // Free the DLL.
            FreeLibrary(cwebdll);
            MessageBox(0, "Failed to get pointers from " DEF_DLL "!", "ERROR", MB_OK);
            return (-1);
        }
		// Register the class of our window to host the browser. 'WindowProc' is our message handler
		// and 'ClassName' is the class name. You can choose any class name you want.
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.hInstance = hInstance;
		wc.lpfnWndProc = WindowProc;
		wc.lpszClassName = &ClassName[0];
		RegisterClassEx(&wc);

		// Create a window. NOTE: We embed the browser object duing our WM_CREATE handling for
		// this window.
		if ((msg.hwnd = CreateWindowEx(0, &ClassName[0], "An HTML string", WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
							HWND_DESKTOP, NULL, hInstance, 0)))
		{
			// For this window, display a string in the BODY of a web page.
			(*lpDisplayHTMLStr)(msg.hwnd, "<H2><CENTER>HTML string test</CENTER></H2><P><FONT COLOR=RED>This is a <U>HTML string</U> in memory.</FONT>");

			// Show the window.
			ShowWindow(msg.hwnd, nCmdShow);
			UpdateWindow(msg.hwnd);
		}

		// Create another window with another browser object embedded in it.
		if ((msg.hwnd = CreateWindowEx(0, &ClassName[0], "Microsoft's web site", WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
						HWND_DESKTOP, NULL, hInstance, 0)))
		{
			// For this window, display a URL. This could also be a HTML file on disk such as "c:\\myfile.htm".
			(*lpDisplayHTMLPage)(msg.hwnd, "http://www.microsoft.com");

			// Show the window.
			ShowWindow(msg.hwnd, nCmdShow);
			UpdateWindow(msg.hwnd);
		}

		// Do a message loop until WM_QUIT.
		while (GetMessage(&msg, 0, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Free the DLL.
		FreeLibrary(cwebdll);

		return(0);
	}
    loudBark();
	return(-1);
}
