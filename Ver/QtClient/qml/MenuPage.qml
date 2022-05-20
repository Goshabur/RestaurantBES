import QtQuick 2.5
import QtQuick.Controls 2.12

Item {
    anchors.fill: parent

    Label {
        id: header
        anchors {
            top: parent.top
            topMargin: parent.width/20
            horizontalCenter: parent.horizontalCenter
        }
        text: "Menu"
        font.bold: true
        font.pixelSize: parent.width/15
    }
    Menu {
		anchors {
			top: header.bottom
			topMargin: parent.width/30
			bottom: parent.bottom
		}
    }
}