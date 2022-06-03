import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import Ver 1.0

Popup {
	id: frame
	modal: true
	implicitWidth: parent.width * 2/5
    implicitHeight: parent.height *5/7
    anchors.centerIn: parent
	property int fontSize: 1 + (frame.width + frame.height) / 40
	property int bigFontSize: 1 + (frame.width + frame.height) / 30
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
        var date = a.getDate();
        var hour = a.getHours();
        var min = a.getMinutes();
        var sec = a.getSeconds();
        var time = date + '.' + month + '.' + year + ' ' + hour + ':' + min;
        return time;
    }

	contentItem: Rectangle {
		anchors.fill: parent
		color: "pink"
		ColumnLayout {
			width: parent.width
			height: parent.height
			spacing: 5
			Label {
				Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
				Layout.topMargin: 20
				text: "Order " + order.orderId
				wrapMode: Text.WordWrap
				font.pixelSize: bigFontSize
				font.bold: true
			}
			Label {
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                text: timeConverter(order.timestamp)
                wrapMode: Text.WordWrap
                font.pixelSize: fontSize
                font.bold: false
            }
			Rectangle {
                Layout.preferredWidth: frame.width - 20
                Layout.preferredHeight: frame.height/3
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                color: "lightpink"

                ListView {
                    anchors.fill: parent
                    anchors.margins: 10
                    anchors.centerIn: parent
                    spacing: 20
                    boundsBehavior: Flickable.StopAtBounds
                    clip: true
                    model: MenuModel {
                        id: cartModel
                        menuList: theClient.getMenu()
                        cartList: order.cart
                        displayMode: MenuModel.ShowCart
                    }
                    delegate: Item {
                        id: positionFrame
                        width: frame.width - 20
                        height: (frame.width - 20) / 12
                        anchors.margins: 10
                        anchors {
                            horizontalCenter: parent.horizontalCenter
                        }

                        RowLayout {
                            id: rowLayout
                            anchors.fill: positionFrame
                            anchors.leftMargin: 10
                            anchors.rightMargin: 10
                            Label {
                                id: title
                                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                                wrapMode: Text.WordWrap
                                text: name
                                font.pixelSize: fontSize
                                font.bold: true
                            }
                            Label {
                                id: priceTag
                                Layout.alignment: Qt.AlignRight | Qt.AlignTop
                                wrapMode: Text.WordWrap
                                text: price + "x" + count
                                font.pixelSize: fontSize
                                font.bold: true
                            }
                        }
                        Rectangle {
                            width: parent.width
                            height: 3
                            anchors.bottom: parent.bottom
                            color: "#E0FFFF"
                        }
                    }
                }
            }
            Label {
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                text: "Total cost: " + order.totalCost
                wrapMode: Text.WordWrap
                font.pixelSize: fontSize
                font.bold: false
            }
            Label {
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                text: "Status: " + order.status
                wrapMode: Text.WordWrap
                font.pixelSize: fontSize
                font.bold: false
            }
            Label {
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                width: frame.width
                text: "Address: " + order.address
                wrapMode: Text.WordWrap
                font.pixelSize: fontSize
                font.bold: false
            }
            Label {
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                width: frame.width
                text: "Comment: " + order.comment
                wrapMode: Text.WordWrap
                font.pixelSize: fontSize
                font.bold: false
            }
            Button {
                id: closeButton
                Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
                Layout.margins: 20
                Layout.preferredWidth: root.width *1/3
                Layout.preferredHeight: root.width *1/3 /5
                text: "Close"
                font.pixelSize: fontSize
                font.bold: true
                background: Rectangle {
                    anchors.fill: parent
                    color: "lightyellow"
                }
                onClicked: frame.close()
            }
		}
	}
}