import QtQuick 2.9
import QtQml.Models 2.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import "../Common.js" as Common
import "../color.js" as Color

ApplicationWindow {
    id: rootWindow
    visible: true
    width: 1240
    height: 700
    title: qsTr("SManagement")
    visibility: Window.Maximized

    Material.accent: Material.color(Material.LightBlue,Material.Shade300)

//    menuBar: MenuBar {
//        Menu {
//            title: "File"
//            MenuItem { text: "Open..." }
//            MenuItem { text: "Close" }
//        }

//        Menu {
//            title: "Edit"
//            MenuItem { text: "Cut" }
//            MenuItem { text: "Copy" }
//            MenuItem { text: "Paste" }
//        }
//    }

    toolBar:ToolBar {
        height: 50

        ExclusiveGroup { id: toolBarGroup }
        HomeToolArea {
            id: homeToolBar
            anchors.fill: parent
            checked: true
            exclusiveGroup: toolBarGroup
        }
        DetailsToolArea {
            id: detailsToolBar
            anchors.fill: parent
            exclusiveGroup: toolBarGroup
        }
        Connections {
            target: rootTabView
            onCurrentIndexChanged: {
                if (rootTabView.currentIndex === 0)
                    homeToolBar.checked = true
                else
                   detailsToolBar.checked = true
            }
        }
    }

    statusBar: StatusBar {
        id: statisBar
        implicitWidth: parent.width
        property string status
        RowLayout {
            anchors.fill: parent
            IconButton {
                width: 32
                height: 32
                hoverIcon: "qrc:/res/syncHover.png"
                pressIcon: "qrc:/res/syncPress.png"
                normalIcon: "qrc:/res/sync.png"
                onClicked: {
                    if( rootTabView.currentIndex === 0 )
                        calendarListManager.updateCalendarQMLView()
                    else if( rootTabView.currentIndex === 1 )
                        scheduleManager.updateStageWorkStatus()
                }
            }
            Label { text: statisBar.status }
            Item { Layout.fillWidth: true }
        }
    }

//    Connections {
//        target: scheduleManager
//        onStageUpdateStatus: {
//            console.log("onStageUpdateStatus updating=",updating)
//            if( updating ) {
//                rootWindow.statusBar.visible = true
//                rootWindow.statusBar.status = "updating"
//            } else {
//                rootWindow.statusBar.status = "done"
//            }
//        }
//    }

    Component {
        id: tabViewStyle
        TabViewStyle {
            tab: Rectangle {
                implicitWidth: 80
                implicitHeight: 60
//                border.color: "steelblue"
                Image {
                    anchors.centerIn: parent
                    width: 48
                    height: 48
                    source: styleData.selected ? Common.resNormalPath( styleData.title ) :
                                                ( styleData.hovered ? Common.resHoverPath( styleData.title )
                                                                    : Common.resPressPath( styleData.title ) )
                }
            }
        }
    }

    TabView {
        id: rootTabView
        style: tabViewStyle
        anchors.fill: parent
        tabPosition: Qt.BottomEdge
        Tab {
            title: "home"

            Rectangle {
                anchors.fill: parent
                color: "transparent"
                SplitView {
                    width: parent.width
                    height: parent.height
                    CalendarEventList {
                        id: eventList
                        Layout.minimumWidth: 300
                        Layout.maximumWidth: 500
                        onJumpToScheduleDetail: {
                            rootTabView.currentIndex = 1
                            scheduleManager.selectScheduleWithId(schId, subId, stageId)
                        }
                    }
                    CalendarMonthView {
                        id: calendarMonthView
                        Layout.minimumWidth: 900
                        height: parent.height
                        Layout.fillWidth: true
                        onSelectedDateChanged: eventList.selectedDate = selectedDate
                    }
                }
            }
        }
        Tab {
            title: "details"
            Rectangle {
                width: 1000
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                SplitView {
                    width: parent.width
                    height: parent.height

                    ScheduleView {
                        id: schedleView
                        Layout.minimumWidth: 200
                        Layout.maximumWidth: 300

                        function isValueBeEdited() {
                            return stageEditor.valueBeEdited()
                        }
                    }

                    SubScheduleView {
                        Layout.minimumWidth: 900
                        height: parent.height
                        Layout.fillWidth: true
                        onSigSelectStage: {
                            stageEditor.showStage(date,title,details,result,id)
                        }
                        onClearStageEdit: {
                            stageEditor.clearStage()
                        }

                        function isValueBeEdited() {
                            return stageEditor.valueBeEdited()
                        }
                    }

                    StageEditer {
                        id: stageEditor
                        height: parent.height
                        Layout.minimumWidth: 300
                        Layout.maximumWidth: 500
                    }
                }

            }
        }
    }
}
