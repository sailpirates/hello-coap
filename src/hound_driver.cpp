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

extern QByteArray hound_driver(QString audioUrl)
{
      QString username = "ba729964-80b5-49f8-bf87-762359a22198";
      QString password = "uFwS1bGI8yUZ";
      QUrl watsonURL("https://stream.watsonplatform.net/speech-to-text/api/v1/recognize");

      QString url("curl -X POST -u %1:%2 --header \"Content-Type: audio/flac\" --data-binary @%3 \"%4\"");

      qDebug() << audioUrl;
      url = url.arg(username, password, audioUrl, watsonURL.toString());

      qDebug() << url;
      const char* command = url.toLatin1().data();
      QProcess p;
      p.start(command, QProcess::ReadWrite);
      p.waitForFinished();

      return p.readAllStandardOutput();
}
