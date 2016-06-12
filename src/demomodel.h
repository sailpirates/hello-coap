#ifndef DEMOMODEL_H
#define DEMOMODEL_H

#include <QAbstractListModel>

#include "coapnetworkaccessmanager.h"
#include "coappdu.h"
#include "coapreply.h"

class DemoModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DemoRoles {
        NameRole = Qt::UserRole + 1,
    };

    explicit DemoModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex&) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE void activate(const int i);
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void send_light(char value);

    Q_PROPERTY(QString cur_uri READ cur_uri WRITE set_cur_uri NOTIFY cur_uriChanged)
    Q_PROPERTY(int cur_val READ cur_val WRITE set_cur_val NOTIFY cur_valChanged)

    QString cur_uri() const
    {
        return m_cur_uri;
    }

    int cur_val() const
    {
        return m_cur_val;
    }

public slots:
    void set_cur_uri(QString cur_uri)
    {
        if (m_cur_uri == cur_uri)
            return;

        m_cur_uri = cur_uri;
        emit cur_uriChanged(cur_uri);
    }

    void set_cur_val(int cur_val)
    {
        qDebug() << "set cur val" << cur_val;
        if (m_cur_val == cur_val)
            return;

        m_cur_val = cur_val;
        emit cur_valChanged(cur_val);
    }

signals:
    void cur_uriChanged(QString cur_uri);
    void cur_valChanged(int cur_val);

private slots:
    void onReplyFinished(CoapReply *reply);
    void onNotificationReceived(const CoapObserveResource &resource, const int &notificationNumber, const QByteArray &payload);

private:
    CoapNetworkAccessManager m_coap;
    QPointer<CoapReply> m_reply;

    struct DevRes
    {
        QString uri;
        QString name;
    };
    QList<DevRes> m_backing;

    QString m_cur_uri;
    int m_cur_val;
};

#endif // DEMOMODEL_H
