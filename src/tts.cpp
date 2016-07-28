#include "tts.h"

#include <QUrl>
#include <QDir>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QProcess>
#include <QByteArray>
#include <QBuffer>
#include <QXmlStreamReader>

TTS_Manager::TTS_Manager(QObject *parent):
    QObject(parent),
    _recorder(this),
    _is_recording(false),
    _is_tts_watson(true)
{}

bool TTS_Manager::isRecording() const {
    return _is_recording;
}

QString TTS_Manager::text() const { return _text; }

QString TTS_Manager::getSynthesizedAudioURL()
{
    return synthesizedAudioURL;
}

void TTS_Manager::setTTS(const QString& provider)
{
    _is_tts_watson = (provider.compare("Watson") == 0);
}

void TTS_Manager::startRecord()
{
    static const QString AUDIO_FOLDER = "/home/nemo/.voice_project/";

    if (!QDir(AUDIO_FOLDER).exists()
            && !QDir().mkpath(AUDIO_FOLDER))
        return;

    QString location = AUDIO_FOLDER + "recording-"
            + QDateTime::currentDateTime().toString("yyyyMMddHHmmss");

    QAudioEncoderSettings settings;
    settings.setEncodingMode(QMultimedia::TwoPassEncoding);
    settings.setQuality(QMultimedia::VeryHighQuality);
    if (_is_tts_watson)
    {
        settings.setCodec("audio/FLAC");
        settings.setSampleRate(44100);
        location += ".flac";
        _recorder.setContainerFormat("raw");
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
    _recorder.setOutputLocation(QUrl(location));

    _recorder.record();

    _is_recording = true;
    emit isRecordingChanged();
}

namespace {
QString prepare_command(const QUrl& record_url, bool is_watson) {
    if (is_watson) {
        QString username = "d8faffed-adb3-4553-a652-8ba0d4997301";
        QString password = "CGzOE2hfEobW";
        QUrl cloud_service_url("https://stream.watsonplatform.net/speech-to-text/api/v1/recognize");
        QString query_str("curl -X POST -u %1:%2 --header \"Content-Type: audio/flac\" --data-binary \"@%3\" \"%4\"");
        return query_str.arg(username, password, record_url.toString(), cloud_service_url.toString());
    } else {
        QUrl cloud_service_url("https://asr.yandex.net/asr_xml?"
                       "uuid=bdfd31c86a694aa780873c91602907b7"
                       "&key=4914b813-d96d-4d1f-8f60-849840fffbed"
                       "&topic=notes"
                       "&lang=en-US");
        QString query_str("curl -X POST -H \"Content-Type: audio/x-wav\" --data-binary \"@%1\" \"%2\"");
        return query_str.arg(record_url.toString(), cloud_service_url.toString());
    }
}
}

void TTS_Manager::stopRecord() {
    QUrl record_url = _recorder.outputLocation();
    _recorder.stop();
    _is_recording = false;
    emit isRecordingChanged();

    QString sh_cmd_str = prepare_command(record_url, _is_tts_watson);
    qDebug() << sh_cmd_str;
    QProcess p;
    p.start(sh_cmd_str, QProcess::ReadWrite);
    p.waitForFinished();
    QByteArray curlRes = p.readAllStandardOutput();
    qDebug() << curlRes;

    if (_is_tts_watson)
    {
        QJsonDocument descr = QJsonDocument::fromJson(curlRes);
        _text = descr.object()["results"].toArray()[0]
                .toObject()["alternatives"].toArray()[0]
                .toObject()["transcript"].toString();
        qDebug() << _text;
        // actionOnCommand(_text);
        emit textChanged();

//        synthesizedAudioURL = "https://39898ae0-c9aa-4ade-8849-b0c879544e22:0QO1ON0gBk7U"
//            "@stream.watsonplatform.net/text-to-speech/api/v1/synthesize"
//            "?accept=audio/x-wav"
//            "&text=\"You said %1\""
//            "&voice=en-US_AllisonVoice";
//        synthesizedAudioURL = synthesizedAudioURL.arg(_text);
//        emit synthesizedAudioURLChanged();
    }
    else
    {
        _text = "can't recognize command!";
        QXmlStreamReader xml(curlRes);
        while (!xml.atEnd() && !xml.hasError())
            {
            xml.readNext();
            if (xml.name().compare("variant") == 0)
            {
                _text = xml.readElementText();
                break;
            }
        }
        emit textChanged();
    }
}

//const QString TURN_ON = "turn on";
//const QString TURN_OFF = "turn off";
//const QString DISCOVER = "discover";
//const QString DIE = "goodbye";

//void actionOnCommand(QString textCommand) {
//    QString command = textCommand.toLower().trimmed();
//    if (command == TURN_ON) {

//    } else if (command == TURN_OFF) {

//    } else if (command == DISCOVER) {

//    } else if (command == DIE) {

//    }
//}
