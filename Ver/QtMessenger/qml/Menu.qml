import QtQuick 2.5
import QtQuick.Controls 2.12
import Ver 1.0

Rectangle {
    anchors {
        horizontalCenter: parent.horizontalCenter
    }
    width: parent.width/2
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
        model: MenuModel { list: menu }
		/*
        ListModel {
            ListElement {
                name: "Lasagna"
                colorCode: "light blue"
            }
            ListElement {
                name: "Steak"
                colorCode: "light pink"
            }
        }
        */
        cellWidth: parent.width/2 - menuView.anchors.rightMargin
        cellHeight: parent.width/2

        delegate: Item {
            width: menuView.cellWidth
            height: menuView.cellHeight
            Rectangle {
                anchors {
                    fill: parent
                    leftMargin: 10
                    rightMargin: 10
                    topMargin: 20
                    bottomMargin: 0
                }
                color: "lightblue"
                Label {
                    id: courseName
                    anchors.centerIn: parent
                    text: name
                    font.bold: true
                    font.pixelSize: 20
                }
            }
        }
    }
}