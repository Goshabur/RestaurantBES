import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1

Rectangle{
    anchors.leftMargin: parent.width/4
    anchors.rightMargin: parent.width/4
    anchors.topMargin: 100
    anchors.bottomMargin: 100
    anchors.fill: parent
    radius: 100
    color: "#F5F5DC"

    Label {
        anchors.topMargin: parent.height*1/10
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            id: login_label
            text: qsTr("Your Info")
            color: "red"
            font.pixelSize: 30
            font.family: "Arial"
            font.italic: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
        }
    }

    Rectangle{
        anchors.topMargin: parent.height*2/10
        anchors.top: parent.top
        anchors.leftMargin: parent.width/8
        anchors.left: parent.left
        width: 200
        height: 50
        radius: 20
        color: "green"

        Label {

            anchors.fill: parent

            Text {
                text: qsTr("Your First Name")
                font.pixelSize: 15
                font.family: "Arial"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "white"
                font.bold: true
            }
        }
    }


    Rectangle{
        anchors.topMargin: parent.height*2/10
        anchors.top: parent.top
        anchors.leftMargin: parent.width*3/8
        anchors.left: parent.left
        width: 200
        height: 50
        radius: 20
        color: "#F5F5DC"

        Label {

            anchors.fill: parent

            Text {
                text: qsTr("Your First Name")
                font.pixelSize: 15
                font.family: "Arial"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "green"
            }
        }
    }


    Button{

        id: button1
        anchors.topMargin: parent.height*2/10
        anchors.top: parent.top
        anchors.leftMargin: parent.width*5/8
        anchors.left: parent.left
        width: 100
        height: 50


        background: Rectangle{

            anchors.fill: parent
            radius: 20


            color: button1.hovered ? "#FFC0CB" : "green"

            Text {
                text: qsTr("Change")
                font.pixelSize: 15
                font.family: "Arial"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "white"
                font.bold: true
            }

        }

        MouseArea {
            anchors.fill: parent
        }
    }





    Rectangle{
        anchors.topMargin: parent.height*4/10
        anchors.top: parent.top
        anchors.leftMargin: parent.width/8
        anchors.left: parent.left
        width: 200
        height: 50
        radius: 20
        color: "green"

        Label {

            anchors.fill: parent

            Text {
                text: qsTr("Your Last Name")
                font.pixelSize: 15
                font.family: "Arial"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "white"
                font.bold: true
            }
        }
    }


    Rectangle{
        anchors.topMargin: parent.height*4/10
        anchors.top: parent.top
        anchors.leftMargin: parent.width*3/8
        anchors.left: parent.left
        width: 200
        height: 50
        radius: 20
        color: "#F5F5DC"

        Label {

            anchors.fill: parent

            Text {
                text: qsTr("Your First Name")
                font.pixelSize: 15
                font.family: "Arial"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "green"
            }
        }
    }

    Button{
        id: button2
        anchors.topMargin: parent.height*4/10
        anchors.top: parent.top
        anchors.leftMargin: parent.width*5/8
        anchors.left: parent.left
        width: 100
        height: 50


        background: Rectangle{

            anchors.fill: parent
            radius: 20


            color: button2.hovered ? "#FFC0CB" : "green"

            Text {
                text: qsTr("Change")
                font.pixelSize: 15
                font.family: "Arial"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "white"
                font.bold: true
            }

        }

        MouseArea {
            anchors.fill: parent
        }
    }


    Rectangle{
        anchors.topMargin: parent.height*6/10
        anchors.top: parent.top
        anchors.leftMargin: parent.width/8
        anchors.left: parent.left
        width: 200
        height: 50
        radius: 20
        color: "green"

        Label {

            anchors.fill: parent

            Text {
                text: qsTr("Your Email")
                font.pixelSize: 15
                font.family: "Arial"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "white"
                font.bold: true
            }
        }
    }


    Rectangle{
        anchors.topMargin: parent.height*6/10
        anchors.top: parent.top
        anchors.leftMargin: parent.width*3/8
        anchors.left: parent.left
        width: 200
        height: 50
        radius: 20
        color: "#F5F5DC"

        Label {

            anchors.fill: parent

            Text {
                text: qsTr("Your First Name")
                font.pixelSize: 15
                font.family: "Arial"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "green"
            }
        }
    }

    Button{
        id: button3
        anchors.topMargin: parent.height*6/10
        anchors.top: parent.top
        anchors.leftMargin: parent.width*5/8
        anchors.left: parent.left
        width: 100
        height: 50


        background: Rectangle{

            anchors.fill: parent
            radius: 20


            color: button3.hovered ? "#FFC0CB" : "green"

            Text {
                text: qsTr("Change")
                font.pixelSize: 15
                font.family: "Arial"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "white"
                font.bold: true
            }

        }

        MouseArea {
            anchors.fill: parent
        }
    }

    Rectangle{
        anchors.topMargin: parent.height*8/10
        anchors.top: parent.top
        anchors.leftMargin: parent.width/8
        anchors.left: parent.left
        width: 200
        height: 50
        radius: 20
        color: "green"

        Label {

            anchors.fill: parent

            Text {
                text: qsTr("Your Password")
                font.pixelSize: 15
                font.family: "Arial"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "white"
                font.bold: true
            }
        }
    }

    Rectangle{
        anchors.topMargin: parent.height*8/10
        anchors.top: parent.top
        anchors.leftMargin: parent.width*3/8
        anchors.left: parent.left
        width: 200
        height: 50
        radius: 20
        color: "#F5F5DC"

        Label {

            anchors.fill: parent

            Text {
                text: qsTr("Your First Name")
                font.pixelSize: 15
                font.family: "Arial"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "green"
            }
        }
    }

    Button{
        id: button4
        anchors.topMargin: parent.height*8/10
        anchors.top: parent.top
        anchors.leftMargin: parent.width*5/8
        anchors.left: parent.left
        width: 100
        height: 50


        background: Rectangle{

            anchors.fill: parent
            radius: 20


            color: button4.hovered ? "#FFC0CB" : "green"

            Text {
                text: qsTr("Change")
                font.pixelSize: 15
                font.family: "Arial"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "white"
                font.bold: true
            }

        }

        MouseArea {
            anchors.fill: parent
        }
    }
}
