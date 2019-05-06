import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Button {
    id: iconButton
    property string hoverIcon
    property string pressIcon
    property string normalIcon

    Component {
        id: iconButtonStyle
        ButtonStyle {
            background: Image {
                source: control.pressed ? iconButton.pressIcon :
                                          ( control.hovered ? iconButton.hoverIcon : iconButton.normalIcon )
            }
        }
    }

    style: iconButtonStyle
}
