#include "recorder.h"

#include <QUrl>
#include <QDir>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QProcess>
#include <QByteArray>
#include <QBuffer>

#include "hound_driver.h"

const QString AUDIO_FOLDER = "/home/nemo/.voice_project/";

AudioRecorder::AudioRecorder(QObject *parent): QObject(parent) {
    QAudioEncoderSettings audioEncoderSettings;
    audioEncoderSettings.setCodec("audio/speex");
    audioEncoderSettings.setBitRate(16);
    audioEncoderSettings.setQuality(QMultimedia::VeryHighQuality);
    audioEncoderSettings.setSampleRate(16000);
    audioEncoderSettings.setEncodingMode(QMultimedia::TwoPassEncoding);

    qAudioRecorder = new QAudioRecorder(this);
    qAudioRecorder->setContainerFormat("ogg");
    qAudioRecorder->setEncodingSettings(audioEncoderSettings);

    QDir audioDir(AUDIO_FOLDER);
    if (!audioDir.exists()) {
        audioDir.mkpath(AUDIO_FOLDER);
    }
    bRecording = false;
}

const bool& AudioRecorder::recording() const {
    return bRecording;
}

const QString TURN_ON = "turn on";
const QString TURN_OFF = "turn off";
const QString DISCOVER = "discover";
const QString DIE = "goodbye";

void actionOnCommand(QString textCommand) {
    QString command = textCommand.toLower().trimmed();
    if (command == TURN_ON) {

    } else if (command == TURN_OFF) {

    } else if (command == DISCOVER) {

    } else if (command == DIE) {

    }
}

QUrl AudioRecorder::get_raw_audio() {
    if (mediaContentUrls.empty()) {
        return QUrl();
    }
    QUrl url = mediaContentUrls.front();
    mediaContentUrls.pop_front();
    return url;
}

QString AudioRecorder::getTextCommand()
{
    qDebug() << jsonResults.length();
    if (jsonResults.length() > 0) {
        QJsonDocument descr = jsonResults.front();
        jsonResults.pop_front();
        textCommand = descr.object()["results"].toArray()[0].toObject()["alternatives"].toArray()[0].toObject()["transcript"].toString();
        qDebug() << textCommand;

        actionOnCommand(textCommand);
    }
    return textCommand;
}

QString AudioRecorder::getSynthesizedAudioURL()
{
    return synthesizedAudioURL;
}

void AudioRecorder :: record(const QString& mode) {
    qDebug() << mode;

    QDateTime currentDate = QDateTime::currentDateTime();
    QString location = AUDIO_FOLDER + "recording-" + currentDate.toString("yyyyMMddHHmmss");
    location += (mode == "watson")? ".flac": ".wav";

    mediaContentUrls.push_back(QUrl(location));

    if(!QDir(AUDIO_FOLDER).exists()) {
        bool madeDirs = QDir().mkpath(AUDIO_FOLDER);
        if(!madeDirs) {
            return;
        }
    }

    int selectedSampleRate = 44000;
    QAudioEncoderSettings settings;

    if (mode == "watson") {
        settings.setCodec("audio/FLAC");
    } else {
        settings.setCodec("audio/PCM");
    }
    settings.setEncodingMode(QMultimedia::TwoPassEncoding);
    settings.setQuality(QMultimedia::VeryHighQuality);
    settings.setSampleRate(selectedSampleRate);

    qAudioRecorder->setAudioInput("pulseaudio:");
    qAudioRecorder->setEncodingSettings(settings);
    qAudioRecorder->setOutputLocation(QUrl(location));
    // qAudioRecorder->setContainerFormat("wav");

    qAudioRecorder->record();
    bRecording = true;
    emit recordingChanged();
}

void AudioRecorder :: stop(const QString& mode) {
    qAudioRecorder->stop();
    bRecording = false;
    emit recordingChanged();

    if (!mediaContentUrls.empty())
    {
        QUrl audioUrl = mediaContentUrls.front();
        mediaContentUrls.pop_front();
        speechToText(audioUrl, mode);
        emit textCommandChanged();
    }
    synthesizedAudioURL = "https://39898ae0-c9aa-4ade-8849-b0c879544e22:0QO1ON0gBk7U@stream.watsonplatform.net/text-to-speech/api/v1/synthesize?accept=audio/wav&text=\"You said %1\"&voice=en-US_AllisonVoice";
    synthesizedAudioURL = synthesizedAudioURL.arg(textCommand);
    emit synthesizedAudioURLChanged();
}

QString AudioRecorder::speechToText(const QUrl& audioUrl, const QString& mode) {
    QByteArray curlRes = hound_driver(audioUrl.toString(), mode);
    jsonResults.push_back(QJsonDocument::fromJson(curlRes));

    qDebug() << curlRes;
    return "text";
}
