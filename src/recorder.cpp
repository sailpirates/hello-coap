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

AudioRecorder::AudioRecorder(QObject *parent):
    QObject(parent),
    _recorder(this),
    _is_recording(false)
{}

const bool& AudioRecorder::recording() const {
    return _is_recording;
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

void AudioRecorder::record(const QString& mode) {
    if (!QDir(AUDIO_FOLDER).exists()
            && !QDir().mkpath(AUDIO_FOLDER))
        return;

    QString location = AUDIO_FOLDER + "recording-"
            + QDateTime::currentDateTime().toString("yyyyMMddHHmmss");

    QAudioEncoderSettings settings;
    if (mode == "watson")
    {
        settings.setCodec("audio/FLAC");
        settings.setEncodingMode(QMultimedia::TwoPassEncoding);
        settings.setQuality(QMultimedia::VeryHighQuality);

        settings.setSampleRate(44100);
        location += ".flac";
    }
    else
    {
        settings.setCodec("audio/PCM");
        settings.setEncodingMode(QMultimedia::TwoPassEncoding);
        settings.setQuality(QMultimedia::VeryHighQuality);

        location += ".wav";
        _recorder.setContainerFormat("wav");
    }
    _recorder.setAudioSettings(settings);
    _recorder.setAudioInput("pulseaudio:");

    QUrl location_url(location);
    _recorder.setOutputLocation(location_url);
    mediaContentUrls.push_back(location_url);

    _recorder.record();

    _is_recording = true;
    emit recordingChanged();
}

void AudioRecorder :: stop(const QString& mode) {
    _recorder.stop();
    _is_recording = false;
    emit recordingChanged();

    if (!mediaContentUrls.empty())
    {
        QUrl audioUrl = mediaContentUrls.front();
        mediaContentUrls.pop_front();
        speechToText(audioUrl, mode);
        emit textCommandChanged();
    }
    synthesizedAudioURL = "https://39898ae0-c9aa-4ade-8849-b0c879544e22:0QO1ON0gBk7U@stream.watsonplatform.net/text-to-speech/api/v1/synthesize?accept=audio/x-wav&text=\"You said %1\"&voice=en-US_AllisonVoice";
    synthesizedAudioURL = synthesizedAudioURL.arg(textCommand);
    emit synthesizedAudioURLChanged();
}

QString AudioRecorder::speechToText(const QUrl& audioUrl, const QString& mode) {
    QByteArray curlRes = hound_driver(audioUrl.toString(), mode);
    jsonResults.push_back(QJsonDocument::fromJson(curlRes));

    qDebug() << curlRes;
    return "text";
}
