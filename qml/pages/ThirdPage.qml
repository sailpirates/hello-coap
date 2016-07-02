import QtQuick 2.0
import Sailfish.Silica 1.0
import org.sailpirates.components 1.0
import QtMultimedia 5.0

Page {
    id: page

    AudioRecorder{
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

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        TextField {
            id: text
            anchors.bottom: parent.bottom
            width: parent.width
            label: "Type command..."
            placeholderText: "Type command"
        }

        TextField {
            id: textField
            anchors.top: parent.top
            width: parent.width
            placeholderText: recorder.textCommand
        }

        Button {
            id: rec_btn
            text: "START RECORDING"
            anchors.bottom: text.top
            onClicked: {
                if (recorder.recording) {
                    rec_btn.text = "START RECORDING"
                    console.log('STOP RECORDING');
                    recorder.stop();
                } else {
                    console.log('START RECORDING');
                    rec_btn.text = "RECORDING... PRESS TO STOP"
                    recorder.record();
                }
            }
        }
    }
}
