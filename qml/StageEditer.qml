import QtQuick 2.10
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import Fluid.Controls 1.0 as FluidControls
import "../js/size.js" as Size
import "../js/color.js" as Color
import "../js/Common.js" as Common

Item {
    implicitWidth: 300
    implicitHeight: 800

    property date date

    property string title

    property string details

    property string result

    property int __id

    function showStage(pdate,ptitle,pdetails,presult,pid) {
        date = pdate
        title = ptitle
        details = pdetails
        result = presult

        dateEdit.text = Common.dateFormat("yyyy-MM-dd hh:mm:ss",pdate)
        titleEdit.text = ptitle
        detailsEdit.text = pdetails
        resultEdit.text = presult

        __id = pid
    }

    function clearStage() {
        titleEdit.text = ""
        detailsEdit.text = ""
        resultEdit.text = ""

        title = ""
        details = ""
        result = ""

        __id = -1
    }

    function valueBeEdited() {
        if( Common.isDiffString(title, titleEdit.text)
                || Common.isDiffString(details, detailsEdit.text)
                || Common.isDiffString(result, resultEdit.text)
                || Common.isDiffString(Common.dateFormat("yyyy-MM-dd hh:mm:ss",date)
                                       ,dateEdit.text)) {
            return true
        }

        return false
    }

    FluidControls.DateTimePickerDialog {
        id: dateTimePickerDialog
        weekNumberVisible: false
        standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel
        onAccepted: {
            dateEdit.text = Common.dateFormat("yyyy-MM-dd hh:mm:ss",selectedDateTime)
        }

        function openWithDateTime(dateTime) {
            selectedDateTime = dateTime
            open()
        }
    }

    Rectangle {
        id: dateRect
        width: parent.width
        height: 40
        color: "#E0EEEE"
        Text {
            id: dateEdit
            text: Common.dateFormat("yyyy-MM-dd hh:mm:ss",date)
            anchors.centerIn: parent
            elide: Text.ElideRight
            visible: __id !== -1
        }

        Button {
            width: 32
            height: 32
            icon.source: "qrc:/res/pull.png"
            highlighted: true
            anchors.left: dateEdit.right
            anchors.leftMargin: 20
            anchors.verticalCenter: dateEdit.verticalCenter
            visible: __id !== -1
            onClicked: {
                dateTimePickerDialog.openWithDateTime(date)
            }
        }
    }

    Rectangle {
        id: titleRect
        width: parent.width
        anchors.top: dateRect.bottom
        height: 60
        TextField {
            id: titleEdit
            text: title
            width: parent.width - 20
            anchors.horizontalCenter: parent.horizontalCenter
            wrapMode: TextInput.Wrap
            selectByMouse: true
            font.bold: true
            placeholderText: qsTr("阶段标题...")
        }
    }

    Pane {
        id: detailsRect
        width: parent.width - 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: titleRect.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 150
        Material.elevation: 1
        Flickable {
            id: flick
            anchors.fill: parent
            anchors.margins: 10
            contentWidth: detailsEdit.paintedWidth
            contentHeight: detailsEdit.paintedHeight
            clip: true

            function ensureVisible(r)
            {
                if (contentX >= r.x)
                    contentX = r.x;
                else if (contentX+width <= r.x+r.width)
                    contentX = r.x+r.width-width;
                if (contentY >= r.y)
                    contentY = r.y;
                else if (contentY+height <= r.y+r.height)
                    contentY = r.y+r.height-height;
            }

            TextEdit {
                id: detailsEdit
                width: flick.width
                height: flick.height
                focus: true
                wrapMode: TextEdit.Wrap
                selectByMouse: true
                onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)                
                text: details
//                onTextChanged: details = text
            }
        }
    }

    Rectangle {
        id: resultRect
        width: parent.width
        anchors.top: detailsRect.bottom
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 90
        color: Color.stageResultColor()

        Image {
            id: resultImg
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 5
            width: 32
            height: 32
            source: "qrc:/res/result.png"
        }

        TextField {
            id: resultEdit
            text: result
            anchors.margins: 10
            anchors.left: resultImg.right
            anchors.right: parent.right
            placeholderText: qsTr("完成结果...")
            selectByMouse: true
        }
    }

    IconButton {
        width: 48
        height: 48
        anchors.left: parent.left
        anchors.leftMargin: parent.width/4 - width/2
        anchors.top: resultRect.bottom
        anchors.topMargin: 21
        hoverIcon: "qrc:/res/updateStageHover.png"
        pressIcon: "qrc:/res/updateStagePress.png"
        normalIcon: "qrc:/res/updateStage.png"
        onClicked: {
            if( scheduleManager.editStage(__id, new Date(dateEdit.text), titleEdit.text, detailsEdit.text, resultEdit.text) ) {
                showStage(new Date(dateEdit.text), titleEdit.text, detailsEdit.text, resultEdit.text)
            }

        }
    }

    Rectangle {
        width: 3
        height: 48
        anchors.top: resultRect.bottom
        anchors.topMargin: 21
        anchors.horizontalCenter: resultRect.horizontalCenter
        color: "gray"
    }

    IconButton {
        width: 48
        height: 48
        anchors.right: parent.right
        anchors.rightMargin: parent.width/4 - width/2
        anchors.top: resultRect.bottom
        anchors.topMargin: 21
        hoverIcon: "qrc:/res/removeStageHover.png"
        pressIcon: "qrc:/res/removeStagePress.png"
        normalIcon: "qrc:/res/removeStage.png"
        onClicked: scheduleManager.removeStage(__id)
    }
}
