/*
 * 
 * Copyright 2016 Lucas Tsatiris <chartgeany@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
#include <stdlib.h>
#include <string.h>
#include "ta_libc.h"
#include <QtGui/QApplication>
#include <QTextCodec>
#include <QLocale>
#include <QWebSettings>
#include <QTimer>
#include "appdata.h"
#include "mainwindow.h"

/*
#ifdef Q_OS_WIN
#include <windows.h> 
typedef BOOL (WINAPI *SetProcessDPIAwarePtr)(VOID);
 
INT APIENTRY DllMain(HMODULE hDLL, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH )
    {
        // Make sure we're not already DPI aware
        assert( !IsProcessDPIAware() );
 
        // First get the DPIAware function pointer
        SetProcessDPIAwarePtr lpDPIAwarePointer = (SetProcessDPIAwarePtr)
            GetProcAddress(GetModuleHandle("user32.dll"), 
            "SetProcessDPIAware");
 
        // Next make the page writeable so that we can change the function assembley
        DWORD oldProtect;
        VirtualProtect((LPVOID)lpDPIAwarePointer, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
 
        // write "ret" as first assembly instruction to avoid actually setting HighDPI
        BYTE newAssembly[] = {0xC3};
        memcpy(lpDPIAwarePointer, newAssembly, sizeof(newAssembly));
 
        // change protection back to previous setting.
        VirtualProtect((LPVOID)lpDPIAwarePointer, 1, oldProtect, NULL);
    }
    return TRUE;
}
#endif 
*/

int
main (int argc, char *argv[])
{
   MainWindow *ChartGeanyMain;		
   TA_RetCode retCode;
   int retcode, argc1 = argc;
   char **argv1 = argv;
   
   retCode = TA_Initialize( );
   if( retCode != TA_SUCCESS )
   {
     printf( "Cannot initialize TA-Lib (%d)!\n", retCode );	
     exit (1);
  }
  
// manipulate command line arguments
// -platform windows:fontengine=freetype
#ifdef Q_OS_WIN32
   char arg1[64], arg2[64];

   argc1 = argc + 2;
   argv1 = (char **) malloc((argc1) * sizeof(*argv1));
   for (int counter = 0; counter < argc; counter ++)
     argv1[counter] = argv[counter];

   strcpy (arg1, "-platform");
   argv1[argc] = arg1;

   strcpy (arg2, "windows:fontengine=freetype");
   argv1[argc + 1] = arg2;
#endif
     
  QApplication::setApplicationName (APPNAME);
  QApplication::setOrganizationName (APPNAME);
  QApplication App (argc, argv);
  QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
  QApplication::setStyle("plastique");
  QApplication::setGraphicsSystem ("native");
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

  QWebSettings *settings = QWebSettings::globalSettings();
  settings->setAttribute (QWebSettings::PluginsEnabled, false);
  QLocale::setDefault(QLocale (QLocale::English, QLocale::UnitedStates));
   
  ChartGeanyMain = new MainWindow;
  QObject::connect(&App, SIGNAL(aboutToQuit()), ChartGeanyMain, SLOT(closing()));
  ChartGeanyMain->show ();
  retcode = App.exec ();

  delete ChartGeanyMain;  

  if (argv1 != argv && argv1 != NULL)
    free (argv1);
    
  return retcode;
}
