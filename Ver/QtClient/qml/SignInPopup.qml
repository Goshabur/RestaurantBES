import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1

Popup {
	id: root
	modal: true
    property alias emailInput: emailInput
    property alias passwordInput: passwordInput
    property alias timer: timer
    property color windowColor: "pink"

    signal clickRegister
    signal clickSignIn

    Timer {
        id: timer
        interval: 200
        running: false
        onTriggered: windowColor = "pink"
    }

	contentItem: Rectangle {
		anchors.fill: parent
		color: windowColor
		ColumnLayout {
			anchors.centerIn: parent
			spacing: 10
			Label {
				Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
				Layout.margins: 20
				text: "Sign In"
				wrapMode: Text.WordWrap
				font.pixelSize: 30
				font.bold: true
			}
			Label {
				Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
				text: "Email:"
				font.pixelSize: 20
			}
			Rectangle {
				Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
				Layout.preferredWidth: root.width *2/3
				Layout.preferredHeight: root.width *2/3 /8
				color: "white"
				clip: true
				TextInput {
                    id: emailInput
                    anchors.fill: parent
                    anchors.centerIn: parent
                    anchors.margins: 10
                    text: "email@gma"
                    font.pixelSize: parent.height *2/3
                    maximumLength: 40
                }
            }

			Label {
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                text: "Password:"
                font.pixelSize: 20
            }

			Rectangle {
				Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
				Layout.preferredWidth: root.width *2/3
				Layout.preferredHeight: root.width *2/3 /8
				color: "white"
				clip: true
				TextInput {
                    id: passwordInput
                    anchors.fill: parent
                    anchors.centerIn: parent
                    anchors.margins: 10
                    passwordMaskDelay: 400
                    echoMode: TextInput.Password
                    text: "lol"
                    font.pixelSize: parent.height *2/3
                    maximumLength: 40
                }
            }

            Button {
                id: signInButton
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.preferredWidth: root.width *1/3
                Layout.preferredHeight: root.width *1/3 /5
                Layout.topMargin: 20
                text: "Sign in"
                background: Rectangle {
                    anchors.fill: parent
                    color: "lightyellow"
                }
                onClicked: clickSignIn()
            }
            Button {
                id: registerButton
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.preferredWidth: root.width *1/3
                Layout.preferredHeight: root.width *1/3 /5
                text: "Register now"
                background: Rectangle {
                    anchors.fill: parent
                    color: "transparent"
                }
                onClicked: clickRegister()
            }
		}
	}
}