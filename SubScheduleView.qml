import QtQuick 2.0
import QtQuick.Controls 2.2
import "Common.js" as Common
import "size.js" as Size
import "color.js" as Color

Item {
    implicitWidth: 900
    implicitHeight: 800

    ListView {
        id: view
        width: parent.width
        height: parent.height
        model: subScheduleModel
        delegate: subScheduleDelegate
        visible: count > 0

        property int readySwapFromId: -1

        property bool readyForSwap: false

        property int __fromSwapIndex

        property int __toSwapIndex

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
    }


    IconButton {
        id: firstAddBtn
        width: 48
        height: 48
        anchors.left: parent.left
        anchors.leftMargin: width /2
        anchors.top: parent.top
        anchors.topMargin: height /2
        hoverIcon: "qrc:/res/addSubHover.png"
        pressIcon: "qrc:/res/addSubPress.png"
        normalIcon: "qrc:/res/addSub.png"
        visible: !view.visible

        onClicked: scheduleManager.addSubSchedule("",0)
    }

    Connections {
        target: scheduleManager
        onSubScheduleAdded: {
            goEditAddedSubSchedule(index)
        }
        onSubScheduleReset: {
            view.readySwapFromId = -1
        }
    }

    function goEditAddedSubSchedule(index) {
        view.currentIndex = index
        view.currentItem.editNameAction()
    }

    Component {
        id: subScheduleDelegate
        Rectangle {
            id: wrapper
            width: 1020
            height: 190
            property var __view: wrapper.ListView.view
            property bool hovered: false
            property bool editing: false
            property bool readySwap: __view.readySwapFromId === id

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
//                onClicked: {
//                    wrapper.__view.currentIndex = index
//                    scheduleManager.selectSubSchedule(index)
//                }
                onEntered: wrapper.hovered = true
                onExited: wrapper.hovered = false
            }

            Rectangle {
                id: swapSelect
                width: 10
                height: Size.subScheduleHeight
                anchors.left: wrapper.left
                anchors.leftMargin: 3
                anchors.verticalCenter: wrapper.verticalCenter
                radius: 2
                color: "#63B8FF"
                visible: readySwap
            }

            Rectangle {
                id: subScheduleTitle
                width: Size.subScheduleHeight
                height: Size.subScheduleHeight
                anchors.left: wrapper.__view.readyForSwap ? swapSelect.right : wrapper.left
                anchors.leftMargin: 3
                anchors.verticalCenter: wrapper.verticalCenter
                radius: 2
                color: Color.sNavajoWhite_Half()
                opacity: wrapper.hovered ? 0.7 : 1
                enabled: !wrapper.__view.readyForSwap

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        wrapper.__view.currentIndex = index
                        scheduleManager.selectSubSchedule(index)
                    }
                    onEntered: wrapper.hovered = true
                    onExited: wrapper.hovered = false
                }
                Text {
                    anchors.centerIn: parent
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 5
                    anchors.rightMargin: 5
                    text: name
                    visible: !wrapper.editing
                    wrapMode: Text.WordWrap
                }

                TextInput {
                    id: textInput
                    anchors.centerIn: parent
                    anchors.left: parent.left
                    anchors.right: parent.right
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
                            scheduleManager.editSubScheduleName(id, text)
                        }
                        wrapper.editing = false
                    }
                }

                IconButton {
                    id: removeBtn
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 32
                    width: 32
                    height: 32
                    hoverIcon: "qrc:/res/removeHover.png"
                    pressIcon: "qrc:/res/removePress.png"
                    normalIcon: "qrc:/res/remove.png"
                    visible: wrapper.hovered && !wrapper.__view.readyForSwap

                    onHoveredChanged: wrapper.hovered = hovered
                    onClicked: scheduleManager.removeSubSchedule(id,index)
                }

                IconButton {
                    id: editBtn
                    anchors.right: removeBtn.left
                    anchors.bottom: parent.bottom
                    width: 32
                    height: 32
                    hoverIcon: "qrc:/res/editHover.png"
                    pressIcon: "qrc:/res/editPress.png"
                    normalIcon: "qrc:/res/edit.png"
                    visible: wrapper.hovered && !wrapper.__view.readyForSwap

                    onHoveredChanged: wrapper.hovered = hovered
                    onClicked: wrapper.editNameAction()
                }


                IconButton {
                    id: addBtn
                    anchors.right: editBtn.left
                    anchors.bottom: parent.bottom
                    width: 32
                    height: 32
                    hoverIcon: "qrc:/res/addHover.png"
                    pressIcon: "qrc:/res/addPress.png"
                    normalIcon: "qrc:/res/add.png"
                    visible: wrapper.hovered && !wrapper.__view.readyForSwap

                    onHoveredChanged:  wrapper.hovered = hovered
                    onClicked: scheduleManager.addSubSchedule("",index + 1)
                }
            }

            IconButton {
                id: swapBtn
                anchors.right: subScheduleTitle.right
                anchors.bottom: subScheduleTitle.bottom
                width: 32
                height: 32
                hoverIcon: "qrc:/res/swapHover.png"
                pressIcon: "qrc:/res/swapPress.png"
                normalIcon: "qrc:/res/swap.png"
                visible: wrapper.hovered

                onHoveredChanged: wrapper.hovered = hovered
                onClicked: {
                    if( wrapper.__view.readySwapFromId === -1 ) {
                        wrapper.__view.__fromSwapIndex = index
                        wrapper.__view.readySwapFromId = id
                        wrapper.__view.readyForSwap = true
                    } else {
                        wrapper.__view.__toSwapIndex = index
                        if( wrapper.__view.readySwapFromId !== id ) {
                            scheduleManager.moveSubSchedule(wrapper.__view.readySwapFromId,id)
                        }
                        wrapper.__view.readySwapFromId = -1
                        wrapper.__view.readyForSwap = false
                    }
                }
            }

            Rectangle {
                id: stageRec
                anchors.left: subScheduleTitle.right
                anchors.right: wrapper.right
                anchors.verticalCenter: subScheduleTitle.verticalCenter
                height: Size.subScheduleHeight
                color: Color.sFloralWhite()
                enabled: !wrapper.__view.readyForSwap

                Rectangle {
                    anchors.left: parent.left
                    height: parent.height
                    width: 5
                    color: Color.sNavajoWhite_Half()
                    opacity: 0.5
                }

                GridView {
                    id: stageView
                    width: stageRec.width
                    height: stageRec.height
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.top: parent.top
                    anchors.margins: 4
                    cellWidth: Size.stageWidth + 3
                    cellHeight: Size.stageHeight + 3
                    clip: true
                    model: subScheduleModel.selectStageModel(index)
                    delegate: stageDelegate
                    property int subScheduleIndex: index
                }

                Component {
                    id: stageDelegate
                    Rectangle {
                        id: stageWrapper
                        width: Size.stageWidth
                        height: Size.stageHeight
                        property var __view: stageView
                        property var __parentSubView: wrapper.__view
                        property bool stageChecked: __view.subScheduleIndex === view.currentIndex && index === __view.currentIndex

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                stageWrapper.__view.currentIndex = index
                                stageWrapper.__parentSubView.currentIndex = __view.subScheduleIndex
                            }
                        }

                        Rectangle {
                            id: stageRec
                            anchors.fill: parent
                            Rectangle {
                                id: dateRec
                                width: stageRec.width
                                height: 20
                                color: "#E0EEEE"
                                Text {
                                    text: Common.dateFormat("yyyy-MM-dd hh:mm",date)
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    anchors.leftMargin: 2
                                }
                                Rectangle {
                                    id: stateRec
                                    width: 15
                                    height: parent.height
                                    color: Common.overDateForNow(date) ? (Common.isEmptyString(result) ? Color.unDoneStateColor() : Color.doneStateColor())
                                                                       : Color.waitingStateColor()
                                    anchors.right: parent.right
                                }
                            }
                            Rectangle {
                                id: titleRec
                                width: stageRec.width
                                anchors.left: parent.left
                                anchors.top: dateRec.bottom
                                anchors.right: parent.right
                                height: 30
                                Text {
                                    text: title
                                    anchors.left: parent.left
                                    anchors.leftMargin: 5
                                    anchors.right: parent.right
                                    anchors.rightMargin: 5
                                    elide: Text.ElideRight
                                }
                            }
                            Rectangle {
                                id: resultRec
                                width: stageRec.width
                                anchors.left: parent.left
                                anchors.top: titleRec.bottom
                                anchors.right: parent.right
                                height: 30
                                color: Color.stageResultColor()
                                visible: !Common.isEmptyString(resultText.text)
                                Text {
                                    id: resultText
                                    text: result
                                    anchors.left: parent.left
                                    anchors.leftMargin: 2
                                }
                            }
                        }

                        Rectangle {
                            anchors.fill: parent
                            color: Color.stageSelectedColor()
                            opacity: 0.1
                            visible: stageWrapper.stageChecked
                        }
                    }
                }
            }
            Rectangle {
                anchors.left: subScheduleTitle.left
                anchors.right: wrapper.right
                anchors.verticalCenter: wrapper.verticalCenter
                height: Size.subScheduleHeight
                color: Color.enabledColor()
                visible: wrapper.__view.readyForSwap
                opacity: 0.3
            }
        }
    }
}
