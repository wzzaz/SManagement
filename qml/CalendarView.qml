import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import Qt.labs.calendar 1.0
import "../color.js" as Color

Item {
    property date selectedDate: new Date
    //    color: "white"
    property bool showDone: false

    Label {
        id: label
        text: Qt.locale().standaloneMonthName(listView.currentDate.getMonth())
                                  + listView.currentDate.toLocaleDateString(Qt.locale(), " yyyy")
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 35
    }

    Button {
        id: previousMonthbtn
        anchors.right: label.left
        anchors.rightMargin: 30
        anchors.verticalCenter: label.verticalCenter
        width: 70
        height: 50
        icon.source: "qrc:/res/next.png"
        rotation: 180
        onClicked: listView.currentIndex--
    }

    Button {
        id: nextMonthBtn
        anchors.left: label.right
        anchors.leftMargin: 30
        anchors.verticalCenter: label.verticalCenter
        width: 70
        height: 50
        icon.source: "qrc:/res/next.png"
        onClicked: listView.currentIndex++
    }

    ListView {
        id: listView

        clip: true
        anchors.fill: parent
        anchors.topMargin: 100
        snapMode: ListView.SnapOneItem
        orientation: ListView.Horizontal
        highlightRangeMode: ListView.StrictlyEnforceRange
        highlightMoveDuration: 0

        model: CalendarModel {
            id: calendarModel
            from: new Date(2010, 0, 1)
            to: new Date(2020, 11, 31)
        }

        delegate: calendarDelegate

        property date currentDate: new Date

        onCurrentIndexChanged: {
            currentDate = null
            currentDate = new Date(listView.model.yearAt(currentIndex), listView.model.monthAt(currentIndex), 1)
        }

        Component.onCompleted: currentIndex = listView.model.indexOf(selectedDate)
    }


    Component {
        id: calendarDelegate
        GridLayout {
            id: monthGridDelegate

            columns: 2
            rows: 2
            width: listView.width
            height: listView.height

            DayOfWeekRow {
                visible: true
                locale: grid.locale

                Layout.column: 2
                Layout.fillWidth: true
                Layout.topMargin: 10

                delegate: Label {
                    text: model.shortName
                    font.weight: Font.DemiBold
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    //                            color: control.Material.secondaryTextColor
                }
            }

            WeekNumberColumn {
                //                        visible: weekNumberVisible
                month: grid.month
                year: grid.year
                locale: grid.locale

                Layout.fillHeight: true
                Layout.column: 1
                Layout.row: 1
                Layout.rowSpan: 2
                Layout.bottomMargin: 10
                Layout.topMargin: 10

                delegate: Label {
                    text: model.weekNumber
                    font.weight: Font.DemiBold
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    //                            color: control.Material.secondaryTextColor
                }

            }

            MonthGrid {
                id: grid
                month: model.month
                year: model.year
                //                        locale: control.locale

                Layout.column: 2
                Layout.row: 1
                Layout.rowSpan: 2
                Layout.bottomMargin: 10
                Layout.topMargin: 10
                Layout.fillWidth: true
                Layout.fillHeight: true

                delegate: Item {
                    id: dayDelegate

                    Rectangle {
                        anchors.centerIn: parent
                        anchors.fill: parent
                        //                                y: -dayLabel.height / 8
                        color: isEqual(selectedDate) ? Color.stageSelectedColor() : "#F5F5F5"
                        visible: model.month === grid.month ? 1 : 0
                        radius: 5
                    }

                    Label {
                        id: dayLabel
                        text: model.day
                        font.weight: Font.DemiBold
                        font.pixelSize: 13
                        font.underline: model.today
                        width: parent.width
                        height: parent.height
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: isEqual(selectedDate) ? "white" : (model.today ? "orange" : "black")
                        opacity: model.month === grid.month ? 1 : 0
                    }

                    RowLayout {
                        visible: model.month === grid.month ? 1 : 0
                        Image {
                            id: doneMarkImg
                            width: 32
                            height: 32
                            source: "qrc:/res/markDone.png"
//                            visible: showDone && calendarListManager.isGivenDateExistGivenStatus(model.date, 1)
                        }
                        Image {
                            id: unDoneMarkImg
                            width: 32
                            height: 32
                            source: "qrc:/res/markUndone.png"
//                            visible: calendarListManager.isGivenDateExistGivenStatus(model.date, 2)
                        }
                        Image {
                            id: nearPastMarkImg
                            width: 32
                            height: 32
                            source: "qrc:/res/markNearPast.png"
//                            visible: calendarListManager.isGivenDateExistGivenStatus(model.date, 3)
                        }
                        Image {
                            id: waitingMarkImg
                            width: 32
                            height: 32
                            source: "qrc:/res/markWaiting.png"
//                            visible: calendarListManager.isGivenDateExistGivenStatus(model.date, 4)
                        }
                        Component.onCompleted: {
                            if( model.month !== grid.month )
                                return

                            doneMarkImg.visible = showDone && calendarListManager.isGivenDateExistGivenStatus(model.date, 1)
                            unDoneMarkImg.visible = calendarListManager.isGivenDateExistGivenStatus(model.date, 2)
                            nearPastMarkImg.visible = calendarListManager.isGivenDateExistGivenStatus(model.date, 3)
                            waitingMarkImg.visible = calendarListManager.isGivenDateExistGivenStatus(model.date, 4)
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        enabled: model.month === grid.month
                        onClicked: selectedDate = model.date
                    }

                    function isEqual(date) {
                        return model.day === date.getDate() &&
                                model.month === date.getMonth() &&
                                model.year === date.getFullYear();
                    }
                }
            }
        }
    }
}
