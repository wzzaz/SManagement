import QtQuick 2.9
import QtQml.Models 2.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "Common.js" as Common

ApplicationWindow {
    visible: true
    width: 1240
    height: 700
    title: qsTr("SManagement")
    visibility: Window.Maximized

    Component {
        id: tabViewStyle
        TabViewStyle {
            tab: Rectangle {
                implicitWidth: 80
                implicitHeight: 60
                border.color: "steelblue"
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
        style: tabViewStyle
        anchors.fill: parent
        Tab {
            title: "home"
            Rectangle {}
        }
        Tab {
            title: "details"
            Rectangle {
                width: 1000
                height: 700
                SplitView {
                    width: parent.width
                    height: parent.height

                    ScheduleView {
                        id: schedleView
                        Layout.minimumWidth: 200
                        Layout.maximumWidth: 300
                    }

                    SubScheduleView {
//                        anchors.left: schedleView.right
//                        anchors.leftMargin: 5
                        Layout.minimumWidth: 900
                        Layout.fillWidth: true
                    }
                }

            }
        }
    }

//    ListView {
//        anchors.left: schedleView.right
//        width: 180; height: 200

//        model: subScheduleModel
//        delegate: Item {
//            Text {
//                id: textComp
//             text: name + ": "
//             Component.onCompleted: {
//                 console.log(index)
//                 console.log(subScheduleModel.selectStageModel(index), index)
//             }
//            }
//            ListView {
//                anchors.left: textComp.right
//                width: 80; height: 30
//                model: subScheduleModel.selectStageModel(index)
//                orientation: ListView.Horizontal
//                delegate: Text {
//                    text: details
//                }
//            }
//        }
//    }
}
