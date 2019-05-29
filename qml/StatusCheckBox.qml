import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import "../js/color.js" as Color

CheckBox {
    id: checkbox
    property int status: 0
    Material.accent: "#30000000"
    background: Pane {
        padding: 5
        Material.elevation: 1
        implicitHeight: 20
        height: 30
        anchors.verticalCenter: checkbox.verticalCenter
        Material.background: Color.stageStatusColor(checkbox.status)
        implicitWidth: text.contentWidth
        MouseArea {
            anchors.fill: parent
            onClicked: {
                checkbox.checked = !checkbox.checked
            }
        }
    }

    Text {
        id: text
        text: checkbox.text
        font: checkbox.font
        visible: false
    }
}
