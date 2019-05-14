import QtQuick 2.10
import QtQuick.Controls 2.5
import Fluid.Controls 1.0 as FluidControls
import "../size.js" as Size
import "../color.js" as Color
import "../Common.js" as Common

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

        IconButton {
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 10
            normalIcon: "qrc:/res/pull.png"
            hoverIcon: "qrc:/res/pull.png"
            pressIcon: "qrc:/res/pull.png"
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
            anchors.fill: parent
            anchors.margins: 10
            wrapMode: TextInput.Wrap
            selectByMouse: true
            font.bold: true
//            onAccepted: {
//                title = text
//            }
//            onEditingFinished: {
//                title = text
//            }
//            onTextChanged: title = text
        }
    }

    Rectangle {
        id: detailsRect
        width: parent.width
        anchors.top: titleRect.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 150
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
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 90
        color: Color.stageResultColor()
        TextField {
            id: resultEdit
            text: result
            anchors.fill: parent
            anchors.margins: 10
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
