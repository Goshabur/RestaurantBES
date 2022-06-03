import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import Ver 1.0

Popup {
	id: frame
	modal: false
	implicitWidth: parent.width
    implicitHeight: parent.height
	property int fontSize: 1 + (frame.width + frame.height) / 30
	property int bigFontSize: 1 + (frame.width + frame.height) / 20
	property Order order: Order {
		orderId: 123
		timestamp: 13245
		totalCost: 30
		status: 1
		address: "Somewhere in Bremen"
		comment: "Everything is good"
		cart: theClient.getCart()
	}

	Timer {
		id: timer
        interval: 2000
        running: false
        onTriggered: frame.close()
    }

    Connections {
        target: frame
        onOpened: timer.start()
    }

	contentItem: Rectangle {
		anchors.fill: parent
		color: "white"
		ColumnLayout {
			width: parent.width
			//height: parent.height
			Label {
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.topMargin: 20
                text: "Order " + order.orderId + ": status changed!"
                wrapMode: Text.WordWrap
                font.pixelSize: bigFontSize
                font.bold: true
            }
            Label {
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                text: "New status: " + order.status
                wrapMode: Text.WordWrap
                font.pixelSize: bigFontSize
                font.bold: true
            }
		}
	}
}