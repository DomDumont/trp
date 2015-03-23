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

#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QtNetwork>

class TcpClient : public QObject
{
    Q_OBJECT

public :
    TcpClient(int _port);
public slots :
    void ConnectTo(QString _ip);  // en provenance de l'IHM et se connecte au serveur
    void DisConnect();
    void SendText(QString t); // en provenance de l'IHM et écrit sur la socket
    bool IsConnected();
private slots :
    void connexion_OK();  // en provenance de la socket et émet un signal vers l'IHM
    void lecture();       // en provenance de la socket, lit la socket, émet un signal vers l'IHM
    void onError(QAbstractSocket::SocketError);

signals :
    void vers_IHM_connexion_OK();
    void vers_IHM_connexion_Lost();
    void vers_IHM_texte(QString);
private :
    QString     IP;
    int         port;
    QTcpSocket  soc;
    bool        m_bConnected;
};

#endif // TCPCLIENT_H
