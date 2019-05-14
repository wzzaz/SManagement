import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "../color.js" as Color

Item {
    id: detailsToolArea

    property bool checked: false
    property ExclusiveGroup exclusiveGroup: null
    visible: checked

    onExclusiveGroupChanged: {
        if (exclusiveGroup)
            exclusiveGroup.bindCheckable(detailsToolArea)
    }

    onCheckedChanged: {
        if (checked && exclusiveGroup)
            exclusiveGroup.bindCheckable(detailsToolArea)
    }

    RowLayout {
        anchors.fill: parent

        Item { Layout.fillWidth: true }

        StatusCheckBox {
            text: qsTr("已完成")
            checked: true
            status: 1
            style: detailsCheckBoxStyle
            onCheckedChanged: scheduleManager.setStatusFilter(status,checked)
        }
        MenuSeparator{}
        StatusCheckBox {
            text: qsTr("未完成")
            checked: true
            status: 2
            style: detailsCheckBoxStyle
            onCheckedChanged: scheduleManager.setStatusFilter(status,checked)
        }
        StatusCheckBox {
            text: qsTr("接近过期")
            checked: true
            status: 3
            style: detailsCheckBoxStyle
            onCheckedChanged: scheduleManager.setStatusFilter(status,checked)
        }
        StatusCheckBox {
            text: qsTr("等待完成")
            checked: true
            status: 4
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 5
            style: detailsCheckBoxStyle
            onCheckedChanged: scheduleManager.setStatusFilter(status,checked)
        }
    }

    Component {
        id: detailsCheckBoxStyle
        CheckBoxStyle {
            id: style
            label: Rectangle {
                implicitHeight: 20
                implicitWidth: text.contentWidth + 25
                color: Color.stageStatusColor(control.status)
                radius: 3
                Text {
                    id: text
                    text: control.text
                    anchors.centerIn: parent
                }

//                Rectangle {
//                    anchors.left: parent.left
//                    anchors.leftMargin: 15
//                    anchors.right: parent.right
//                    height: parent.height
//                    color: "transparent"

//                    Text {
//                        id: text
//                        text: control.text
//                    }
//                }
            }
        }
    }
}
