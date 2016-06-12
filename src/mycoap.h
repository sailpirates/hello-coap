#ifndef MYCOAP_H
#define MYCOAP_H

#include <QUrl>
#include <QObject>
#include <QPointer>

#include "coapnetworkaccessmanager.h"
#include "coappdu.h"
#include "coapreply.h"

class MyCoAP : public QObject
{
    Q_OBJECT
public:
    explicit MyCoAP(QObject *parent = 0);
    void observe(const QUrl &url);

private:
    CoapNetworkAccessManager *m_coap;
    //QPointer<CoapReply> m_reply;
    QPointer<CoapReply> m_observeReply;
    //QPointer<CoapReply> m_discoverReply;
    QObject *textObject;

private slots:
    void onReplyFinished(CoapReply *reply);
    void onNotificationReceived(const CoapObserveResource &resource, const int &notificationNumber, const QByteArray &payload);
};

#endif // MYCOAP_H
