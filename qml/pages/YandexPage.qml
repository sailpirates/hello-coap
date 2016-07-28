import QtQuick 2.0
import Sailfish.Silica 1.0
import org.sailpirates.components 1.0
import QtMultimedia 5.0

Page {
    id: stt_page

    AudioRecorder {
        id: recorder
        onTextCommandChanged: {
            console.log(textCommand)
            textField.text = textCommand
        }

        onSynthesizedAudioURLChanged: {
            commandMediaPlayer.source = synthesizedAudioURL
        }
    }

    MediaPlayer {
        id: commandMediaPlayer

        onSourceChanged: {
            commandMediaPlayer.play();
        }
    }

    TextField {
        id: textField
        anchors.top: parent.top
        width: parent.width
        placeholderText: recorder.textCommand
    }

    Column {
        width: parent.width
        anchors.bottom: parent.bottom

        Button {
            id: rec_btn
            text: "START RECORDING"

            onClicked: {
                if (recorder.recording) {
                    rec_btn.text = "START RECORDING"
                    console.log('STOP RECORDING');
                    recorder.stop("yandex");
                } else {
                    console.log('START RECORDING');
                    rec_btn.text = "RECORDING... PRESS TO STOP"
                    recorder.record("yandex");
                }
            }
        }

        TextField {
            width: parent.width
            id: text
            enabled: !recorder.recording
            label: "Type command..."
            placeholderText: "Type command"
        }
    }
}
