import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1

Popup {
	id: root
	modal: true
	property alias nameInput: nameInput
	property alias emailInput: emailInput
	property alias passwordInput: passwordInput
	signal clickRegister
	signal clickSignIn
	contentItem: Rectangle {
		anchors.fill: parent
		color: "pink"
		ColumnLayout {
			anchors.centerIn: parent
			spacing: 10
			Label {
				Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
				Layout.margins: 20
				text: "Register"
				wrapMode: Text.WordWrap
				font.pixelSize: 30
				font.bold: true
			}

			Label {
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                text: "Name:"
                font.pixelSize: 20
            }

			Rectangle {
				Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
				Layout.preferredWidth: root.width *2/3
				Layout.preferredHeight: root.width *2/3 /8
				color: "white"
				clip: true
				TextInput {
                    id: nameInput
                    anchors.fill: parent
                    anchors.centerIn: parent
                    anchors.margins: 10
                    font.pixelSize: parent.height *2/3
                    maximumLength: 40
                }
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
                    font.pixelSize: parent.height *2/3
                    maximumLength: 40
                }
            }

            Button {
                id: registerButton
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.preferredWidth: root.width *1/3
                Layout.preferredHeight: root.width *1/3 /5
                Layout.topMargin: 20
                text: "Register"
                background: Rectangle {
                    anchors.fill: parent
                    color: "lightyellow"
                }
                onClicked: clickRegister()
            }
            Button {
                id: signInButton
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.preferredWidth: root.width *1/3
                Layout.preferredHeight: root.width *1/3 /5
                text: "Sign In"
                background: Rectangle {
                    anchors.fill: parent
                    color: "transparent"
                }
                onClicked: clickSignIn()
            }
		}
	}
}