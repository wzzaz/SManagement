import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "../js/color.js" as Color

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
        spacing: 5

        Item { Layout.fillWidth: true }

        StatusCheckBox {
            text: qsTr("已完成")
            checked: true
            status: 1
            onCheckedChanged: commonManager.setStatusFilter(status,checked)
        }
        StatusCheckBox {
            text: qsTr("未完成")
            checked: true
            status: 2
            onCheckedChanged: commonManager.setStatusFilter(status,checked)
        }
        StatusCheckBox {
            text: qsTr("接近过期")
            checked: true
            status: 3
            onCheckedChanged: commonManager.setStatusFilter(status,checked)
        }
        StatusCheckBox {
            text: qsTr("等待完成")
            checked: true
            status: 4
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 5
            onCheckedChanged: commonManager.setStatusFilter(status,checked)
        }
    }
}
