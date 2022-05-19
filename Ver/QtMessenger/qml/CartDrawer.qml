import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import Ver 1.0

Drawer {
	width: parent.width/3
	edge: Qt.RightEdge

	Rectangle {
		id: base
		anchors.fill: parent
		color: "lightyellow"

		ColumnLayout {
			anchors.fill: parent
			Label {
				Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
				Layout.topMargin: 15
				text: "Cart"
                font.pixelSize: 1 + parent.width / 7
                font.bold: true
			}

			Rectangle {
				Layout.fillWidth: true
                Layout.fillHeight: true
                color: "lightpink"

				ListView {
					anchors.fill: parent
					anchors.margins: 20
					spacing: 20
					boundsBehavior: Flickable.StopAtBounds
					clip: true
					model: CartModel {
						list: theCart
					}

					delegate: Item {
						id: frame
                        width: base.width - 20
                        height: (base.width - 20) / 3
                        anchors {
                            horizontalCenter: parent.horizontalCenter
                        }
						Rectangle {
							anchors.fill: parent
							color: "white"
							/*
							Rectangle {
								id: picture
	                            height: frame.height - 20
	                            width: frame.height - 20
	                            anchors {
	                                margins: 20
	                                left: parent.left
	                                verticalCenter: parent.verticalCenter
	                            }
	                            clip: true
	                            Image {
	                                sourceSize.height: parent.height
	                                anchors.centerIn: parent
	                                source: image
	                            }
	                        }
	                        */
                            Label {
                                id: title
                                wrapMode: Text.WordWrap
                                anchors {
                                    left: parent.left
                                    top: parent.top
                                    margins: 10
                                    leftMargin: 20
                                }
                                text: name
                                font.pixelSize: 1 + frame.height /5
                                font.bold: true
                            }
                            Label {
                                id: priceTag
                                anchors {
                                    left: parent.left
                                    top: title.bottom
                                    margins: 10
                                    leftMargin: 20
                                }
                                text: price
                                font.pixelSize: 1 + frame.height /5
                                font.bold: false
                            }

                            // Top button
                            Rectangle {
                                id: topButton
                                width: frame.width/13
                                height: frame.width/13
                                radius: frame.width/13
                                anchors {
                                    bottom: itemCount.top
                                    horizontalCenter: itemCount.horizontalCenter
                                }
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

                            // Number of the items in the cart
                            Label {
                                id: itemCount
                                anchors {
                                    verticalCenter: parent.verticalCenter
                                    right: parent.right
                                    margins: 10
                                    rightMargin: frame.height /4
                                }
                                text: count
                                font.pixelSize: 1 + frame.height /5
                                font.bold: false
                            }

                            // Bottom button
                            Rectangle {
                                id: bottomButton
                                width: frame.width/13
                                height: frame.width/13
                                radius: frame.width/13
                                anchors {
                                    top: itemCount.bottom
                                    horizontalCenter: itemCount.horizontalCenter
                                }
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
						}
					}
				}
			}
		}
	}
}
