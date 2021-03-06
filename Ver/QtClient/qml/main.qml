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
	        var success = theClient.signInUser(emailInput.text, passwordInput.text)
            if (success) {
                mainLoader.setSource("ProfilePage.qml", {"popupLoader": popupLoader})
                passwordInput.clear()
                emailInput.clear()
                close()
            } else {
                windowColor = "red"
                timer.start()
            }
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
            var success = theClient.registerUser(emailInput.text, passwordInput.text, nameInput.text)
            if (success) {
                mainLoader.setSource("ProfilePage.qml", {"popupLoader": popupLoader})
	            nameInput.clear()
	            emailInput.clear()
	            passwordInput.clear()
	            close()
            } else {
				windowColor = "red"
				timer.start()
			}
        }
    }

    Timer {
        interval: 100
        running: true
        onTriggered: signIn.open()
    }

    Header {
        id: header
        navigDrawer.onClickProfile: {
            mainLoader.setSource("ProfilePage.qml", {"popupLoader": popupLoader})
            navigDrawer.close()
        }
        navigDrawer.onClickMenu: {
            mainLoader.source = "MenuPage.qml"
            navigDrawer.close()
        }
        navigDrawer.onClickSignIn: {
            signIn.open()
            navigDrawer.close()
        }
        navigDrawer.onClickRegister: {
            register.open()
            navigDrawer.close()
        }
        cartDrawer.onClickClearCart: {
            theClient.clearCart(Client.NotifyServer)
            cartDrawer.close()
        }
        cartDrawer.onClickOrder: {
            if (theClient.regStatus) {
                mainLoader.setSource("OrderPage.qml", {"mainLoader": mainLoader})
            } else {
                signIn.open()
            }
            cartDrawer.close()
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

    Loader {
	    id: notificationLoader
	    width: parent.width * 2/6
        height: parent.height *2/12
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
	    source: "OrderStatusPopup.qml"
	}

	Connections {
		target: theClient
		onShowOrder: {
			popupLoader.setSource("OrderPopup.qml", {"order": order})
			popupLoader.item.open()
		}
		onOrderStatusChanged: {
			notificationLoader.setSource("OrderStatusPopup.qml", {"order": order})
			notificationLoader.item.open()
		}
	}
}

