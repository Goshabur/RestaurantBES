import QtQuick 2.5
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.1
import Ver 1.0

Rectangle {
    anchors {
        horizontalCenter: parent.horizontalCenter
    }
    width: parent.width/2
    color: "pink"
    GridView {
        id: menuView
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            topMargin: 0
            leftMargin: 10
            rightMargin: menuView.anchors.leftMargin
            bottomMargin: 20
        }
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        model: MenuModel {
            menuList: menu
            cartList: theCart
        }
        cellWidth: parent.width/2 - menuView.anchors.rightMargin
        cellHeight: menuView.cellWidth * 5/4

        delegate: Item {
            width: menuView.cellWidth
            height: menuView.cellHeight
            Rectangle {
                id: frame
                anchors {
                    fill: parent
                    leftMargin: 10
                    rightMargin: 10
                    topMargin: 20
                    bottomMargin: 0
                }
                color: "white"

                ColumnLayout {
                    anchors {
                        top: frame.top
                        topMargin: 10
                        horizontalCenter: frame.horizontalCenter
                        leftMargin: 10
                        rightMargin: 10
                    }

                    // Image
                    Rectangle {
                        Layout.preferredWidth: frame.width - 20
                        Layout.preferredHeight: (frame.width - 20) *4/5
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                        clip: true
	                    Image {
	                        sourceSize.height: parent.height
	                        anchors.centerIn: parent
	                        source: image
	                    }
                    }
                    // Name of the dish
                    Label {
	                    Layout.maximumWidth: frame.width - 20
                        wrapMode: Text.WordWrap
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                        text: name
                        font.pointSize: 1 + frame.height /20
                        font.bold: true
                    }
                    // Price
                    Label {
                        Layout.maximumWidth: frame.width *2/3 - 10
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                        text: price
                        font.pointSize: 1 + frame.height /20
                        font.bold: false
                    }
                }

                // Adding/removing items to/from the cart
                RowLayout {
	                anchors {
	                    horizontalCenter: frame.horizontalCenter
	                    bottomMargin: 15
	                    bottom: frame.bottom
	                }
	                spacing: 10

	                // Left button
	                Rectangle {
	                    Layout.preferredWidth: frame.width/10
	                    Layout.preferredHeight: frame.width/10
	                    radius: frame.width/10
	                    color: "pink"
	                    Text {
	                        anchors.centerIn: parent
	                        font.pixelSize: parent.height
	                        text: "-"
	                    }

	                    MouseArea {
	                        anchors.fill: parent
	                        onClicked: {
	                            count = count - 1
	                        }
	                    }
	                }

	                // Number of the items in the cart
	                Label {
	                    Layout.alignment: Qt.AlignHCenter | Qt.AlightVCenter
	                    text: count
	                    font.pointSize: 1 + frame.height /20
	                    font.bold: false
	                }

	                // Right button
	                Rectangle {
	                    Layout.preferredWidth: frame.width/10
	                    Layout.preferredHeight: frame.width/10
	                    radius: frame.width/10
	                    color: "pink"
	                    Text {
	                        anchors.centerIn: parent
	                        font.pixelSize: parent.height
	                        text: "+"
	                    }

	                    MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                count = count + 1
                            }
                        }
	                }
	            }
            }
        }
    }
}