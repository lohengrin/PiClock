#pragma once

#include <QtCore/QObject>
#include <QtNetwork/QTcpServer>

struct HttpRequest {
    explicit HttpRequest(const QString& in) {parse(in);}
    QString method;
    QString target;
    QString version;

    void parse(const QString& in);
};

using RequestCB = bool(const HttpRequest&);

class HttpServer : public QObject
{
    Q_OBJECT
public:

    explicit HttpServer(QObject *parent = 0, RequestCB* cb = nullptr);
    virtual ~HttpServer();


    QTcpSocket *socket;

public slots:
    void myConnection();

private:
    qint64 bytesAvailable() const;
    QTcpServer *server;
    RequestCB* reqCB;
};
