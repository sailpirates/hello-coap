import QtQuick 2.0
import Sailfish.Silica 1.0

import "pages"

ApplicationWindow
{
    initialPage: Component { FirstPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: Orientation.All
    _defaultPageOrientations: Orientation.All
}

/*


    Image {
        anchors.fill: parent
        id: imageOff
        fillMode: Image.PreserveAspectFit
        source: "coap_off.png"
    }

    Image {
        anchors.fill: parent
        id: imageOn
        fillMode: Image.PreserveAspectFit
        source: "coap_on.png"
        visible: (parseInt(rootText.light[7]) % 2)
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }
*/
