import QtQuick 2.5
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.1
import Ver 1.0

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1400/2
    height: 960/2
    title: qsTr("Messenger")
    //color: "#E0FFFF"

    // Пересчёт независимых от плотности пикселей в физические пиксели устройства
    readonly property int dpi: Screen.pixelDensity * 25.4
    function dp(x) {
        return (dpi < 120) ? x : x*(dpi/160);
    }

    Client {
        id: client
        onNameChanged: {
            name.text = client.name
            var data = {'name': name.text}
            nameHistory.append(data)
        }
        onRegStatusChanged: {
            reg.text = client.regStatus
        }
        onIncremented: {
            randomButton.counter = randomButton.counter + 1
            randomButton.text = randomButton.counter
        }
    }

    ListModel {
        id: nameHistory
    }

    Rectangle {
        anchors {
            top: parent.top
            topMargin: 20
            right: parent.right
            rightMargin: 20
            bottomMargin: 20
        }
        height: parent.height*1/3
        width: parent.width*1/5
        color: "light gray"
        ListView {
            anchors.fill: parent
            height: parent.height*1/10
            interactive: true
            model:nameHistory
            delegate: Text {
                text: name
                anchors {
                    right: parent.right
                    topMargin: 20
                }
            }
        }
    }

// Reg label
    Text {
        id: textReg
        anchors {
            top: parent.top
            topMargin: 20
            left: parent.left
            leftMargin: 20
        }
        font.pixelSize: 24
        text: "Registered:"
    }
    Text {
        id: reg
        anchors {
            top: textReg.top
            topMargin: textReg.topMargin
            left: textReg.right
            leftMargin: 20
        }
        font.pixelSize: 24
        text: client.regStatus
    }

// Name label
    Text {
        id: textName
        anchors {
            top: reg.bottom
            topMargin: 20
            left: parent.left
            leftMargin: 20
        }
        font.pixelSize: 24
        text: "Name:"
    }
    Text {
        id: name
        anchors {
            top: textName.top
            topMargin: textName.topMargin
            left: textName.right
            leftMargin: 20
        }
        font.pixelSize: 24
        text: client.name
    }

//Input
    Text {
        id: nameInputHeader
        anchors {
            bottom: nameInput.top
            bottomMargin: 20
            horizontalCenter: parent.horizontalCenter}
        text: qsTr("Name")
    }

    TextField {
        id: nameInput
        placeholderText: qsTr("Enter your name")
        anchors {
            centerIn: parent
        }
        maximumLength: 20
        onAccepted: {
            client.name = nameInput.getText(0, nameInput.maximumLength)
            nameInput.remove(0, nameInput.maximumLength)
        }
    }

//Counter button
    Button {
        id: randomButton
        anchors {
            top: nameInput.bottom
            topMargin: 20
            horizontalCenter: parent.horizontalCenter
        }
        property int counter: 0
        text: counter
        onClicked: client.increment()
    }
}

