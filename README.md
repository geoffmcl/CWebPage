CWebPage Project
-------------

I was looking for ways to create a dialog control that handled and displayed HTML, and came across this great source, posted by Jeff Glatt, on December 12th, 2002 -

https://www.codeguru.com/cpp/i-n/ieprogram/article.php/c4379/Display-a-Web-Page-in-a-Plain-C-Win32-Application.htm

While it is **not** a control in a dialog, it is a great example of a way to create a WIN32 simple window, and display HTML, in essence using the Microsoft `IWebBrowser2` interface, but **read** the very well written, descriptive article, and **study** the source to understand more...

My first effort was the add a [cmake](https://cmake.org/) build file generator, since this supports most versions of MSVC you may have installed, and lots of other windows code generators... and removed the `dsw/dsp` files...

My first wrinkle was in building the `x64`, 64-bit, **WIN64** version. The interface replies on storing a pointer to the interface in the `GWL_USERDATA`, but that had to be changed for `WIN64` to `GWLP_USERDATA`... not very difficult with an `#ifdef WIN64` like macro switch...

Now it can be built either as 32 or 64 bit projects...

Note, the main **aim** of this code is **educational** in nature... just some sample code using the `IWebBrowser2` interface, and is **not** intended as a fully functional application...

**Building**

```
> cd build # or build.x64
> cmake .. [options]
> cmake --build . --config Release
```

The above build folders contain a `build-me.bat`, which should work if you have MSVC14 2015 installed, or with any other installed MSVC with a few minor changes... some warnings may be issued, but should be no particular problem...

**License**

The original `codeguru` zip source does **not** appear to have any `license` statement, it is noted it is more or less repeated on [codeproject](https://www.codeproject.com/Articles/3365/Embed-an-HTML-control-in-your-own-window-using-pla), which implies it is under a `The Code Project Open License (CPOL)`, but to the extent possible this modified source is released under GNU GPL v2 (or later, at your discretion) - See `LICENSE.txt`...

My **big** thanks to Jeff, and enjoy...

Geoff. 20171220

; eof
