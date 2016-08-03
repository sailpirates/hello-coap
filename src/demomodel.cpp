#include "demomodel.h"
#include <QDebug>
#include <QByteArray>
#include <QNetworkInterface>

DemoModel::DemoModel(QObject *parent) :
    QAbstractListModel(parent),
    m_coap(this)
{
    connect(&m_coap, &CoapNetworkAccessManager::replyFinished, this, &DemoModel::onReplyFinished);
    connect(&m_coap, &CoapNetworkAccessManager::notificationReceived, this, &DemoModel::onNotificationReceived);
}

QHash<int, QByteArray> DemoModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::UserRole] = "name";
    return roles;
}

QVariant DemoModel::data(const QModelIndex &index, int role) const {
    if (index.isValid()
            && (role == Qt::UserRole || role == Qt::DisplayRole))
    {
        const DevRes& devres = m_backing[index.row()];
        QString str = devres.uri + QString(" , ") + devres.name;
        return QVariant(str);
    }
    return QVariant();
}

int DemoModel::rowCount(const QModelIndex &) const
{
    return m_backing.size();
}

void DemoModel::activate(const int i)
{
   if (i < 0 || i >= m_backing.size())
       return;

   m_reply = m_coap.get(CoapRequest
                        (QUrl(m_backing[i].uri)));
   set_cur_uri(m_backing[i].uri);
}


void DemoModel::discover()
{
    foreach (QNetworkInterface iface, QNetworkInterface::allInterfaces())
    {
        int flags = iface.flags();
        if (flags & QNetworkInterface::IsLoopBack)
            continue;
        if (!(flags & QNetworkInterface::IsUp))
            continue;

        foreach (QNetworkAddressEntry addr, iface.addressEntries())
        {
            if (addr.ip().protocol() != QAbstractSocket::IPv4Protocol)
                continue;
            m_reply = m_coap.get(CoapRequest(QUrl("coap://"
                                                  + addr.broadcast().toString()
                                                  + "/.well-known-core")));
            qDebug() << QString("coap://"
                                + addr.broadcast().toString()
                                + "/.well-known-core");
        }
    }

    if (m_backing.size())
    {
        beginRemoveRows(QModelIndex(), 0, m_backing.size() - 1);
        m_backing.clear();
        endRemoveRows();
    }
}

void DemoModel::send_light(char value)
{
    qDebug() << cur_uri();

    m_reply = m_coap.put(CoapRequest(QUrl(cur_uri())),
                         QByteArray( 1, value));
}

void DemoModel::onReplyFinished(CoapReply *reply)
{
    if (reply->error() != CoapReply::NoError) {
        qDebug() << "Error:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    if (reply->statusCode() == CoapPdu::Content)
    {
        qDebug() << "***" << reply->request().url().path();

        if ((reply->request().url().path())
                .compare(QString("/.well-known/core")) == 0)
        {
            DevRes reply_dev_res;
            QString payload_str = QString::fromLatin1(reply->payload());
            qDebug() << payload_str;
            QStringList dev_strlist = payload_str.split(",");
            foreach (QString str, dev_strlist)
            {
                QStringList res_params = str.split(";");
                qDebug() << res_params;
                if (res_params.isEmpty())
                    continue;
                QString res_str = res_params.first();
                qDebug() << res_str;
                if (res_str.length() < 4)
                    continue;
                res_str.chop(1); // chop last'>'
                res_str = res_str.right(res_str.length() - 2); // cutoff first '</'

                reply_dev_res.uri = reply->request().url().toString();
                reply_dev_res.uri.chop(reply->request()
                                       .url().path().length());

                reply_dev_res.uri += QString("/") + res_str;
                qDebug() << reply_dev_res.uri;

                res_params.pop_front();
                if (res_params.isEmpty())
                    continue;
                reply_dev_res.name = res_params.first();
                beginInsertRows(QModelIndex(),
                                m_backing.size(), m_backing.size());
                m_backing.push_back(reply_dev_res);
                endInsertRows();
            }
        }
        else
        {
            qDebug() << "****" << reply->request().url();
            // set_cur_uri(reply->request().url().toString());
            set_cur_val(reply->payload().toInt());
        }
    }

    reply->deleteLater();
    return;
}

void DemoModel::onNotificationReceived(const CoapObserveResource &resource,
                                       const int &notificationNumber,
                                       const QByteArray &payload)
{
    qDebug() << QString("Notification #%1").arg(notificationNumber)
             << "from" << resource.url().toString() << endl
             << QString::fromUtf8(payload) << endl;
}
