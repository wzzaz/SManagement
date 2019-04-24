import QtQuick 2.0
import QtQuick.Controls.Styles 1.4
import "color.js" as Color

MenuStyle {
    font: Qt.font({
                       family: "Source Code Pro",
                       pixelSize: 5
                   })
    frame: Rectangle {
        width: 150
        color: Color.sFloralWhite()
        border.color: "black"
        border.width: 1
    }
    itemDelegate.background: Rectangle {
        width: 150
        color: styleData.selected ? Color.sNavajoWhite() : "transparent"
    }
    itemDelegate.label: Text {
        text: styleData.text + "                "
        color: styleData.selected ? "white" : "black"
    }
}
