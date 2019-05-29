import QtQuick 2.0
import QtQuick.Controls 2.2
import "../js/color.js" as Color
import "../js/Common.js" as Common

Rectangle {
    implicitWidth: 200
    implicitHeight: 800
    color: Color.dark()

    property bool __scheduleBeRemoved: false

    function isValueBeEdited(){}

    function __isForceSelectStage() {
        if( isValueBeEdited() )
        {
            var flag = scheduleManager.messageBoxForQuestion(qsTr("当前修改未保存，是否确认跳转？"))
            if( !flag )
            {
                return true
            }

            return false
        }
        else
        {
            return true
        }
    }

    Button {
        id: addBtn
        width: parent.width
        height: 32
//        icon.source: "qrc:/res/add.png"
        text: "+"
        highlighted: true
        visible: view.count === 0
        z: 100
        onClicked: scheduleManager.addSchedule("",1)
    }

    ListView {
        id: view
        width: parent.width
        height: parent.height
        model: scheduleModel
        delegate: scheduleDelegate

        add: Transition {
            NumberAnimation { properties: "x,y"; duration: 200 }
        }
        move: Transition {
            SmoothedAnimation { properties: "x,y"; duration: 500 }
        }
        remove: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; to: 0; duration: 1000 }
                NumberAnimation { properties: "x"; to: 100; duration: 1000 }
            }
        }
        displaced: Transition {
            NumberAnimation { properties: "x,y"; duration: 500 }
        }
        clip: true
        ScrollBar.vertical: ScrollBar {}

        onCurrentIndexChanged: scheduleManager.selectSchedule(currentIndex)

        onCountChanged: {
            if( !__scheduleBeRemoved )
                return

            __scheduleBeRemoved = false
            scheduleManager.selectSchedule(currentIndex)
        }
    }

    Connections {
        target: scheduleManager
        onScheduleAdded: {
            if( __isForceSelectStage() )
                goEditAddedSchedule(index)
        }
        onScheduleRemoved: {
            __scheduleBeRemoved = true
        }
        onJumpToSchedule: {
            if( __isForceSelectStage() ) {
                view.currentIndex = index
                scheduleManager.selectSubScheduleWithId(subId, stageId)
            }
        }
    }

    function goEditAddedSchedule(index) {
        view.currentIndex = index
        view.currentItem.editNameAction()
    }

    Component {
        id: scheduleDelegate
        Rectangle {
            id: wrapper
            width: wrapper.ListView.view.width
            height: 90
//            radius: 3
            color: ListView.isCurrentItem ? Color.light() : "transparent" //Color.sFloralWhite()
//            opacity: hovered ? 0.7 : 1

            property bool hovered: false
            property bool editing: false

            function editNameAction() {
                if( wrapper.editing ) {
                    textInput.finishEditName()
                } else {
                    textInput.goEditName(name)
                }
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                preventStealing: true
                propagateComposedEvents: true
                onClicked: {
                    if( __isForceSelectStage() )
                    {
                        wrapper.ListView.view.currentIndex = index
                    }
                }
                onEntered: wrapper.hovered = true
                onExited: wrapper.hovered = false
            }

            Text {
                anchors.fill: parent
                anchors.topMargin: 5
                anchors.bottomMargin: 20
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                text: name
                visible: !wrapper.editing
                wrapMode: Text.WordWrap
//                color: "#303133"
//                color: wrapper.ListView.isCurrentItem ? "white" : Color.dark()
            }

            TextInput {
                id: textInput
                anchors.fill: parent
                anchors.topMargin: 5
                anchors.bottomMargin: 20
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                selectByMouse: true
                selectionColor: Color.selectionColor()
                font.bold: true
                font.italic: true
                visible: wrapper.editing
                wrapMode: Text.WordWrap
                onAccepted: {
                    finishEditName()
                }
                onFocusChanged: {
                    if( !focus ) {
                        finishEditName()
                    }
                }

                onActiveFocusChanged: {
                    if( !activeFocus ) {
                        finishEditName()
                    }
                }

                function goEditName(val) {
                    text = val
                    wrapper.editing = true
                    selectAll()
                    forceActiveFocus()
                }
                function finishEditName() {
                    if( Common.isDiffString(text,name) ) {
                        scheduleManager.editScheduleName(id, text)
                    }
                    wrapper.editing = false
                }
            }

            Rectangle {
                width: parent.width - 10
                height: 2
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                color: Color.light()
                opacity: 0.2
                visible: !wrapper.ListView.isCurrentItem
            }

            IconButton {
                id: addBtn
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                width: 32
                height: 32
                hoverIcon: "qrc:/res/addHover.png"
                pressIcon: "qrc:/res/addPress.png"
                normalIcon: "qrc:/res/add.png"
                visible: wrapper.hovered && wrapper.ListView.isCurrentItem

                onHoveredChanged: wrapper.hovered = hovered
                onClicked: scheduleManager.addSchedule("",1)
            }

            IconButton {
                id: editBtn
                anchors.left: addBtn.right
                anchors.bottom: parent.bottom
                width: 32
                height: 32
                hoverIcon: "qrc:/res/editHover.png"
                pressIcon: "qrc:/res/editPress.png"
                normalIcon: "qrc:/res/edit.png"
                visible: wrapper.hovered && wrapper.ListView.isCurrentItem

                onHoveredChanged: wrapper.hovered = hovered
                onClicked: wrapper.editNameAction()
            }

            IconButton {
                id: removeBtn
                anchors.left: editBtn.right
                anchors.bottom: parent.bottom
                width: 32
                height: 32
                hoverIcon: "qrc:/res/removeHover.png"
                pressIcon: "qrc:/res/removePress.png"
                normalIcon: "qrc:/res/remove.png"
                visible: wrapper.hovered && wrapper.ListView.isCurrentItem

                onHoveredChanged: wrapper.hovered = hovered
                onClicked: scheduleManager.removeSchedule(id)
            }
        }
    }
}
