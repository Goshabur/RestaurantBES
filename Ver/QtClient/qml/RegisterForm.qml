import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1

Rectangle{
    anchors.leftMargin: parent.width/3
    anchors.rightMargin: parent.width/3
    anchors.topMargin: 100
    anchors.bottomMargin: 100
    anchors.fill: parent
    radius: 100
    color: "#F5F5DC"

    Label {
        anchors.topMargin: parent.height*2/10
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            id: login_label
            text: qsTr("Register")
            color: "red"
            font.pixelSize: 30
            font.family: "Arial"
            font.italic: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
        }
    }

    Label {

        anchors.topMargin: parent.height*30/100
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            id: please
            text: qsTr("Please fill in the information below:")
            font.pixelSize: 15
            font.family: "Arial"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }


    TextField {
          id: first_name
          placeholderText: qsTr("First name")
          anchors.topMargin: parent.height*35/100
          anchors.top: parent.top
          anchors.horizontalCenter: parent.horizontalCenter

          background: Rectangle {
              implicitWidth: 300
              implicitHeight: 40
              color: first_name.enabled ? "transparent" : "#353637"
              border.color: first_name.enabled ? "#21be2b" : "transparent"
          }
     }



    TextField {
          id: last_name
          placeholderText: qsTr("Last name")
          anchors.topMargin: parent.height*45/100
          anchors.top: parent.top
          anchors.horizontalCenter: parent.horizontalCenter

          background: Rectangle {
              implicitWidth: 300
              implicitHeight: 40
              color: last_name.enabled ? "transparent" : "#353637"
              border.color: last_name.enabled ? "#21be2b" : "transparent"
          }
     }

    TextField {
          id: reg_login
          placeholderText: qsTr("Email")
          anchors.topMargin: parent.height*55/100
          anchors.top: parent.top
          anchors.horizontalCenter: parent.horizontalCenter

          background: Rectangle {
              implicitWidth: 300
              implicitHeight: 40
              color: reg_login.enabled ? "transparent" : "#353637"
              border.color: reg_login.enabled ? "#21be2b" : "transparent"
          }
     }



    TextField {
          id: reg_password
          placeholderText: qsTr("Password")
          anchors.topMargin: parent.height*65/100
          anchors.top: parent.top
          anchors.horizontalCenter: parent.horizontalCenter

          background: Rectangle {
              implicitWidth: 300
              implicitHeight: 40
              color: reg_password.enabled ? "transparent" : "#353637"
              border.color: reg_password.enabled ? "#21be2b" : "transparent"
          }
     }





    Button{

        id: login_button
        anchors.topMargin: parent.height*75/100
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            text: qsTr("CREATE MY ACCOUNT")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color: login_button.hovered ? "red" : "white"
        }

        background: Rectangle {
            implicitWidth: 300
            implicitHeight: 40

            color: login_button.hovered ? "white" : "red"
            radius: 20
        }

        onClicked: loader.loadFragment(6)

    }

    Label {

        id: create_one
        anchors.topMargin: parent.height*83/100
        anchors.top: parent.top
        anchors.leftMargin: 280
        anchors.left: parent.left

        Text {
            text: qsTr("Already have an account ?")
            font.pixelSize: 15
            color: "#C0C0C0"
            font.family: "Arial"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

        }
    }

    Button{
        id: reg
        anchors.topMargin: parent.height*83/100
        anchors.top: parent.top
        anchors.leftMargin: 370
        anchors.left: parent.left

        Text {
            text: qsTr("Login")
            font.pixelSize: 15
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color: reg.hovered ? "#C0C0C0" : "black"
        }

        background: Rectangle {
            implicitWidth: 50
            implicitHeight: 20
            color: "#F5F5DC"
            radius: 20
        }
        onClicked: loader.loadFragment(4)
    }
}
