import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform as Platform
import DeviceModel 1.0
import QtQuick.Dialogs

Page {
    anchors.fill: parent
    Column {
        anchors.fill: parent; anchors.bottomMargin: 20
        RowLayout {
            id: select_ROWLAYOUT_prefix
            width: parent.width; height: 60
            Text {
                Layout.preferredWidth: 120; Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                Layout.leftMargin: 10
                text: qsTr("请输入目标前缀:")
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 16
            }
            Rectangle {
                id: select_RECTANGLE_prefix
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                Layout.fillWidth: true; Layout.preferredHeight: 30
                color: "white"
                border.color: "black"
                TextInput {
                    id: select_TEXTINPUT_prefix
                    anchors.fill: parent; anchors.leftMargin: 5
                    verticalAlignment: Text.AlignVCenter
//                    onEditingFinished: myserial.onSelectPrefix(text)
                }
                /*
                states: [
                    State {
                        name: "VALID"
                        PropertyChanges {
                            target: select_RECTANGLE_prefix
                            border.color: "black"
                        }
                    },
                    State {
                        name: "INVALID"
                        PropertyChanges {
                            target: select_RECTANGLE_prefix
                            border.color: "red"
                        }
                    }
                ]
                */
            }
            Button {
                id: select_BUTTON_prefix
                Layout.preferredWidth: 80; Layout.preferredHeight: 50
                Layout.rightMargin: 5
                text: qsTr("读取")
                enabled: select_BUTTONGROUP.checkState === Qt.Unchecked
//                enabled: select_TEXTINPUT_prefix.text !== qsTr("") && select_RECTANGLE_prefix.state === "VALID"
                onClicked: myserial.onSelectPrefix(select_TEXTINPUT_prefix.text)
            }
        }
        ScrollView {
            id: select_SCROLLVIEW
            width: parent.width; height: parent.height - select_ROWLAYOUT_prefix.height - select_BUTTON_write.height
//            orientation: Qt.Vertical
//            ScrollBar.width: 5
//            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            contentWidth: availableWidth
            ScrollBar.vertical: ScrollBar {
                parent: select_SCROLLVIEW
                width: 10; height: select_SCROLLVIEW.availableHeight
                hoverEnabled: true
                x: select_SCROLLVIEW.width - width; y: select_ITEM_header.height
                active: hovered || pressed
                orientation: Qt.Vertical
                size: select_LISTVIEW.height / contentItem.height
//                policy: ScrollBar.AlwaysOn
            }
            ListView {
                id: select_LISTVIEW
                anchors.fill: parent
    //            width: parent.width; height: parent.height - select_ROWLAYOUT_prefix.height - select_BUTTON_write.height
                model: myserial.model
                delegate: select_COMPONENT_delegate
                header: select_COMOPONENT_header
                snapMode: ListView.SnapToItem
                property real idWidth: 90
                property real imeiWidth: 110
                property real userWidth: 60
                property real versionWidth: 40
                property real timeWidth: 200
                property real checkWidth: 60
            }
        }
        Button {
            id: select_BUTTON_write
//            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
//            Layout.preferredWidth: 80; Layout.preferredHeight: 50
            width: 80; height: 50
            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.bottom: parent.bottom; anchors.bottomMargin: 10
            text: qsTr("写入")
            enabled: myserial.model.groupState !== DeviceModel.Unchecked
            onClicked: select_FILEDIALOG.open()
        }
    }
    Component {
        id: select_COMPONENT_delegate
        Item {
            id: select_ITEM_delegate
            width: select_LISTVIEW.width; height: 30
            required property string id
            required property string imei
            required property string user
            required property string version
            required property string time
            required property int index
            property color backgroundColor: index % 2 == 0 ? "white" : Qt.lighter("blue", 1.5)
            property color fontColor: "black"
            Rectangle {
                id: select_RECTANGLE_delegate
                anchors.fill: parent
                color: parent.backgroundColor
                border.color: "grey"
                RowLayout {
                    anchors.fill: parent
                    Text {
                        Layout.preferredWidth: select_LISTVIEW.idWidth - 1 - Layout.leftMargin; Layout.fillHeight: true
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        Layout.leftMargin: 5
                        verticalAlignment: Text.AlignVCenter
                        text: select_ITEM_delegate.id
                        color: select_ITEM_delegate.fontColor
                    }
                    Rectangle {
                        Layout.preferredWidth: 1; Layout.fillHeight: true
                        color: "grey"
                    }
                    Text {
                        Layout.preferredWidth: select_LISTVIEW.imeiWidth - 1; Layout.fillHeight: true
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        verticalAlignment: Text.AlignVCenter
                        text: select_ITEM_delegate.imei
                        color: select_ITEM_delegate.fontColor
                    }
                    Rectangle {
                        Layout.preferredWidth: 1; Layout.fillHeight: true
                        color: "grey"
                    }
                    Text {
                        Layout.preferredWidth: select_LISTVIEW.userWidth - 1; Layout.fillHeight: true
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        verticalAlignment: Text.AlignVCenter
                        text: select_ITEM_delegate.user
                        color: select_ITEM_delegate.fontColor
                    }
                    Rectangle {
                        Layout.preferredWidth: 1; Layout.fillHeight: true
                        color: "grey"
                    }
                    Text {
                        Layout.preferredWidth: select_LISTVIEW.versionWidth - 1; Layout.fillHeight: true
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        verticalAlignment: Text.AlignVCenter
                        text: select_ITEM_delegate.version
                        color: select_ITEM_delegate.fontColor
                    }
                    Rectangle {
                        Layout.preferredWidth: 1; Layout.fillHeight: true
                        color: "grey"
                    }
                    Text {
//                        Layout.preferredWidth: select_LISTVIEW.timeWidth;
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                        verticalAlignment: Text.AlignVCenter
                        text: select_ITEM_delegate.time
                        color: select_ITEM_delegate.fontColor
                    }
                    Rectangle {
                        Layout.preferredWidth: 1; Layout.fillHeight: true
                        color: "grey"
                    }
                    CheckBox {
//                        Layout.fillWidth: true
                        Layout.preferredWidth: select_LISTVIEW.checkWidth; Layout.fillHeight: true
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        leftPadding: 0
                        checkState: Qt.Unchecked
                        tristate: false
                        ButtonGroup.group: select_BUTTONGROUP
                        onToggled: {
                            if (checkState === Qt.Checked) myserial.onSelected(select_ITEM_delegate.index, true)
                            else if (checkState === Qt.Unchecked) myserial.onSelected(select_ITEM_delegate.index, false)
                        }
                    }
                }
            }
        }
    }

    Component {
        id: select_COMOPONENT_header
        Item {
            id: select_ITEM_header
            width: select_LISTVIEW.width; height: 30
            Rectangle {
                id: select_RECTANGLE_header
                anchors.fill: parent
                color: "blue"
                border.color: "grey"
                RowLayout {
                    anchors.fill: parent
                    Text {
                        Layout.preferredWidth: select_LISTVIEW.idWidth - 1; Layout.fillHeight: true
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                        text: qsTr("ID")
                        font.bold: true; font.pixelSize: 18; color: "white"
                    }
                    Rectangle {
                        Layout.preferredWidth: 1; Layout.fillHeight: true
                        color: "white"
                    }
                    Text {
                        Layout.preferredWidth: select_LISTVIEW.imeiWidth - 1; Layout.fillHeight: true
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                        text: qsTr("IMEI")
                        font.bold: true; font.pixelSize: 18; color: "white"
                    }
                    Rectangle {
                        Layout.preferredWidth: 1; Layout.fillHeight: true
                        color: "white"
                    }
                    Text {
                        Layout.preferredWidth: select_LISTVIEW.userWidth - 1; Layout.fillHeight: true
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                        text: qsTr("USER")
                        font.bold: true; font.pixelSize: 18; color: "white"
                    }
                    Rectangle {
                        Layout.preferredWidth: 1; Layout.fillHeight: true
                        color: "white"
                    }
                    Text {
                        Layout.preferredWidth: select_LISTVIEW.versionWidth - 1; Layout.fillHeight: true
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                        text: qsTr("VER")
                        font.bold: true; font.pixelSize: 18; color: "white"
                    }
                    Rectangle {
                        Layout.preferredWidth: 1; Layout.fillHeight: true
                        color: "white"
                    }
                    Text {
                        Layout.fillWidth: true; Layout.fillHeight: true
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                        text: qsTr("TIME")
                        font.bold: true; font.pixelSize: 18; color: "white"
                    }
                    Rectangle {
                        Layout.preferredWidth: 1; Layout.fillHeight: true
                        color: "white"
                    }
                    CheckBox {
                        Layout.preferredWidth: select_LISTVIEW.checkWidth; Layout.fillHeight: true
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        leftPadding: 0
                        checkState: myserial.model.groupState
                        tristate: true
                        icon.color: "white"
                        nextCheckState: function() {
                            if (checkState == Qt.Unchecked || checkState == Qt.PartiallyChecked) {
                                select_BUTTONGROUP.toggleAll(true)
                                return Qt.Checked
                            } else {
                                select_BUTTONGROUP.toggleAll(false)
                                return Qt.Unchecked
                            }
                        }
                    }
                }
            }
        }
    }

    ButtonGroup {
        id: select_BUTTONGROUP
        checkState: Qt.Unchecked
        exclusive: false
        function toggleAll(op) {
            for (var i = 0; i < buttons.length; ++i) {
                if (op === true) {
                    buttons[i].checkState = Qt.Checked
                    myserial.onSelected(i, true)
                } else {
                    buttons[i].checkState = Qt.Unchecked
                    myserial.onSelected(i, false)
                }
            }
        }
    }
    /*
    Connections {
        target: myserial
        function onPrefixValid(valid) {
            if (valid === false) {
                select_RECTANGLE_prefix.state = "INVALID"
                mytoast.show(qsTr("前缀格式错误！"), 3000)
            }
            else select_RECTANGLE_prefix.state = "VALID"
        }
    }
    */
    Connections {
        target: myserial
        function onFileChanged(path) {
            mytoast.show(qsTr("IMEI json文件变更！"), 3000)
        }
    }

    Platform.FileDialog {
        id: select_FILEDIALOG
        fileMode: FileDialog.SaveFile
        defaultSuffix: "json"
        nameFilters: ["Text files (*.json)"]
        folder: Platform.StandardPaths.writableLocation(Platform.StandardPaths.HomeLocation)
        onAccepted: {
            var filedir = select_FILEDIALOG.currentFile.toString()
            filedir = filedir.replace(/^(file:\/{3})/,"")
            console.log("saving file: " + filedir)
            myserial.onSaveDevices(filedir)
        }
    }

    Component.onCompleted: {
//        select_RECTANGLE_prefix.state = "VALID"
        myserial.onSelectPrefix("")
    }
}
