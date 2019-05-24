import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import "../size.js" as Size
import "../color.js" as Color
import "../Common.js" as Common

Item {
    implicitWidth: 900
    implicitHeight: 800

    signal sigSelectStage(date date, string title, string details, string result, int id)
    signal clearStageEdit()

    property bool __stageBeRemoved: false

    property bool __subScheduleBeRemoved: false

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

    ListView {
        id: view
//        width: parent.width
//        height: parent.height
        anchors.fill: parent
        anchors.margins: 5
        model: subScheduleModel
        delegate: subScheduleDelegate
        visible: count > 0
        spacing: 8
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 500


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

        onCurrentIndexChanged: {
            clearStageEdit()
            if(currentItem)
                currentItem.selectCurStageAction()
        }
        onCountChanged: {
            if( !__subScheduleBeRemoved )
                return

            __subScheduleBeRemoved = false
            clearStageEdit()
            if(currentItem)
                currentItem.selectCurStageAction()
        }
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
            if( __isForceSelectStage() )
            {
                goEditAddedSubSchedule(index)
            }
        }
        onStageAdded: {
            if( __isForceSelectStage() )
            {
                goSelectAddedStage(index)
            }
        }
        onSubScheduleReset: {
            clearStageEdit()
            view.readySwapFromId = -1
            view.readyForSwap = false
        }
        onSubScheduleRemoved: {
            __subScheduleBeRemoved = true
        }
        onStageRemoved: {
            __stageBeRemoved = true
        }
        onJumpToSubSchedule: {
            if( __isForceSelectStage() ) {
                view.currentIndex = index
                scheduleManager.selectStageWithId(stageId)
            }
        }
        onJumpToStage: {
            if( __isForceSelectStage() )
            {
                if(view.currentItem)
                    view.currentItem.selectStageIndexAction(index)
            }
        }
    }

    function goEditAddedSubSchedule(index) {
        view.currentIndex = index
        view.currentItem.editNameAction()
    }

    function goSelectAddedStage(index) {
        view.currentItem.selectStageIndexAction(index)
    }

    Component {
        id: subScheduleDelegate
        Pane {
            id: wrapper
            width: parent.width
            height:  subScheduleTitle.height + stageRec.height + padding * 2 + 8
            Material.elevation: 6
            Material.background: Color.subScheduleBackground()
//            color: Material.color(Material.Red)
//            radius: 5
            property var __view: wrapper.ListView.view
//            property bool hovered: false
            property bool editing: false
            property bool readySwap: __view.readySwapFromId === id
            property bool curChecked: __view.currentIndex === index

            function editNameAction() {
                if( wrapper.editing ) {
                    titleField.finishEditName()
                } else {
                    titleField.goEditName(name)
                }
            }

            function selectCurStageAction() {
                selectCurrentSubSchedule()
                if(stageView.currentItem)
                    stageView.currentItem.selectStageAction()
            }

            function selectStageIndexAction(index) {
                stageView.__needEmitSig = true
                stageView.currentIndex = index
                selectCurStageAction()
            }


            function selectCurrentSubSchedule() {
                wrapper.__view.currentIndex = index
                scheduleManager.selectSubSchedule(index)
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    if( curChecked || __isForceSelectStage() )
                    {
                        wrapper.selectCurrentSubSchedule()
                    }
                }
//                onEntered: wrapper.hovered = true
//                onExited: wrapper.hovered = false
            }

            Rectangle {
                id: swapSelect
                width: 10
                height: wrapper.height - wrapper.padding
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.verticalCenter: parent.verticalCenter
                radius: 2
                color: wrapper.Material.accent //"#63B8FF"
                visible: readySwap
            }

            Rectangle {
                id: subScheduleTitle
                height: 35
                anchors.left: wrapper.__view.readyForSwap ? swapSelect.right : parent.left
                anchors.right: parent.right
//                anchors.leftMargin: 3
//                anchors.verticalCenter: wrapper.verticalCenter
//                radius: 2
                color: "transparent"// Color.primary() // Color.sNavajoWhite_Half()
//                opacity: wrapper.hovered ? 0.7 : 1
                enabled: !wrapper.__view.readyForSwap

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        if( curChecked || __isForceSelectStage() )
                        {
                            wrapper.selectCurrentSubSchedule()
                        }
                    }
//                    onEntered: wrapper.hovered = true
//                    onExited: wrapper.hovered = false
                }                

                Text {
                    id: titleFlag
                    width: 5
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.topMargin: 3
                    text: qsTr("•")
                }

                TextField {
                    id: titleField
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    text: name
                    wrapMode: Text.WordWrap
                    selectByMouse: wrapper.editing
                    readOnly: !wrapper.editing
                    font.italic: wrapper.editing
                    placeholderText: qsTr("请输入子计划标题...")

                    onAccepted: finishEditName()

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
                            scheduleManager.editSubScheduleName(id, text)
                        }
                        wrapper.editing = false
                    }
                }

                IconButton {
                    id: addBtn
                    anchors.right: editBtn.left
                    anchors.bottom: removeBtn.bottom
                    anchors.leftMargin: 32
                    width: 32
                    height: 32
                    hoverIcon: "qrc:/res/addHover.png"
                    pressIcon: "qrc:/res/addPress.png"
                    normalIcon: "qrc:/res/add.png"
                    visible: wrapper.hovered && !wrapper.__view.readyForSwap

//                    onHoveredChanged:  wrapper.hovered = hovered
                    onClicked: scheduleManager.addSubSchedule("",index + 1)
                }

                IconButton {
                    id: editBtn
                    anchors.right: removeBtn.left
                    anchors.bottom: removeBtn.bottom
                    width: 32
                    height: 32
                    hoverIcon: "qrc:/res/editHover.png"
                    pressIcon: "qrc:/res/editPress.png"
                    normalIcon: "qrc:/res/edit.png"
                    visible: wrapper.hovered && !wrapper.__view.readyForSwap

//                    onHoveredChanged: wrapper.hovered = hovered
                    onClicked: wrapper.editNameAction()
                }

                IconButton {
                    id: removeBtn
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 7
//                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 10
                    width: 32
                    height: 32
                    hoverIcon: "qrc:/res/removeHover.png"
                    pressIcon: "qrc:/res/removePress.png"
                    normalIcon: "qrc:/res/remove.png"
                    visible: wrapper.hovered && !wrapper.__view.readyForSwap

//                    onHoveredChanged: wrapper.hovered = hovered
                    onClicked: scheduleManager.removeSubSchedule(id,index)
                }
            }

            IconButton {
                id: swapBtn
                anchors.right: subScheduleTitle.right
                anchors.bottom: subScheduleTitle.bottom
                anchors.bottomMargin: 7
                anchors.rightMargin: 32*3 + 10
                width: 32
                height: 32
                hoverIcon: "qrc:/res/swapHover.png"
                pressIcon: "qrc:/res/swapPress.png"
                normalIcon: "qrc:/res/swap.png"
                visible: wrapper.hovered

//                onHoveredChanged: wrapper.hovered = hovered
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
                anchors.left: wrapper.__view.readyForSwap ? swapSelect.right : parent.left
                anchors.right: parent.right
                anchors.top: subScheduleTitle.bottom
                height: Math.max(stageView.height+8, 40)
//                anchors.bottom: parent.bottom
//                anchors.verticalCenter: subScheduleTitle.verticalCenter
//                height: Size.subScheduleHeight
                color: "transparent" //Color.sFloralWhite()
                enabled: !wrapper.__view.readyForSwap

//                Rectangle {
//                    anchors.left: parent.left
//                    height: parent.height
//                    width: 5
//                    color: Color.sNavajoWhite_Half()
//                    opacity: 0.5
//                }

                Button {
                    anchors.left: parent.left
//                    anchors.leftMargin: 6
                    width: 40
                    height: width
                    icon.source: "qrc:/res/add.png"
                    icon.width: 32
                    icon.height: 32
                    visible: wrapper.hovered

                    highlighted: true
//                    Material.accent: Material.Orange
                    onClicked: {
                        wrapper.selectCurrentSubSchedule()
                        scheduleManager.insertStage(new Date, "", "", "")
                    }
                }

                ListView {
                    id: stageView
//                    height: parseInt(count/parseInt(width/cellWidth) + 1) * cellHeight
                    height: {
                        if ( count <= 5 ) {
                            return (Size.stageHeight + spacing) * count
                        } else {
                            (Size.stageHeight + spacing) * 5
                        }
                    }

                    anchors.left: parent.left
                    anchors.leftMargin: 50
                    anchors.right: parent.right
                    anchors.top: parent.top
//                    anchors.bottom: parent.bottom
                    anchors.topMargin: 8
                    highlightFollowsCurrentItem: true
                    highlightMoveDuration: 500
//                    anchors.margins: 4
//                    cellWidth: Size.stageWidth + 6
//                    cellHeight: Size.stageHeight + 6
                    spacing: 3
                    clip: true
                    model: subScheduleModel.selectStageModel(index)
                    delegate: stageDelegate
                    currentIndex: -1
//                    visible: false
                    property int subScheduleIndex: index

                    property bool __needEmitSig: false

                    add: Transition {
                        NumberAnimation { properties: "x,y"; duration: 200 }
                    }
                    move: Transition {
                        SmoothedAnimation { properties: "x,y"; duration: 500 }
                    }
                    remove: Transition {
                        ParallelAnimation {
                            NumberAnimation { property: "opacity"; to: 0; duration: 1000 }
                            NumberAnimation { properties: "y"; to: 100; duration: 1000 }
                        }
                    }
                    displaced: Transition {
                        NumberAnimation { properties: "x,y"; duration: 500 }
                    }
                    ScrollBar.vertical: ScrollBar {}

                    onCurrentIndexChanged: {
                        if( !__needEmitSig )
                            return

                        clearStageEdit()
                        if(currentItem)
                            currentItem.selectStageAction()
                        __needEmitSig = false
                    }
                    onCountChanged: {
                        if(!__stageBeRemoved)
                            return

                        __stageBeRemoved = false
                        clearStageEdit()
                        if(currentItem)
                            currentItem.selectStageAction()
                    }
                }

                Component {
                    id: stageDelegate
                    Pane {
                        id: stageWrapper
                        width: parent.width
                        height: Size.stageHeight
                        padding: 5
//                        Material.elevation: 1
                        Material.background: stageChecked ? stageWrapper.Material.accent : Color.stageBackground()
                        property var __view: stageView
                        property var __parentSubView: wrapper.__view
                        property bool stageChecked: __view.subScheduleIndex === view.currentIndex && index === __view.currentIndex
                        property int __status: status

                        function selectStageAction() {
                            sigSelectStage(date,title,details,result,id)
                        }

                        MouseArea {
                            anchors.fill: parent
                            anchors.margins: -stageWrapper.padding
                            onClicked: {
                                if( stageChecked || __isForceSelectStage() )
                                {
                                    wrapper.selectCurrentSubSchedule()
                                    stageWrapper.__view.__needEmitSig = true
                                    stageWrapper.__view.currentIndex = index
                                    if( stageWrapper.__view.currentIndex === index )
                                        stageWrapper.__view.currentIndexChanged(index)
                                }
                            }
                        }

                        Rectangle {
                            id: mainRec
                            anchors.fill: parent
                            color: "transparent"

                            Image {
                                id: statusImg
                                anchors.top: parent.top
                                anchors.topMargin: 0
                                anchors.left: parent.left
                                width: 32
                                height: width
                                source: {
                                    switch(stageWrapper.__status) {
                                    case 1:
                                        return "qrc:/res/markDone.png"
                                    case 2:
                                        return "qrc:/res/markUndone.png"
                                    case 3:
                                        return "qrc:/res/markNearPast.png"
                                    case 4:
                                        return "qrc:/res/markWaiting.png"
                                    default:
                                        return "qrc:/res/markDone.png"
                                    }
                                }
                            }

                            Text {
                                id: titleText
                                text: title
                                anchors.left: statusImg.right
                                anchors.leftMargin: 5
//                                anchors.right: parent.right
//                                anchors.rightMargin: 5
                                width: Size.stageWidth
                                elide: Text.ElideRight
                                anchors.verticalCenter: parent.verticalCenter
                                color: stageChecked ? Color.stageBackground() : "000000"
                            }

                            Image {
                                id: resultImg
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: titleText.right
                                width: 32
                                height: 32
                                visible: !Common.isEmptyString(resultText.text)
                                source: stageChecked ? "qrc:/res/resultPress.png" : "qrc:/res/result.png"
                            }

                            Text {
                                id: resultText
                                text: result
                                anchors.left: resultImg.right
                                anchors.leftMargin: 2
                                anchors.verticalCenter: parent.verticalCenter
                                color: stageChecked ? Color.stageBackground() : "000000"
                            }

                            Text {
                                id: dateText
                                text: Common.dateFormat("yyyy-MM-dd hh:mm:ss",date)
                                anchors.verticalCenter: parent.verticalCenter
//                                anchors.left: parent.left
//                                anchors.leftMargin: 2
                                width: contentWidth
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                color: stageChecked ? Color.stageBackground() : "000000"
                            }
                        }
                    }
                }
            }
            Rectangle {
                anchors.left: subScheduleTitle.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                height: wrapper.height - wrapper.padding
                color: Color.enabledColor()
                visible: wrapper.__view.readyForSwap
                opacity: 0.3
            }
        }
    }
}
