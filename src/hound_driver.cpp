#include "hound_driver.h"

#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <QUrl>
#include <QString>
#include <QJsonObject>
#include <QByteArray>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QProcess>
#include <QUuid>


QString prepare_command(const QString& audioUrl, const QString& mode) {
    if (mode == "watson") {
        QString username = "d8faffed-adb3-4553-a652-8ba0d4997301";
        QString password = "CGzOE2hfEobW";
        QUrl watsonURL("https://stream.watsonplatform.net/speech-to-text/api/v1/recognize");
        QString url("curl -X POST -u %1:%2 --header \"Content-Type: audio/flac\" --data-binary \"@%3\" \"%4\"");
        return url.arg(username, password, audioUrl, watsonURL.toString());
    } else {
        QUrl yandexURL("https://asr.yandex.net/asr_xml?"
                       "uuid=bdfd31c86a694aa780873c91602907b7"
                       "&key=4914b813-d96d-4d1f-8f60-849840fffbed"
                       "&topic=notes"
                       "&lang=en-US");
        QString url("curl -X POST -H \"Content-Type: audio/x-wav\" --data-binary \"@%1\" \"%2\"");
        return url.arg(audioUrl, yandexURL.toString());
    }
}

extern QByteArray hound_driver(const QString& audioUrl, const QString& mode)
{
    QString url = prepare_command(audioUrl, mode);
    qDebug() << url;
    const char* command = url.toLatin1().data();
    qDebug() << command;
    QProcess p;
    p.start(command, QProcess::ReadWrite);
    p.waitForFinished();

    return p.readAllStandardOutput();
}
