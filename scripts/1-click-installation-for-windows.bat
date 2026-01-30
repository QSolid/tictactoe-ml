@echo off
REM Attempt to stop any running instance to avoid link errors
taskkill /IM TTT.exe /F >nul 2>&1
timeout /t 1 >nul

gcc src\main.c src\minimax.c src\ML.c src\game_logic.c -o TTT.exe -Isrc -IC:/msys64/mingw64/bin/../include/gtk-3.0 -IC:/msys64/mingw64/bin/../include/pango-1.0 -IC:/msys64/mingw64/bin/../include -IC:/msys64/mingw64/bin/../include/harfbuzz -IC:/msys64/mingw64/bin/../include/cairo -IC:/msys64/mingw64/bin/../include/freetype2 -IC:/msys64/mingw64/bin/../include/pixman-1 -IC:/msys64/mingw64/bin/../include/gdk-pixbuf-2.0 -IC:/msys64/mingw64/bin/../include/libpng16 -IC:/msys64/mingw64/bin/../include/webp -DLIBDEFLATE_DLL -IC:/msys64/mingw64/bin/../include/atk-1.0 -IC:/msys64/mingw64/bin/../include/fribidi -IC:/msys64/mingw64/bin/../include/glib-2.0 -IC:/msys64/mingw64/bin/../lib/glib-2.0/include -LC:/msys64/mingw64/bin/../lib -lgtk-3 -lgdk-3 -lz -lgdi32 -limm32 -lshell32 -lole32 -luuid -lwinmm -ldwmapi -lsetupapi -lcfgmgr32 -lhid -lwinspool -lcomctl32 -lcomdlg32 -lpangocairo-1.0 -lpangowin32-1.0 -lpango-1.0 -lcairo-gobject -lcairo -lharfbuzz -lgdk_pixbuf-2.0 -latk-1.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lintl -Wl,--export-all-symbols -mwindows
