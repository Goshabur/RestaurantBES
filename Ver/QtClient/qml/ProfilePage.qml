import QtQuick 2.5
import QtQuick.Controls 2.12
import Ver 1.0

Item {
    anchors.fill: parent

    Label {
        id: header
        anchors {
            top: parent.top
            topMargin: parent.width/20
            horizontalCenter: parent.horizontalCenter
        }
        text: "Profile"
        font.bold: true
        font.pixelSize: parent.width/15
    }
    Rectangle {
		anchors {
			topMargin: parent.width/30
			top: header.bottom
			bottom: parent.bottom
			horizontalCenter: parent.horizontalCenter
		}
		color: "white"
		width: parent.width/2
		Label {
            id: clientName
            anchors {
                top: parent.top
                topMargin: parent.width/20
                horizontalCenter: parent.horizontalCenter
            }
            text: theClient.name
            font.bold: true
            font.pixelSize: parent.width/15
        }
        Label {
            id: clientEmail
            anchors {
                top: clientName.bottom
                topMargin: parent.width/25
                horizontalCenter: parent.horizontalCenter
            }
            text: theClient.email
            font.bold: false
            font.pixelSize: parent.width/17
        }
    }
}