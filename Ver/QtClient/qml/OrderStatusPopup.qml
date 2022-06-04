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

	function timeConverter(UNIX_timestamp){
        var a = new Date(UNIX_timestamp * 1000);
        var months = ['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'];
        var year = a.getFullYear();
        var month = a.getMonth() + 1;
        if (month < 10) month = "0" + month;
        var date = a.getDate();
        if (date < 10) date = "0" + date;
        var hour = a.getHours();
        if (hour < 10) hour = "0" + hour;
        var min = a.getMinutes();
        if (min < 10) min = "0" + min;
        var sec = a.getSeconds();
        if (sec < 10) sec = "0" + sec;
        var time = date + '.' + month + '.' + year + ' ' + hour + ':' + min;
        return time;
    }

    function statusConverter(STATUS) {
        var statuses = ['CREATED', 'ACCEPTED', 'CANCELLED', 'IN PROGRESS', 'COMPLETED'];
        return statuses[STATUS];
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
                text: "New status: " + statusConverter(order.status)
                wrapMode: Text.WordWrap
                font.pixelSize: bigFontSize
                font.bold: true
            }
		}
	}
}