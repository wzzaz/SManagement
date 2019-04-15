import QtQuick 2.0

Item {
    implicitWidth: 900
    implicitHeight: 800

    Component {
        id: subScheduleDelegate
        Rectangle {
            id: wrapper
            width: 900
            height: 100
            border.color: "black"
            Rectangle {
                id: subScheduleTitle
                width: 160
                height: 80
                anchors.left: parent.left
                anchors.leftMargin: 3
                anchors.verticalCenter: parent.verticalCenter
                radius: 2
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        wrapper.ListView.view.currentIndex = index
                        scheduleManager.selectSubSchedule(index)
                    }
                }
                Text {
                    anchors.centerIn: parent
                    text: name
                }
            }

            Rectangle {
                id: stageRec
                anchors.left: subScheduleTitle.right
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 5
                border.color: "yellow"

                GridView {
                    width: stageRec.width - 8
                    height: stageRec.height - 8
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.margins: 4
                    model: subScheduleModel.selectStageModel(index)
                    delegate: stageDelegate
                }
            }
        }
    }

    Component {
        id: stageDelegate
        Item {
            id: stageWrapper
            width: 100
            height: 30
            Rectangle {
                anchors.fill: parent
                border.color: "blue"
                Text {
                    text: details
                }
            }
        }
    }

    ListView {
        width: parent.width
        height: parent.height
        model: subScheduleModel
        delegate: subScheduleDelegate
    }
}
