#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QCamera>
#include <QCameraInfo>
#include <QVideoProbe>
#include <QMediaPlayer>
#include <QMediaRecorder>
#include <QObject>
#include <QVideoFrame>
#include <QCameraImageCapture>
#include <QBuffer>
#include "camerahandler.h"
#include "videohandler.h"
#include "filetest.h"
#include "audiohandler.h"
#include <QCameraViewfinder>
#include <QVariant>
#include <libavutil/opt.h>
#include <libavformat/avformat.h>
#include <QAudioInput>
#include <QFile>
#include "videohandler.h"
#include <QTimer>
#include "streamhandler.h"
#include "playbackhandler.h"
#include "imagehandler.h"
#include <QQuickView>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libavdevice/avdevice.h>
}

#include <QtCore/QCoreApplication>
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);



    ImageHandler* imageHandlerObject = new ImageHandler();
    SocketHandler* socketHandlerObject = new SocketHandler();


    QScopedPointer<ImageHandler> imageHandler(imageHandlerObject);
    QScopedPointer<StreamHandler> streamHandler(new StreamHandler(imageHandlerObject, socketHandlerObject));
    QScopedPointer<PlaybackHandler> playbackHandler(new PlaybackHandler(imageHandlerObject, socketHandlerObject));


    streamHandler->record();
    //streamHandler->finish();
    //QScopedPointer<AudioHandler> audioHandler(new AudioHandler(NULL, NULL));
    engine.rootContext()->setContextProperty("imageHandler", imageHandler.data());
    engine.addImageProvider("live", imageHandler.data());


    //QScopedPointer<VideoHandler> videoHandler(new VideoHandler("/dev/video0", NULL));
    //engine.rootContext()->setContextProperty("VideoHandler", videoHandler.data());
    engine.load(url);


    //videoHandler->init();
    //Denne klassen klarer å lagre audio stream til fil
    //QScopedPointer<AudioHandler> pureAudio(new AudioHandler(NULL,"audioHandler.ismv"));
    //pureAudio->main();

    //Denne klassen klarer å lagre video stream til fil
    //QScopedPointer<filetest> fil(new filetest);
    //fil->main();
    //exit(1337);

    //VideoHandler->init();
    //imageHandler->veryFunStianLoop();


    playbackHandler->start();



    return app.exec();
}
