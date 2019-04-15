import QtQuick 2.0
import "Common.js" as Common

Item {
    implicitWidth: 200
    implicitHeight: 800

    Component {
        id: scheduleDelegate
        Rectangle {
            id: wrapper
            width: 200
            height: 90
            radius: 2
            color: ListView.isCurrentItem ? Common.sNavajoWhite() : Common.sFloralWhite()
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    wrapper.ListView.view.currentIndex = index
                    scheduleManager.selectSchedule(index)
                }
            }

            Text {
                anchors.centerIn: parent
                text: name
            }
        }
    }

    ListView {
        width: parent.width
        height: parent.height
        model: scheduleModel
        delegate: scheduleDelegate
    }
}
