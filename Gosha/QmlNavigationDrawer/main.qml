import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1400
    height: 960
    title: qsTr("Hello World")
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
//        radius: 20

        Rectangle{
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            color: "#F5F5DC"

//            width: 100
//            color: "#4cd964"



            implicitWidth: 80
            implicitHeight: 80
//            color: button.down ? "#d6d6d6" : "#f6f6f6"
//            border.color: "#26282a"
//            border.width: 1
            radius: 10

            Label{
                anchors.fill: parent
//                color: "steelblue"

                Text{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Navigation"
//                    color: "white"
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
//            color: "#FFB6C1"
            color: "#F5F5DC"
            radius: 8

//            width: 100
//            color: "#4cd964"



//            implicitWidth: 100
//            implicitHeight: 30
//            color: button.down ? "#d6d6d6" : "#f6f6f6"
//            border.color: "#26282a"
//            border.width: 1
//            radius: 10

            Label{
                anchors.fill: parent


                Image {
                    id: cart_icon
                    anchors.fill: parent
                    source: "qrc:/cart.png"

                }


//                Text{
//                    anchors.horizontalCenter: parent.horizontalCenter
//                    anchors.verticalCenter: parent.verticalCenter
//                    text: "Cart"
//                    color: "green"
//                    font.pointSize: 10
//                    font.bold: true
//                }



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
//            anchors.bottomMargin: 5
            anchors.rightMargin: 120
            anchors.top: parent.top
//            anchors.bottom: parent.bottom
            anchors.right: parent.right
//            color: "#FFB6C1"
            color: "#F5F5DC"
            radius: 8

            Label{
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


        Rectangle{
            id: info
            width: 80
            height: 80
            anchors.topMargin: 10
//            anchors.bottomMargin: 5
            anchors.rightMargin: 220
            anchors.top: parent.top
//            anchors.bottom: parent.bottom
            anchors.right: parent.right
//            color: "#FFB6C1"
            color: "#F5F5DC"
            radius: 8

            Label{
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






















        // Иконка-Гамбургер
//        Rectangle {
//            anchors.top: parent.top
//            anchors.bottom: parent.bottom
//            anchors.left: parent.left

//            width: dp(48)
//            color: "#4cd964"

//            Rectangle {
//                anchors.top: parent.top
//                anchors.topMargin: dp(16)
//                anchors.left: parent.left
//                anchors.leftMargin: dp(14)
//                width: dp(20)
//                height: dp(2)
//            }

//            Rectangle {
//                anchors.top: parent.top
//                anchors.topMargin: dp(23)
//                anchors.left: parent.left
//                anchors.leftMargin: dp(14)
//                width: dp(20)
//                height: dp(2)
//            }

//            Rectangle {
//                anchors.top: parent.top
//                anchors.topMargin: dp(30)
//                anchors.left: parent.left
//                anchors.leftMargin: dp(14)
//                width: dp(20)
//                height: dp(2)
//            }

//            MouseArea {
//                anchors.fill: parent

//                onClicked: {
//                    lnav.toggle()
//                }
//            }
//        }



//        Rectangle {
//            anchors.top: parent.top
//            anchors.bottom: parent.bottom
//            anchors.right: parent.right

//            width: dp(48)
//            color: "#4cd964"

//            Rectangle {
//                anchors.top: parent.top
//                anchors.topMargin: dp(16)
//                anchors.right: parent.right
//                anchors.rightMargin: dp(14)
//                width: dp(20)
//                height: dp(2)
//            }

//            Rectangle {
//                anchors.top: parent.top
//                anchors.topMargin: dp(23)
//                anchors.right: parent.right
//                anchors.rightMargin: dp(14)
//                width: dp(20)
//                height: dp(2)
//            }

//            Rectangle {
//                anchors.top: parent.top
//                anchors.topMargin: dp(30)
//                anchors.right: parent.right
//                anchors.rightMargin: dp(14)
//                width: dp(20)
//                height: dp(2)
//            }

//            MouseArea {
//                anchors.fill: parent

//                onClicked: {
//                    rnav.toggle()
//                }
//            }
//        }





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

//    Component.onCompleted: {

//        var colores = ['orange', 'green', 'yellow', 'cyan', 'blue'];
//        var jx = 0;
//        var color_texto = 'black';
//        for(var ix = 0; ix < 10; ix++){
//            if(jx == colores.length){
//                jx = 0;
//            }
//            myListView.model.append({valorTexto:'hola ' + ix,
//                                        colorFondo: colores[jx],
//                                        colorTexto: color_texto
//                                    });
//            jx++;
//        }

//    }

//    ListView{
//        anchors.left: parent.left
//        anchors.right: parent.right
//        anchors.bottom: parent.bottom
//        anchors.topMargin: parent.height/5
//        anchors.top: parent.top
//        //anchors.fill: parent
//        id: myListView
//        model: ListModel{}
//        spacing: 5
//        delegate: Rectangle{
//            color: colorFondo
//            border.color: 'white'
//            border.width: 5
//            width: mainWindow.width
//            height: mainWindow.height/7
//            Text {
//                anchors{
//                    verticalCenter: parent.verticalCenter
//                    horizontalCenter: parent.horizontalCenter
//                }
//                color: colorTexto
//                font.pointSize: 20
//                text: valorTexto
//            }

//        }
//    }



    RightNavigationDrawer {
        id: rnav

        Rectangle{
//            anchors.topMargin: parent.height/10
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

