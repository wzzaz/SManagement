import QtQuick 2.0
import QtQuick.Controls 1.4

Rectangle {
    property date selectedDate

    signal jumpToScheduleDetail(int schId, int subId, int stageId)

    Component {
        id: eventListHeader

        Row {
            id: eventDateRow
            width: parent.width
            height: eventDayLabel.height
            spacing: 10

            Label {
                id: eventDayLabel
                text: selectedDate.getDate()
                font.pointSize: 35
            }

            Column {
                height: eventDayLabel.height

                Label {
                    readonly property var options: { weekday: "long" }
                    text: Qt.locale().standaloneDayName(selectedDate.getDay(), Locale.LongFormat)
                    font.pointSize: 18
                }
                Label {
                    text: Qt.locale().standaloneMonthName(selectedDate.getMonth())
                          + selectedDate.toLocaleDateString(Qt.locale(), " yyyy")
                    font.pointSize: 12
                }
            }
        }
    }

    ListView {
        id: eventsListView
        spacing: 4
        clip: true
        header: eventListHeader
        anchors.fill: parent
        anchors.margins: 10
        model: calendarListManager.schedulesForDate(selectedDate)

        delegate: Rectangle {
            width: eventsListView.width
            height: eventItemColumn.height
            anchors.horizontalCenter: parent.horizontalCenter

            Image {
                anchors.top: parent.top
                anchors.topMargin: 0
                width: 32
                height: width
                source: {
                    switch(modelData.status) {
                    case 1:
                        return "qrc:/res/markDone.png"
                    case 2:
                        return "qrc:/res/markUndone.png"
                    case 3:
                        return "qrc:/res/markNearPast.png"
                    case 4:
                        return "qrc:/res/markWaiting.png"
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: "#eee"
            }

            Column {
                id: eventItemColumn
                anchors.left: parent.left
                anchors.leftMargin: 35
                anchors.right: parent.right
                spacing: 3
                height: subScheduleLabel.height + scheduleLabel.height + titleLabel.height + 8

                Label {
                    id: scheduleLabel
                    width: parent.width
                    wrapMode: Text.Wrap
                    font.bold: true
                    text: modelData.scheduleName
                }
                Label {
                    id: subScheduleLabel
                    width: parent.width
                    wrapMode: Text.Wrap
                    text: modelData.subScheduleName
                }
                Label {
                    id: titleLabel
                    width: parent.width
                    wrapMode: Text.Wrap
                    font.underline: true
                    text: modelData.stageTitle
                    color: "#aaa"
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            jumpToScheduleDetail(modelData.schId, modelData.subId, modelData.stageId)
                        }
                    }
                }
            }
        }
    }
}
