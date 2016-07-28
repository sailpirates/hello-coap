#ifndef RECORDER
#define RECORDER

#include <QAudioRecorder>
#include <QQueue>
#include <QUrl>
#include <QJsonDocument>

class AudioRecorder: public QObject {
    Q_OBJECT
    Q_PROPERTY (bool recording READ recording NOTIFY recordingChanged)
    Q_PROPERTY (QString textCommand READ getTextCommand NOTIFY textCommandChanged)
    Q_PROPERTY (QString synthesizedAudioURL READ getSynthesizedAudioURL NOTIFY synthesizedAudioURLChanged)
public:
    AudioRecorder(QObject* parent=0);
    const bool& recording() const;
    QUrl get_raw_audio();
public slots:
    Q_INVOKABLE void record(const QString&);
    Q_INVOKABLE void stop(const QString&);
    Q_INVOKABLE QString getTextCommand();
    Q_INVOKABLE QString getSynthesizedAudioURL();
signals:
    void recordingChanged();
    void textCommandChanged();
    void synthesizedAudioURLChanged();
private:
    QString speechToText(const QUrl& audioUrl, const QString& mode);
    QAudioRecorder _recorder;
    QQueue<QUrl> mediaContentUrls;
    QQueue<QJsonDocument> jsonResults;
    QString textCommand;
    QString synthesizedAudioURL;

    bool _is_recording;
};

#endif // RECORDER

