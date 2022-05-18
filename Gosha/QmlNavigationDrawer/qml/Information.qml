import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1

Rectangle{
    anchors.leftMargin: parent.width/4
    anchors.rightMargin: parent.width/4
    anchors.topMargin: 100
    anchors.bottomMargin: 100
    anchors.fill: parent
    radius: 100
    color: "#F5F5DC"

    Label {
        anchors.topMargin: parent.height*2/10
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            id: login_label
            text: qsTr("Restaurant BES")
            color: "red"
            font.pixelSize: 60
            font.family: "Arial"
            font.italic: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true
        }
    }

    Label {

        anchors.topMargin: parent.height*60/100
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            text: qsTr("«…очевидно, у Вас есть невеста, которую Вам хочется поскорее сбыть
 с рук; но извините, жениться в настоящее время я не могу, потому что,
 во-первых, во мне сидят бациллы, жильцы весьма сумнительные;
 во-вторых, у меня ни гроша, и, в-третьих, мне все еще кажется, что я
 очень молод. Позвольте мне погулять еще годика два-три, а там
 уви­дим — быть может, и в самом деле женюсь. Только зачем Вы хотите,
 чтобы жена меня „растормошила“? Ведь и без того тормошит сама
 жизнь, тормошит шибко».
                                                             (Чехов, одно из писем Ф.Шехтелю)")
            font.pixelSize: 20
            font.family: "Arial"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.bold: true

        }
    }

}
