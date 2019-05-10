import QtQuick 2.0
import QtQuick.Controls 2.2
import "../color.js" as Color
import "../Common.js" as Common

Item {
    implicitWidth: 200
    implicitHeight: 800

    property bool __scheduleBeRemoved: false

    function isValueBeEdited(){}

    function __ifForceSelectStage() {
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
            if( __ifForceSelectStage() )
                goEditAddedSchedule(index)
        }
        onScheduleRemoved: {
            __scheduleBeRemoved = true
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
            radius: 2
            color: ListView.isCurrentItem ? Color.sNavajoWhite() : Color.sFloralWhite()
            opacity: hovered ? 0.7 : 1

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
                    if( __ifForceSelectStage() )
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
                    console.log(focus,active,activeFocus)
                    if( !focus ) {
                        finishEditName()
                    }
                }
                onActiveFocusChanged: {
                    console.log("activeFocus:",activeFocus)
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
                width: parent.width
                height: 2
                anchors.bottom: parent.bottom
                color: ListView.isCurrentItem ? Color.sFloralWhite() : Color.sNavajoWhite()
                opacity: 0.2
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
