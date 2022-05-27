import QtQuick 2.5
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.1
import Ver 1.0

Item {
	id: base
    anchors.fill: parent
    property Loader mainLoader
    //property alias addressInput: addressInput
    //property alias commentInput: commentInput
    //signal clickOrder

    Label {
        id: header
        anchors {
            top: parent.top
            topMargin: parent.width/20
            horizontalCenter: parent.horizontalCenter
        }
        text: "Order form"
        font.bold: true
        font.pixelSize: parent.width/15
    }
    Rectangle {
        id: frame
		anchors {
			topMargin: parent.width/30
			top: header.bottom
			bottom: parent.bottom
			horizontalCenter: parent.horizontalCenter
		}
		color: "pink"
		width: parent.width/2
		ColumnLayout {
            anchors.centerIn: parent
            width: parent.width
            spacing: 10
            Label {
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                text: "Address:"
                font.pixelSize: 20
            }
            Rectangle {
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.preferredWidth: frame.width * 2/3
                Layout.preferredHeight: (frame.width * 2/3) / 15
                color: "white"
                clip: true
                TextInput {
                    id: addressInput
                    anchors.fill: parent
                    anchors.centerIn: parent
                    font.pixelSize: parent.height * 9/10
                    maximumLength: 40
                }
            }

            Label {
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                text: "Comment:"
                font.pixelSize: 20
            }

            Rectangle {
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.preferredWidth: frame.width * 2/3
                Layout.preferredHeight: (frame.width * 2/3) / 15
                color: "white"
                clip: true
                TextInput {
                    id: commentInput
                    anchors.fill: parent
                    anchors.centerIn: parent
                    font.pixelSize: parent.height * 9/10
                    maximumLength: 40
                }
            }

            Rectangle {
                Layout.preferredWidth: frame.width
                Layout.preferredHeight: frame.height * 1/2
                color: "lightpink"

                ListView {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 20
                    boundsBehavior: Flickable.StopAtBounds
                    clip: true
                    model: MenuModel {
                        id: cartModel
                        menuList: theClient.getMenu()
                        cartList: theClient.getCart()
                        displayMode: MenuModel.ShowCart
                    }

                    delegate: Item {
                        id: positionFrame
                        width: frame.width - 20
                        height: (frame.width - 20) / 12
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
                                font.pixelSize: positionFrame.height * 3/5
                                font.bold: true
                            }
                            Label {
                                id: priceTag
                                Layout.alignment: Qt.AlignRight | Qt.AlignTop
                                wrapMode: Text.WordWrap
                                text: price + "x" + count
                                font.pixelSize: positionFrame.height * 3/5
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

            Button {
                id: orderButton
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                Layout.preferredWidth: frame.width *2/5
                Layout.preferredHeight: frame.width *2/5 /5
                text: "Order"
                background: Rectangle {
                    anchors.fill: parent
                    color: "lightyellow"
                }
                onClicked: {
                    addressInput.clear()
                    commentInput.clear()
                    theClient.clearCart()
                    mainLoader.source = "MenuPage.qml"
                    console.log("New order created")
                }
            }
        }
    }
}