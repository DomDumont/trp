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

#include "myapp.h"
#include "mdimainwindow.h"

MyApp::MyApp( int &argc, char **argv )
 : QApplication(argc, argv)
{

}


bool MyApp::event(QEvent *ev)
{
    ///////LogEvents( QString("normal event first  ev->type() = %1").arg(ev->type()));
    /* http://doc.trolltech.com/4.2/qevent.html#Type-enum 170 event resize context ecc...*/
    bool eaten;
    switch (ev->type()) {
    case QEvent::FileOpen:
    {
        QString osfile = static_cast<QFileOpenEvent *>(ev)->file();
        this->m_mainWindow->openProjectFromFilename(osfile);
        eaten = true;
        break;
    }

    default:
        eaten = QApplication::event(ev);
        break;
    }
    return eaten;
}
