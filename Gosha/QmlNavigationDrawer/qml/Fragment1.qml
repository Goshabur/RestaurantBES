import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1


Rectangle {
    color: "#E0FFFF"
    Component.onCompleted: {
        var colores = ['orange', 'green', 'yellow', 'cyan', 'blue'];
        var jx = 0;
        var color_texto = 'black';
        for(var ix = 0; ix < 10; ix++){
            if(jx == colores.length){
                jx = 0;
            }
            myListView.model.append({valorTexto:'hola ' + ix,
                                        colorFondo: colores[jx],
                                        colorTexto: color_texto
                                    });
            jx++;
        }
    }
    Label {
        anchors.topMargin: parent.height/10
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            text: qsTr("Menu")
            font.pixelSize: 60
            color: "red"
            font.italic: true
            font.family: "Arial"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
        }
    }
    Rectangle{
        anchors.leftMargin: parent.width/6
        anchors.left: parent.left
        anchors.rightMargin: parent.width/6
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.topMargin: parent.height/5
        anchors.top: parent.top
        color: "#E0FFFF"
        ListView{
            anchors.fill: parent
            anchors{
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }
            id: myListView
            model: ListModel{}
            spacing: 10
            delegate: Rectangle{
                width: parent.width
                height: parent.width*6/10
                color: "#E0FFFF"
                Rectangle{
                    anchors.left: parent.left
                    height: parent.height
                    width: (parent.width-10)/2
                    color: "#F5F5DC"
                    radius: 30
                    Image {
                        id: pasta
                        width: parent.width*8/10
                        height: parent.width*8/10
                        anchors.topMargin: parent.width/10
                        anchors.top: parent.top
                        anchors.leftMargin: parent.width/10
                        anchors.left: parent.left
                        source: "qrc:/pasta.jpg"
                    }
                    Label {
                        text: "Pasta"
                        font.pixelSize: 25
                        font.italic: true
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.topMargin: parent.width*93/100
                        anchors.top: parent.top
                    }
                    Label {
                        text: "Price: 100$"
                        font.pixelSize: 25
                        font.italic: true
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.topMargin: parent.width
                        anchors.top: parent.top
                    }
                    Button {
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Info")
                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 30
                            color: button.down ? "#d6d6d6" : "#f6f6f6"
                            border.color: "#26282a"
                            border.width: 1
                            radius: 10
                        }
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Нет, и не под чуждым небосводом,
И не под защитой чуждых крыл, —
Я была тогда с моим народом,
Там, где мой народ, к несчастью, был.")
                    }
                }
                Rectangle{
                    anchors.right: parent.right
                    height: parent.height
                    width: (parent.width-10)/2
                    color: "#F5F5DC"
                    radius: 30
                    Image {
                        id: steak
                        width: parent.width*8/10
                        height: parent.width*8/10
                        anchors.topMargin: parent.width/10
                        anchors.top: parent.top
                        anchors.leftMargin: parent.width/10
                        anchors.left: parent.left
                        source: "qrc:/steak.jpg"
                    }
                    Label {
                        text: "Steak"
                        font.pixelSize: 25
                        font.italic: true
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.topMargin: parent.width*93/100
                        anchors.top: parent.top
                    }
                    Label {
                        text: "Price: 100$"
                        font.pixelSize: 25
                        font.italic: true
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.topMargin: parent.width
                        anchors.top: parent.top
                    }
                    Button {
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Info")
                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 30
                            color: button.down ? "#d6d6d6" : "#f6f6f6"
                            border.color: "#26282a"
                            border.width: 1
                            radius: 10
                        }
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Звенела музыка в саду
Таким невыразимым горем.
Свежо и остро пахли морем
На блюде устрицы во льду.

Он мне сказал: «Я верный друг!»
И моего коснулся платья.
Так не похожи на объятья
Прикосновенья этих рук.

Так гладят кошек или птиц,
Так на наездниц смотрят стройных…
Лишь смех в глазах его спокойных
Под легким золотом ресниц.

А скорбных скрипок голоса
Поют за стелющимся дымом:
«Благослови же небеса —
Ты в первый раз одна с любимым».")
                    }
                }
            }
        }
    }
}