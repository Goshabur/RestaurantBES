import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import Ver 1.0

Drawer {
	id: root
	signal clickAboutUs
	signal clickMenu
	signal clickProfile
	signal clickSignIn
	signal clickRegister
	signal clickLogOut

	Rectangle {
		id: base
		anchors.fill: parent
		color: "lightyellow"



		ColumnLayout {
			anchors.fill: parent
			Label {
				Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
				Layout.topMargin: 15
				text: "Navigation"
                font.pixelSize: 1 + parent.width / 7
                font.bold: true
			}

			Rectangle {
				Layout.fillWidth: true
                Layout.fillHeight: true
                color: "lightpink"

				ColumnLayout {
					width: parent.width
					anchors {
						horizontalCenter: parent.horizontalCenter
					}
					NavButton {
                        Layout.fillWidth: true
                        Layout.preferredHeight: base.width / 7
                        Layout.topMargin: 10
                        Layout.leftMargin: 10
                        Layout.rightMargin: 10
                        text: "Menu"
                        onClicked: root.clickMenu()
                    }
                    NavButton {
                        Layout.fillWidth: true
                        Layout.preferredHeight: base.width / 7
                        Layout.topMargin: 10
                        Layout.leftMargin: 10
                        Layout.rightMargin: 10
                        text: "Profile"
                        onClicked: root.clickProfile()
                    }
					NavButton {
						Layout.fillWidth: true
						Layout.preferredHeight: base.width / 7
						Layout.topMargin: 10
						Layout.leftMargin: 10
						Layout.rightMargin: 10
						text: "Sign in"
						onClicked: root.clickSignIn()
					}
					NavButton {
						Layout.fillWidth: true
                        Layout.preferredHeight: base.width / 7
                        Layout.topMargin: 10
                        Layout.leftMargin: 10
                        Layout.rightMargin: 10
                        text: "Register"
                        onClicked: root.clickRegister()
                    }
				}
			}
		}
	}
}
