import QtQuick 2.5

Rectangle {
    anchors.fill: parent
    color: "yellow"

    Text {
        text: "Fragment 2"
        color: "white"
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: dp(50)
        font.pixelSize: dp(30)

        renderType: Text.NativeRendering
    }

}

