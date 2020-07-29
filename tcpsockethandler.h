#ifndef TCPSOCKETHANDLER_H
#define TCPSOCKETHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QHostAddress>
#include "videohandler.h"
#include <vector>
#include "inputstreamhandler.h"
#include <QString>
#include <QByteArray>

class InputStreamHandler;

class TcpSocketHandler : public QObject
{
    Q_OBJECT
public:
    explicit TcpSocketHandler(InputStreamHandler* inputStreamHandler, QString streamId, QString roomId, QHostAddress address, int port = 1338, QObject* parent = nullptr);
    void init();
    void close();
    QByteArray getReply();
    bool isReady();
    void wait();
    int getBytesWritten();
    void writeHeader();
    void writeLeaveSignal();
    QByteArray myHeader;
public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
private:
    //Should match enum in Server::TcpServerHandler
    enum mTcpReturnValues { STREAM_ID_NOT_FOUND, ROOM_ID_NOT_FOUND, SESSION_STARTED };
    QHostAddress mAddress;
    int mPort;
    int mBytesWritten;
    QTcpSocket* mSocket;
    QByteArray mRequest;
    QByteArray mReply;
    bool mReady;
    QString mRoomId;
    QString mStreamId;
    InputStreamHandler* mInputStreamHandler;
    std::vector<QByteArray> videoHeaders;

    void addStream();

};
#endif // TCPSOCKETHANDLER_H