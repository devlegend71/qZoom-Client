#include "tcpsockethandler.h"


TcpSocketHandler::TcpSocketHandler(InputStreamHandler* inputStreamHandler,  QString streamId, QString roomId, QHostAddress address, int port, QObject* parent): QObject(parent)
{
    mAddress = address;
    //mPort = port;
    mPort = 1338;
    qDebug() << mAddress;
    qDebug() << "Tcp port" << mPort;
    mInputStreamHandler = inputStreamHandler;
    mStreamId = streamId;
    mRoomId = roomId;
}

void TcpSocketHandler::init()
{
    mSocket = new QTcpSocket(this);
    /*connect(mSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
*/

    connect(mSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(mSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));

    mSocket->connectToHost(mAddress, mPort);
    //qDebug() << "After ConnectToHost, addr: " << mAddress << " port: " << mPort;
    if(!mSocket->waitForConnected(3000))
    {
        qDebug() << "TcpSocketError: " << mSocket->errorString();
    }
}

void TcpSocketHandler::close()
{
    qDebug() << "Closing TcpSocketHandler";
    mSocket->close();
    delete mSocket;
}

void TcpSocketHandler::readyRead()
{
    qDebug() << "TcpSocket reading incoming message";

    //qDebug() << mSocket->readAll();
    QByteArray data = mSocket->readAll();

    //qDebug() << "Data received: " << data;


    int code = data[0];
    data.remove(0, 1);
    switch(code)
    {
    case 0:
    {
        //HEADER
        int numOfHeaders = data[0];
        qDebug() << "number of headers recieved from server: " << numOfHeaders;
        data.remove(0,1);
        //QString data(reply);


        //qDebug() << "DataString: " << data;
        //qDebug() << reply.indexOf(27);
        for(int i = 0; i < numOfHeaders; i++)
        {
            //qDebug() << "Data: " << data;
            QByteArray temp = QByteArray(data, data.indexOf(27));
            //qDebug() << "Temp: " << temp;
            mInputStreamHandler->handleHeader(temp);
            //qDebug() << "Data before remove endline: " << data;
            data.remove(0, data.indexOf(27)+1);
            //qDebug() << "Data after remove endline: " << data;
        }
        //mSocket->write("0");
        break;
    }

    case 1:
    {
        //REmove the user with this streamId
        qDebug() << "About to remove this user: " << data;
        int streamIdLength = data[0];
        data.remove(0,1);
        QByteArray streamIdArray = QByteArray(data, streamIdLength);
        QString streamId(streamIdArray);
        data.remove(0, streamIdLength);

        mInputStreamHandler->removeStream(streamId);


    }
    default:
        qDebug() << "Dont understand the code sent in beginning of tcp mesasge";
    };
}


//Send header to server, and receive headers from other participants back
void TcpSocketHandler::writeHeader()
{
    //qDebug() << "About to write header";
    //mSocket->connectToHost(mAddress, mPort);



    myHeader.prepend(mStreamId.toLocal8Bit().data());
    myHeader.prepend(mStreamId.size());

    //Puts the roomId and its size at the front of the array
    myHeader.prepend(mRoomId.toLocal8Bit().data());
    myHeader.prepend(mRoomId.size());



   // qDebug() << "My Header: " << myHeader.length() << "\n" << myHeader;

    mSocket->write(myHeader);
    myHeader.clear();
    //mSocket->write("HEAD / HTTP/1.0\r\n\r\n\r\n\r\n");
    //while (mSocket->waitForReadyRead(3000));

    /*QByteArray reply = mSocket->readAll();

    //qDebug() << "Reply from Server: \n" << reply;
    if(reply.size() <= 0)
    {
        qDebug() << "Reply from tcp request was empty or timeout, should not happen @ " << Q_FUNC_INFO;
        return;
    }
    else if(reply.size()==1)
    {
        int returnCode = reply[0];
        switch(returnCode)
        {
        case mTcpReturnValues::SESSION_STARTED:
            //TODO how to handle session started and nothing returned from server
            qDebug() << "Server responded with session started";
            return;
        case mTcpReturnValues::ROOM_ID_NOT_FOUND:
            //TODO handle wrong roomId
            qDebug() << "Server did not find roomId";
            return;
        case mTcpReturnValues::STREAM_ID_NOT_FOUND:
            qDebug() << "Server did not find streamId";
            //TODO handle wrong streamId
            return;
        default:
            qDebug() << "Unkown return code from tcp server @ " << Q_FUNC_INFO;
            exit(-1);
        }
    }*/

    /*int numOfHeaders = reply[0];
    qDebug() << "number of headers recieved from server: " << numOfHeaders;
    reply.remove(0,1);
    //QString data(reply);


    //qDebug() << "DataString: " << data;
    //qDebug() << reply.indexOf(27);


    for(int i = 0; i < numOfHeaders; i++)
    {
        QByteArray temp = QByteArray(reply, reply.indexOf(27));
        //qDebug() << "Temp: " << temp;
        mInputStreamHandler->handleHeader(temp);
        reply.remove(0, reply.indexOf(27));
    }
    */

    //mSocket->write("0");

}

void TcpSocketHandler::sendBumpSignal()
{
    //qDebug() << "Writing bump signal. Denne kommer nok ganske ofte opp :) Heisann folkens.";
    QByteArray header = QString("BUMP").toUtf8();

    header.prepend(mStreamId.toLocal8Bit().data());
    header.prepend(mStreamId.size());

    //Puts the roomId and its size at the front of the array
    header.prepend(mRoomId.toLocal8Bit().data());
    header.prepend(mRoomId.size());

    //mSocket->write(header);
    header.clear();
}


void TcpSocketHandler::connected()
{
    qDebug() << "Tcp socket connected to " << mAddress << "on port " << mPort;
    qDebug() << "TCP Request: " << mRequest;
    //mSocket->write(mRequest);
}

void TcpSocketHandler::disconnected()
{
    qDebug() << "TCPSocket disconnected";
}

void TcpSocketHandler::bytesWritten(qint64 bytes)
{
    qDebug() << "Tcpsocket wrote " << bytes << " bytes";
    mBytesWritten = bytes;
}

void TcpSocketHandler::wait()
{
    while (mSocket->waitForReadyRead(1000));
}

int TcpSocketHandler::getBytesWritten()
{
    return mBytesWritten;
}

QByteArray TcpSocketHandler::getReply()
{
    return mReply;
}

bool TcpSocketHandler::isReady()
{
    return mReady;
}
