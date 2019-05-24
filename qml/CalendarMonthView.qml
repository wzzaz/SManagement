import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import Qt.labs.calendar 1.0
import "../color.js" as Color

Item {
    property date selectedDate: new Date

    property bool showDone: true

    property date __calendarViewDate: new Date

    function __previousMonth() {
        var year = __calendarViewDate.getFullYear()
        var month = __calendarViewDate.getMonth()

        __calendarViewDate = null
        if( month === 0 ) {
            __calendarViewDate = new Date(year-1, 11, 1)
        } else {
            __calendarViewDate = new Date(year,month-1,1)
        }
    }

    function __nextMonth() {
        var year = __calendarViewDate.getFullYear()
        var month = __calendarViewDate.getMonth()

        __calendarViewDate = null
        if( month === 11 ) {
            __calendarViewDate = new Date(year+1, 0, 1)
        } else {
            __calendarViewDate = new Date(year,month+1,1)
        }
    }

    function updateView() {
        var year = __calendarViewDate.getFullYear()
        var month = __calendarViewDate.getMonth()

        __calendarViewDate = null

        __calendarViewDate = new Date(year, month, 1)
    }

    Label {
        id: label
        text: Qt.locale().standaloneMonthName(__calendarViewDate.getMonth())
                                  + __calendarViewDate.toLocaleDateString(Qt.locale(), " yyyy")
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
        highlighted: true
        rotation: 180
        onClicked: __previousMonth()
    }

    Button {
        id: nextMonthBtn
        anchors.left: label.right
        anchors.leftMargin: 30
        anchors.verticalCenter: label.verticalCenter
        width: 70
        height: 50
        icon.source: "qrc:/res/next.png"
        highlighted: true
        onClicked: __nextMonth()
    }

    GridLayout {
        id: monthGridDelegate

        columns: 2
        rows: 2
        anchors.fill: parent
        anchors.topMargin: 100

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
            }

        }

        MonthGrid {
            id: grid
            month: __calendarViewDate.getMonth()
            year: __calendarViewDate.getFullYear()

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
                        visible: showDone && calendarListManager.isGivenDateExistGivenStatus(model.date, 1)
                    }
                    Image {
                        id: unDoneMarkImg
                        width: 32
                        height: 32
                        source: "qrc:/res/markUndone.png"
                        visible: calendarListManager.isGivenDateExistGivenStatus(model.date, 2)
                    }
                    Image {
                        id: nearPastMarkImg
                        width: 32
                        height: 32
                        source: "qrc:/res/markNearPast.png"
                        visible: calendarListManager.isGivenDateExistGivenStatus(model.date, 3)
                    }
                    Image {
                        id: waitingMarkImg
                        width: 32
                        height: 32
                        source: "qrc:/res/markWaiting.png"
                        visible: calendarListManager.isGivenDateExistGivenStatus(model.date, 4)
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

    Connections {
        target: calendarListManager
        onUpdate: updateView()
    }
}
