import QtQuick 2.9
import QtQuick.Window 2.2
import QtQml.Models 2.2

Window {
    visible: true
    width: 640
    height: 700
    title: qsTr("Hello World")

    ScheduleView {
        id: schedleView
    }

    SubScheduleView {
        anchors.left: schedleView.right
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
