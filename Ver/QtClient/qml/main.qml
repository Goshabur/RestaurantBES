import QtQuick 2.5
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.1
import Ver 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 1024
    height: 680
    title: "Restaurant BES"
    color: "#E0FFFF"

    // Пересчёт независимых от плотности пикселей в физические пиксели устройства
    readonly property int dpi: Screen.pixelDensity * 25.4
    function dp(x) {
        return (dpi < 120) ? x : x*(dpi/160);
    }

    SignInPopup {
        id: signIn
        width: parent.width * 2/5
        height: parent.height *3/5
        anchors.centerIn: parent
        onClickRegister: {
            close()
            register.open()
        }
        onClickSignIn: {
	        emailInput.clear()
	        passwordInput.clear()
        }
    }

    RegisterPopup {
        id: register
        width: parent.width * 2/5
        height: parent.height *5/7
        anchors.centerIn: parent
        onClickSignIn: {
            close()
            signIn.open()
        }
        onClickRegister: {
            nameInput.clear()
            emailInput.clear()
            passwordInput.clear()
        }
    }

    Timer {
        interval: 100
        running: true
        onTriggered: signIn.open()
    }

    Header {
        id: header
	}

    Loader {
        id: mainLoader
        anchors {
            left: parent.left
            right: parent.right
            top: header.bottom
            bottom: parent.bottom
        }
        source: "MenuPage.qml"
    }
}

