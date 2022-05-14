import QtQuick 2.5
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.1
import Ver 1.0

Window {
    id: root
    visible: true
    width: 1024
    height: 680
    title: qsTr("Messenger")
    color: "#E0FFFF"

    // Пересчёт независимых от плотности пикселей в физические пиксели устройства
    readonly property int dpi: Screen.pixelDensity * 25.4
    function dp(x) {
        return (dpi < 120) ? x : x*(dpi/160);
    }

    Header {
        id: header
    }
}

