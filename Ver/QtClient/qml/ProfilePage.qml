import QtQuick 2.5
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.1
import Ver 1.0

Item {
	id: base
    anchors.fill: parent
    property Loader popupLoader

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
        id: frame
		anchors {
			top: header.bottom
			bottom: parent.bottom
			horizontalCenter: parent.horizontalCenter
			topMargin: 10
		}
		property int fontSize: 1 + (frame.width + frame.height) / 40
        property int bigFontSize: 1 + (frame.width + frame.height) / 30
		color: "pink"
		width: parent.width/2
		ColumnLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 20
            anchors.top: parent.top
            width: parent.width
            anchors.bottom: parent.bottom
            spacing: 10
            Label {
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                text: theClient.name
                font.pixelSize: frame.bigFontSize
                font.bold: true
            }

            Label {
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                text: theClient.email
                font.pixelSize: frame.fontSize
            }
            Rectangle {
                Layout.preferredWidth: frame.width - 20
                Layout.fillHeight: true
                Layout.margins: 10
                color: "lightpink"
	            ListView {
	                flickDeceleration: 14000000 / parent.height
	                verticalLayoutDirection: ListView.BottomToTop
	                anchors.fill: parent
	                spacing: 20
	                boundsBehavior: Flickable.StopAtBounds
	                clip: true
	                model: OrderModel {
	                    id: cartModel
	                    orderList: theClient.getOrderList()
	                }

	                delegate: Item {
	                    id: positionFrame
	                    width: frame.width - 20
	                    height: (frame.width - 20) / 12
	                    anchors {
	                        horizontalCenter: parent.horizontalCenter
	                    }
	                    Label {
	                        id: dateLabel
	                        anchors.left: parent.left
	                        anchors.verticalCenter: parent.verticalCenter
	                        anchors.margins: 10
	                        wrapMode: Text.WordWrap
	                        text: timeConverter(date)
	                        font.pixelSize: frame.fontSize * 3/4
	                        font.bold: false
	                    }
	                    Label {
	                        id: idLabel
	                        anchors.centerIn: parent
	                        anchors.margins: 10
	                        wrapMode: Text.WordWrap
	                        text: "Order " + id
	                        font.pixelSize: frame.fontSize
	                        font.bold: true
	                    }
	                    Label {
	                        id: statusLabel
	                        anchors.right: parent.right
	                        anchors.verticalCenter: parent.verticalCenter
	                        anchors.margins: 10
	                        wrapMode: Text.WordWrap
	                        text: statusConverter(status)
	                        font.pixelSize: frame.fontSize
	                        font.bold: false
	                    }
	                    Rectangle {
	                        width: parent.width
	                        height: 3
	                        anchors.bottom: parent.bottom
	                        color: "#E0FFFF"
	                    }
	                    MouseArea {
	                        anchors.fill: parent
	                        onClicked: {
	                            var order = theClient.getOrder(id, Client.WindowPopup);
	                            if (order) {
		                            popupLoader.setSource("OrderPopup.qml", {"order": order})
		                            popupLoader.item.open()
	                            }
	                        }
	                    }
	                }
	            }
            }
        }
    }
}