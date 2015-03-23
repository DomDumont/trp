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

#include "tcpclient.h"

#include <QMessageBox>


TcpClient::TcpClient(int _port)
{
    port=_port; // choix arbitraire (>1024)
    QObject::connect(&soc,SIGNAL(connected()),this,SLOT(connexion_OK()));
    // signal émis lors de la connexion au serveur
    QObject::connect(&soc, SIGNAL(readyRead()), this, SLOT(lecture()));
    // signal émis lorsque des données sont prêtes à être lues

    QObject::connect(&soc, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
    m_bConnected = false;
}

void TcpClient::onError(QAbstractSocket::SocketError err)
{
    (void) err;
    m_bConnected = false;
    emit vers_IHM_connexion_Lost(); // on envoie un signal à l'IHM
    QMessageBox::warning(NULL, tr("Application"),this->soc.errorString());
}

bool TcpClient::IsConnected()
{
    return m_bConnected ;
}

void TcpClient::ConnectTo(QString _ip)
{
    IP=_ip;
    soc.connectToHost(IP,port); // pour se connecter au serveur
}

void  TcpClient::DisConnect()
{
   soc.close();
   m_bConnected = false;
   emit vers_IHM_connexion_Lost();
}

void TcpClient::SendText(QString t)
{
    QTextStream texte(&soc); // on associe un flux à la socket
    texte << t<<endl;        // on écrit dans le flux le texte saisi dans l'IHM

}
void TcpClient::connexion_OK()
{
    m_bConnected = true;
    emit vers_IHM_connexion_OK(); // on envoie un signal à l'IHM
}
void TcpClient::lecture()
{
    QString ligne;
    while(soc.canReadLine()) // tant qu'il y a quelque chose à lire dans la socket
    {
        ligne = soc.readLine();     // on lit une ligne
        emit vers_IHM_texte(ligne); // on envoie à l'IHM
    }
}
