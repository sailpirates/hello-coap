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


QVariant DemoModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid()) {
        return QVariant();
    }
    if(role == NameRole) {
        const DevRes& devres = m_backing[index.row()];
        return QVariant(devres.uri
                + QString(" - ")
                + devres.name);
    }
    return QVariant();
}


QHash<int, QByteArray> DemoModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
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
                        (QUrl(QString("coap://")
                              + m_backing[i].uri)));
    // query on


//    QString value = backing[i];

//    // Remove the value from the old location.
//    beginRemoveRows(QModelIndex(), i, i);
//    backing.erase(backing.begin() + i);
//    endRemoveRows();

//    // Add it to the top.
//    beginInsertRows(QModelIndex(), 0, 0);
//    backing.insert(0, value);
//    endInsertRows();
}


void DemoModel::refresh()
{
    QList<QHostAddress> addr_list = QNetworkInterface::allAddresses();
    foreach (QHostAddress addr, addr_list)
    {
        if (addr.protocol() == QAbstractSocket::IPv4Protocol
                && addr != QHostAddress(QHostAddress::LocalHost))
        {
            quint32 nhost = addr.toIPv4Address();
            quint32 n = nhost & 0xFFFFFF00;
            for (quint32 i = 1; i < 0xFF; ++i)
            {
                quint32 ndev = n + i;
                if (ndev == nhost)
                    continue;
                QHostAddress dev_addr(ndev);
                m_reply = m_coap.get(CoapRequest
                                     (QUrl("coap://" + dev_addr.toString()
                                     //      + "/.well-known/core")));
                                     + "/led1")));
            }
        }
    }

    //static int i = 0;
    //++i;
    //m_reply = m_coap.put(CoapRequest(QUrl("coap://172.28.172.5/led1")), QByteArray( 1, i % 2 ? 1 : 255));
    //if (m_reply->isFinished())    {    }

    if (m_backing.size())
    {
        beginRemoveRows(QModelIndex(), 0, m_backing.size() - 1);
        m_backing.clear();
        endRemoveRows();
     }
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
        qDebug() << reply->request().url().path();
        if (reply->request().url().path()
                .compare(QString("/./well-known/core")))
        {
            DevRes reply_dev_res;
            QString payload_str = QString::fromLatin1(reply->payload());
            QStringList dev_strlist = payload_str.split(",");
            foreach (QString str, dev_strlist)
            {
                QStringList res_params = str.split(";");
                if (res_params.isEmpty())
                    continue;
                QString res_str = res_params.first();
                if (res_str.length() < 4)
                    continue;
                res_str.chop(1); // chip last'>'
                res_str.right(res_str.length() - 2); // cutoff first '</'

                reply_dev_res.uri = reply->request().url().toString()
                        + QString("/") + res_str;
                res_params.pop_front();
                if (res_params.isEmpty())
                    continue;
                reply_dev_res.name = res_params.first();
                beginInsertRows(QModelIndex(), m_backing.size(), m_backing.size() + 1);
                m_backing.push_back(reply_dev_res);
                endInsertRows();
            }
        }
        else
        {
            set_cur_uri(reply->request().url().toString());
            set_cur_val(reply->payload().toInt());
        }
    }
    reply->deleteLater();
    return;
}

void DemoModel::onNotificationReceived(const CoapObserveResource &resource, const int &notificationNumber, const QByteArray &payload)
{
    qDebug() << QString("Notification #%1").arg(notificationNumber) << "from" << resource.url().toString() << endl << QString::fromUtf8(payload) << endl;

//    if (textObject) {
//        textObject->setProperty("light", QString::fromUtf8(payload));
//    }
}
