import QtQuick 2.5
import QtQuick.Controls 2.0
import Ver 1.0

Rectangle {
	signal clicked
	property alias text: button.text
	Button {
		id: button
        anchors.fill: parent
        background: Rectangle {
            anchors.fill: parent
            color: "white"
        }
        onClicked: parent.clicked()
    }
}