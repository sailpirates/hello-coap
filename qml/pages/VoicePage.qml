import QtQuick 2.0
import Sailfish.Silica 1.0
import org.sailpirates.components 1.0
import QtMultimedia 5.0

Page {
    id: stt_page
    PageHeader { title: "Voice Input" }
    MediaPlayer {
        id: commandMediaPlayer

        onSourceChanged: {
            console.log("source changed");
            commandMediaPlayer.play();
        }
    }
    TTS {
        id: recorder

        onSynthesizedAudioURLChanged: {
            commandMediaPlayer.source = synthesizedAudioURL
        }
    }

    Column {

        width: parent.width
        anchors.bottom: parent.bottom

        ComboBox {
            id: engine_combo
            enabled: !recorder.isRecording
            label: "TTS provider"

            onCurrentItemChanged: {
                recorder.setTTS(currentItem.text);
            }

            menu: ContextMenu {
                MenuItem { text: "Watson" }
                MenuItem { text: "Yandex" }
            }
        }

        Button {
            id: rec_btn
            text: "START RECORDING"

            onClicked: {
                recorder.setTTS(engine_combo.currentItem.text);
                if (recorder.isRecording) {
                    recorder.stopRecord();
                    rec_btn.text = "START RECORDING"
                }
                else {
                    recorder.startRecord();
                    rec_btn.text = "RECORDING... PRESS TO STOP"
                }
            }
        }

        TextField {
            width: parent.width
            id: recognized_text
            enabled: !recorder.isRecording
            label: "Type command..."
            placeholderText: recorder.text
        }
    }
}
