#ifndef __TTS_MANAGER_H__
#define __TTS_MANAGER_G__

#include <QAudioRecorder>
#include <QQueue>
#include <QUrl>
#include <QJsonDocument>

class TTS_Manager: public QObject {
    Q_OBJECT
    Q_PROPERTY (bool isRecording READ isRecording NOTIFY isRecordingChanged)
    Q_PROPERTY (QString text READ text NOTIFY textChanged)
    Q_PROPERTY (QString synthesizedAudioURL READ getSynthesizedAudioURL NOTIFY synthesizedAudioURLChanged)
public:
    TTS_Manager(QObject* parent=0);

    bool isRecording() const;
    QString text() const;
public slots:
    Q_INVOKABLE void setTTS(const QString&);
    Q_INVOKABLE void startRecord();
    Q_INVOKABLE void stopRecord();

    Q_INVOKABLE QString getSynthesizedAudioURL();
signals:
    void isRecordingChanged();
    void textChanged();
    void synthesizedAudioURLChanged();
private:
    QAudioRecorder _recorder;

    QString _text;
    QString synthesizedAudioURL;

    bool _is_recording;
    bool _is_tts_watson;
};

#endif //__TTS_MANAGER_H__

