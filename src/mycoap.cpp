#include "mycoap.h"

#include "coaprequest.h"
#include "corelinkparser.h"

#include <QDebug>
#include <QUrlQuery>
#include <QQmlProperty>

MyCoAP::MyCoAP(QObject *parent) : QObject(parent)
{
    // create coap instance
    // Note: there sould only be one per application
    m_coap = new CoapNetworkAccessManager(this);
    connect(m_coap, &CoapNetworkAccessManager::replyFinished, this, &MyCoAP::onReplyFinished);
    connect(m_coap, &CoapNetworkAccessManager::notificationReceived, this, &MyCoAP::onNotificationReceived);
    textObject = parent;
}

void MyCoAP::observe(const QUrl &url)
{
    m_observeReply = m_coap->enableResourceNotifications(CoapRequest(url));
    if (m_observeReply->isFinished()) {
        if (m_observeReply->error() != CoapReply::NoError) {
            qWarning() << "Reply error:" << m_observeReply->errorString();
            m_observeReply->deleteLater();
            exit(-1);
        }
    }
}

void MyCoAP::onReplyFinished(CoapReply *reply)
{
    if (reply->error() != CoapReply::NoError) {
        qDebug() << "Error:" << reply->errorString();
        reply->deleteLater();
        exit(-1);
    }

//    if (reply == m_discoverReply) {
//        qDebug() << "Discovery finished";
//        qDebug() << "------------------------------------------" << endl;
//        CoreLinkParser linkParser(reply->payload());
//        foreach (const CoreLink &link, linkParser.links()) {
//            qDebug() << link;
//        }
/*    } else*/ if (reply == m_observeReply) {
        if (m_observeReply->statusCode() != CoapPdu::Content) {
            qWarning() << reply;
            reply->deleteLater();
            exit(-1);
        }

        qDebug() << "Notifications enabled successfully on resource" << reply->request().url().path();
        qDebug() << QString::fromUtf8(reply->payload()) << endl;
        if (textObject) {
            textObject->setProperty("light", QString::fromUtf8(reply->payload()));
        }

        reply->deleteLater();
        return;

    } /*else if (reply == m_reply) {
        qDebug() << "------------------------------------------" << endl << reply;
    }*/ else {
        qWarning() << "Unknown reply received: Please report a bug if you get this message";
    }

    // Note: don't forget to delete the reply
    reply->deleteLater();
    exit(0);

}

void MyCoAP::onNotificationReceived(const CoapObserveResource &resource, const int &notificationNumber, const QByteArray &payload)
{
    qDebug() << QString("Notification #%1").arg(notificationNumber) << "from" << resource.url().toString() << endl << QString::fromUtf8(payload) << endl;
    //emit signal_notification(QString::fromUtf8(payload));

    if (textObject) {
        textObject->setProperty("light", QString::fromUtf8(payload));
    }

}
