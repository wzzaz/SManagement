import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 1.4

Item {
    id: homeToolArea

    property bool checked: false
    property ExclusiveGroup exclusiveGroup: null
    visible: checked

    onExclusiveGroupChanged: {
        if (exclusiveGroup)
            exclusiveGroup.bindCheckable(homeToolArea)
    }

    onCheckedChanged: {
        if (checked && exclusiveGroup)
            exclusiveGroup.bindCheckable(homeToolArea)
    }

    RowLayout {
        anchors.fill: parent

        Item { Layout.fillWidth: true }

        ExclusiveGroup { id: topPositionGroup }
        RadioButton {
            text: qsTr("Left")
            checked: true
            exclusiveGroup: topPositionGroup
        }
        RadioButton {
            text: "Right"
            exclusiveGroup: topPositionGroup
            Layout.alignment: Qt.AlignRight
        }
    }
}
