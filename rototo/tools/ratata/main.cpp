/*
  ==============================================================================

   This file is part of the T.R.P. Engine
   Copyright (c) 2014 - Dominique Dumont

   Permission is granted to use this software under the terms of either:
   a) the GPL v3 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   T.R.P. is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   -----------------------------------------------------------------------------

   To release a closed-source product which uses T.R.P., commercial licenses are
   available: visit veed.fr for more information.

  ==============================================================================
*/
#include "mdimainwindow.h"

#include "myapp.h"

int main(int argc, char *argv[])
{
    MyApp app(argc, argv);
#if defined(Q_OS_WIN32)
    app.addLibraryPath(app.applicationDirPath());
#endif

    MDIMainWindow win;
    app.m_mainWindow = &win;

#if defined(Q_OS_WIN32)


    int my_argc = app.arguments().count();


    if (my_argc == 2)
    {

     QString osfile = app.arguments().at(1);
     if (osfile.endsWith(".rap") == true)
         {
         win.openProjectFromFilename(osfile);
         }
     else
         {
         win.openFileFromFilename(osfile);
         }

    }
#endif
    win.show();

    return app.exec();
}
