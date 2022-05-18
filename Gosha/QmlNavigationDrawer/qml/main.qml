import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1400
    height: 960
    title: qsTr("Restaurant BES")
    color: "#E0FFFF"

    // Пересчёт независимых от плотности пикселей в физические пиксели устройства
    readonly property int dpi: Screen.pixelDensity * 25.4
    function dp(x){ return (dpi < 120) ? x : x*(dpi/160); }

    // Application Bar
    Rectangle {
        id: menuRect
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 100
        color: "#FFC0CB"
        Rectangle{
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            color: "#F5F5DC"
            implicitWidth: 80
            implicitHeight: 80
            radius: 10
            Label{
                anchors.fill: parent
                Text{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Navigation"
                    font.pointSize: 10
                    font.bold: true
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        lnav.toggle()
                    }
                }
            }
        }
        Rectangle{
            id: cart
            width: 80
            height: 80
            anchors.topMargin: 10
            anchors.rightMargin: 20
            anchors.top: parent.top
            anchors.right: parent.right
            color: "#F5F5DC"
            radius: 8
            Label{
                anchors.fill: parent
                Image {
                    id: cart_icon
                    anchors.fill: parent
                    source: "qrc:/cart.png"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        rnav.toggle()
                    }
                }
            }
        }
        Rectangle{
            id: profile
            width: 80
            height: 80
            anchors.topMargin: 10
            anchors.rightMargin: 120
            anchors.top: parent.top
            anchors.right: parent.right
            color: "#F5F5DC"
            radius: 8
            Label {
                anchors.fill: parent
                Image {
                    id: profile_icon
                    anchors.fill: parent
                    source: "qrc:/profile.png"

                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        loader.loadFragment(4)
                    }
                }
            }
        }
        Rectangle {
            id: info
            width: 80
            height: 80
            anchors.topMargin: 10
            anchors.rightMargin: 220
            anchors.top: parent.top
            anchors.right: parent.right
            color: "#F5F5DC"
            radius: 8
            Label {
                anchors.fill: parent
                Image {
                    id: info_icon
                    anchors.fill: parent
                    source: "qrc:/info.png"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        loader.loadFragment(5)
                    }
                }
            }
        }
    }

    // Loader для смены Фрагментов
    Loader {
        id: loader
        anchors.top: menuRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        source: "Fragment1.qml"

        // Функция для смены содержимого Loader
        function loadFragment(index){

            switch(index){
            case 0:
                loader.source = "Fragment1.qml"
                break;
            case 1:
                loader.source = "LoginForm.qml"
                break;
            case 2:
                loader.source = "Fragment3.qml"
                break;
            case 3:
                loader.source = "RegisterForm.qml"
                break;
            case 4:
                loader.source = "LoginForm.qml"
                break;
            case 5:
                loader.source = "Information.qml"
                break;
            case 6:
                loader.source = "Profile.qml"
                break;
            default:
                loader.source = "Fragment1.qml"
                break;
            }
        }
    }
    RightNavigationDrawer {
        id: rnav
        Rectangle{
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 200
            color: "#FFFF00"
            Label {
                anchors.fill: parent
                Text {
                    text: qsTr("Your Cart:")
                    font.pixelSize: 60
                    color: "red"
                    font.italic: true
                    font.family: "Arial"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    font.bold: true
                }
            }
        }
        Label {
            anchors.topMargin: parent.height/2
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                text: qsTr("Your cart is empty")
                font.pixelSize: 20
                color: "red"
                font.italic: true
                font.family: "Arial"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.bold: true

            }
        }

    }
    LeftNavigationDrawer {
        id: lnav
        Rectangle {
            anchors.fill: parent
            color: "#90EE90"

            // Список с пунктами меню
            ListView {
                anchors.topMargin: 30
                anchors.fill: parent
                spacing: 50
                delegate: Item {

                    height: 80
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: dp(5)
                        color: "pink"
                        radius: 25

                        Text {
                            text: fragment
                            anchors.fill: parent
                            font.pixelSize: dp(20)

                            renderType: Text.NativeRendering
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        MouseArea {
                            anchors.fill: parent

                            // По нажатию на пункт меню заменяем компонент в Loader
                            onClicked: {
                                loader.loadFragment(index)
                            }
                        }
                    }
                }

                model: navModel
            }
        }
    }

    // Модель данных для списка с пунктами меню
    ListModel {
        id: navModel

        ListElement {fragment: "Menu"}
        ListElement {fragment: "Login"}
        ListElement {fragment: "Fragment 3"}
    }
}

