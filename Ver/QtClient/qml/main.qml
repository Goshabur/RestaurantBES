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
	        theClient.signInUser(emailInput.text, passwordInput.text)
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
            theClient.registerUser(emailInput.text, passwordInput.text, nameInput.text)
            mainLoader.setSource("ProfilePage.qml", {"popupLoader": popupLoader})
            nameInput.clear()
            emailInput.clear()
            passwordInput.clear()
            close()
        }
    }

	OrderPopup {
		id: orderPopup
		implicitWidth: parent.width * 2/5
        implicitHeight: parent.height *5/7
        anchors.centerIn: parent
	}

    Timer {
        interval: 100
        running: true
        onTriggered: register.open()
    }

    Header {
        id: header
        navigDrawer.onClickProfile: {
            mainLoader.setSource("ProfilePage.qml", {"popupLoader": popupLoader})
        }
        navigDrawer.onClickMenu: {
            mainLoader.source = "MenuPage.qml"
        }
        navigDrawer.onClickSignIn: {
            signIn.open()
        }
        navigDrawer.onClickRegister: {
            register.open()
        }
        cartDrawer.onClickClearCart: {
            theClient.clearCart(Client.NotifyServer)
        }
        cartDrawer.onClickOrder: {
            mainLoader.setSource("OrderPage.qml", {"mainLoader": mainLoader})
        }
        onClickLogo: {
            mainLoader.source = "MenuPage.qml"
        }
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

    Loader {
        id: popupLoader
        anchors.fill: parent
        source: "OrderPopup.qml"
    }
}

