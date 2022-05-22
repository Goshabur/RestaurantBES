import QtQuick 2.5
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.1

Rectangle {
    height: parent.height * 15/100
    anchors {
        top: parent.top
        left: parent.left
        right: parent.right
    }
    color: "#FFC0CB"

    CartDrawer {
        id: cartDrawer
        height: parent.height
        width: parent.width/3
        edge: Qt.RightEdge
    }


    Item {
        id: navigation

        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        Rectangle {
            id: navigationIcon
            anchors {
                top: parent.top
                topMargin: parent.height/10
                left: parent.left
                leftMargin: parent.height/10
                bottom: parent.bottom
                bottomMargin: parent.height/10
            }
            width: height
            radius: width
            color: "#F5F5DC"
        }
        Label {
            anchors {
                verticalCenter: parent.verticalCenter
                left: navigationIcon.right
                leftMargin: parent.height/10
            }
            text: "Navigation"
            font.pointSize: 1 + parent.height/4
            font.bold: true
        }
    }


    Item {
        id: logo

        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
        }
        Label {
            anchors.centerIn: parent
            text: "Restaurant\nBES"
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 1 + 2*parent.height/7
            font.bold: true
        }
    }


    Item {
        id: cart

        anchors {
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }
        Rectangle {
            id: cartIcon
            anchors {
                top: parent.top
                topMargin: parent.height/10
                right: parent.right
                rightMargin: parent.height/10
                bottom: parent.bottom
                bottomMargin: parent.height/10
            }
            width: height
            radius: width
            color: "#F5F5DC"

            MouseArea {
                anchors.fill: parent
                onClicked: {
					cartDrawer.open()
                }
            }
        }
        Label {
            anchors {
                verticalCenter: parent.verticalCenter
                right: cartIcon.left
                rightMargin: parent.height/10
            }
            text: "Cart"
            font.pointSize: 1 + parent.height/4
            font.bold: true
        }
    }
}