#include "HttpServer.h"

#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QTcpSocket>

#include <iostream>

void HttpRequest::parse(const QString& in)
{
    QStringList res = in.split(' ');
    method = res[0];
    target = res[1];
    version = res[3];
}


HttpServer::HttpServer(QObject *parent, RequestCB* cb) : 
    QObject(parent),
    reqCB(cb)
{
     server = new QTcpServer(this);
    // waiting for the web brower to make contact,this will emit signal
    connect(server, SIGNAL(newConnection()),this, SLOT(myConnection()));
    if(!server->listen(QHostAddress::Any,8090))
        std::cout<< "\nWeb server      could not start";
    else 
        std::cout<<"\nWeb server is waiting for a connection on port 8090" << std::endl;
}

void HttpServer::myConnection()
{
    socket = server->nextPendingConnection();
    while(!(socket->waitForReadyRead(100)));  //waiting for data to be read from web browser

    char webBrowerRXData[1000];
    int sv=socket->read(webBrowerRXData,1000);

    bool ok = true;

    if (reqCB)
    {
        HttpRequest req(webBrowerRXData);
        ok = reqCB(req);
    }

    socket->write("HTTP/1.1 200 OK\r\n");       // \r needs to be before \n
    socket->write("Content-Type: text/html\r\n");
    socket->write("Connection: close\r\n");
    socket->write("\r\n");
    socket->write("<!DOCTYPE html>\r\n");
    if (ok)
	    socket->write("<html><body>OK<br><a href=\"/weather\">Weather</a><br><a href=\"/lcd\">Lcd</a></body>\n</html>\n");
    else
	    socket->write("<html><body>NOK<br><a href=\"/weather\">Weather</a><br><a href=\"/lcd\">Lcd</a></body>\n</html>\n");

    socket->flush();
    connect(socket, SIGNAL(disconnected()),socket, SLOT(deleteLater()));
    socket->disconnectFromHost();
}

HttpServer::~HttpServer()
{
    socket->close();
}
