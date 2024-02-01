import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform as Platform
import SerialMoveToThread 1.0

Page {
    anchors.fill: parent
    Column {
        id: input_COLUMN
        anchors.fill: parent
        anchors.margins: 10
        Text {
            id: input_TEXT_imei
            width: parent.width; height: 40
            verticalAlignment: Text.AlignVCenter
            text: qsTr("IMEI: " + myserial.imei)
            font.pixelSize: 16
        }
        RowLayout {
            width: parent.width; height: 40
            Text {
                Layout.preferredWidth: 100; Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter
                verticalAlignment: Text.AlignVCenter
                text: qsTr("请输入设备ID:")
                font.pixelSize: 16
            }
            Rectangle {
                id: input_RECTANGLE_id
                Layout.fillWidth: true; Layout.preferredHeight: 30
                color: "white"
                TextInput {
                    id: input_TEXTINPUT_id
                    anchors.fill: parent; anchors.leftMargin: 5
                    verticalAlignment: Text.AlignVCenter
                    onEditingFinished: {
                        if (text !== qsTr("")) myserial.checkValid(SerialMoveToThread.ID, text)
                        else input_RECTANGLE_id.state = "VALID"
                    }
                }
                states: [
                    State {
                        name: "DISABLED"
                        PropertyChanges {
                            target: input_RECTANGLE_id
                            border.color: Qt.lighter("grey", 1.5)
                        }
                        PropertyChanges {
                            target: input_TEXTINPUT_id
                            enabled: false
                            cursorVisible: false
                        }
                    },
                    State {
                        name: "VALID"
                        PropertyChanges {
                            target: input_RECTANGLE_id
                            border.color: "black"
                        }
                        PropertyChanges {
                            target: input_TEXTINPUT_id
                            enabled: true
                            cursorVisible: true
                        }
                    },
                    State {
                        name: "INVALID"
                        PropertyChanges {
                            target: input_RECTANGLE_id
                            border.color: "red"
                        }
                        PropertyChanges {
                            target: input_TEXTINPUT_id
                            enabled: true
                            cursorVisible: true
                        }
                    }
                ]
            }
        }
        RowLayout {
            width: parent.width; height: 40
            Text {
                Layout.preferredWidth: 100; Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter
                verticalAlignment: Text.AlignVCenter
                text: qsTr("请输入用户名:")
                font.pixelSize: 16
            }
            Rectangle {
                id: input_RECTANGLE_user
                Layout.fillWidth: true; Layout.preferredHeight: 30
                color: "white"
                TextInput {
                    id: input_TEXTINPUT_user
                    anchors.fill: parent; anchors.leftMargin: 5
                    verticalAlignment: Text.AlignVCenter
                    onEditingFinished: myserial.user = text
                }
                states: [
                    State {
                        name: "DISABLED"
                        PropertyChanges {
                            target: input_RECTANGLE_user
                            border.color: Qt.lighter("grey", 1.5)
                        }
                        PropertyChanges {
                            target: input_TEXTINPUT_user
                            enabled: false
                            cursorVisible: false
                        }
                    },
                    State {
                        name: "ENABLED"
                        PropertyChanges {
                            target: input_RECTANGLE_user
                            border.color: "black"
                        }
                        PropertyChanges {
                            target: input_TEXTINPUT_user
                            enabled: true
                            cursorVisible: true
                        }
                    }
                ]
            }
        }
        RowLayout {
            width: parent.width; height: 40
            Text {
                Layout.preferredWidth: 100; Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter
                verticalAlignment: Text.AlignVCenter
                text: qsTr("请输入版本号:")
                font.pixelSize: 16
            }
            Rectangle {
                id: input_RECTANGLE_version
                Layout.fillWidth: true; Layout.preferredHeight: 30
                color: "white"
                TextInput {
                    id: input_TEXTINPUT_version
                    anchors.fill: parent; anchors.leftMargin: 5
                    verticalAlignment: Text.AlignVCenter
                    onEditingFinished: {
                        if (text !== qsTr("")) myserial.checkValid(SerialMoveToThread.VERSION, text)
                        else input_RECTANGLE_version.state = "VALID"
//                        console.log("version: " + myserial.version)
                    }
                }
                states: [
                    State {
                        name: "DISABLED"
                        PropertyChanges {
                            target: input_RECTANGLE_version
                            border.color: Qt.lighter("grey", 1.5)
                        }
                        PropertyChanges {
                            target: input_TEXTINPUT_version
                            enabled: false
                            cursorVisible: false
                        }
                    },
                    State {
                        name: "VALID"
                        PropertyChanges {
                            target: input_RECTANGLE_version
                            border.color: "black"
                        }
                        PropertyChanges {
                            target: input_TEXTINPUT_version
                            enabled: true
                            cursorVisible: true
                        }
                    },
                    State {
                        name: "INVALID"
                        PropertyChanges {
                            target: input_RECTANGLE_version
                            border.color: "red"
                        }
                        PropertyChanges {
                            target: input_TEXTINPUT_version
                            enabled: true
                            cursorVisible: true
                        }
                    }
                ]
            }
        }
    }

    Button {
        width: 80; height: 60
        x: (parent.width + width) * 0.5
        anchors.bottom: parent.bottom; anchors.bottomMargin: 100
        text: qsTr("确定")
        enabled: myserial.imei !== qsTr("") && myserial.id !== qsTr("") && myserial.user !== qsTr("") && myserial.version !== qsTr("")
        onClicked: {
            if (!myserial.fileok) {
                input_TEXTINPUT_id.enabled = false
                input_MESSAGEDIALOG_nofile.open()
            }
            else myserial.onWriteId(input_TEXTINPUT_id.text)
        }
    }

    Button {
        width: 80; height: 60
        x: (parent.width - width * 3) * 0.5
        anchors.bottom: parent.bottom; anchors.bottomMargin: 100
        text: qsTr("刷新")
        onClicked: {
            input_TEXTINPUT_id.clear()
            input_TEXTINPUT_version.clear()
            myserial.onRefresh()
        }
    }

    Platform.MessageDialog {
        id: input_MESSAGEDIALOG_overwrite
        text: qsTr("此设备记录已存在,是否覆盖?")
        buttons: Platform.MessageDialog.Ok | Platform.MessageDialog.Cancel
        onOkClicked: {
            myserial.onConfirmId(input_TEXTINPUT_id.text)
            input_MESSAGEDIALOG_overwrite.close()
            input_TEXTINPUT_id.enabled = true
        }
        onCancelClicked: {
            input_MESSAGEDIALOG_overwrite.close()
            input_TEXTINPUT_id.enabled = true
        }
    }

    Platform.MessageDialog {
        id: input_MESSAGEDIALOG_nofile
        text: qsTr("是否写入新文件?")
        buttons: Platform.MessageDialog.Ok | Platform.MessageDialog.Cancel
        onOkClicked: {
            myserial.onWriteId(input_TEXTINPUT_id.text)
            input_MESSAGEDIALOG_overwrite.close()
            input_TEXTINPUT_id.enabled = true
        }
        onCancelClicked: {
            input_MESSAGEDIALOG_overwrite.close()
            input_TEXTINPUT_id.enabled = true
        }
    }

    StateGroup {
        id: input_STATEGROUP
        states: [
            State {
                name: "UNCONNECTED"
                PropertyChanges {
                    target: input_RECTANGLE_id
                    state: "DISABLED"
                }
                PropertyChanges {
                    target: input_RECTANGLE_user
                    state: "DISABLED"
                }
                PropertyChanges {
                    target: input_RECTANGLE_version
                    state: "DISABLED"
                }
            },
            State {
                name: "CONNECTED"
                PropertyChanges {
                    target: input_RECTANGLE_id
                    state: "VALID"
                }
                PropertyChanges {
                    target: input_RECTANGLE_user
                    state: "ENABLED"
                }
                PropertyChanges {
                    target: input_RECTANGLE_version
                    state: "VALID"
                }
            }
        ]
    }

    Connections {
        target: myserial
        function onRecordExists(imei) {
            console.log("record exists")
            input_TEXTINPUT_id.enabled = false
            input_MESSAGEDIALOG_overwrite.open()
        }
    }

    Connections {
        target: myserial
        function onConnChanged(conn) {
            if (conn === SerialMoveToThread.Disconnected) {
                input_STATEGROUP.state = "UNCONNECTED"
                console.log("serial disconnected")
                input_TEXTINPUT_id.clear()
                commodel.refresh()
            }
        }
    }

    Connections {
        target: myserial
        function onIdValid(valid) {
            if (valid === false) {
                input_RECTANGLE_id.state = "INVALID"
                mytoast.show(qsTr("ID格式错误!"), 3000)
            }
            else input_RECTANGLE_id.state = "VALID"
        }
    }

    Connections {
        target: myserial
        function onVersionValid(valid) {
            if (valid === false) {
                input_RECTANGLE_version.state = "INVALID"
                mytoast.show(qsTr("版本格式错误!"), 3000)
            }
            else input_RECTANGLE_version.state = "VALID"
        }
    }

    Connections {
        target: myserial
        function onImeiChanged(imei) {
            if (imei !== qsTr("")) input_STATEGROUP.state = "CONNECTED"
        }
    }

    Connections {
        target: myserial
        function onBlankImei() {
            input_STATEGROUP.state = "UNCONNECTED"
            mytoast.show(qsTr("IMEI无效!"), 3000)
        }
    }

    Component.onCompleted: {
//        input_RECTANGLE_id.state = "DISABLED"
//        input_RECTANGLE_user.state = "DISABLED"
//        input_RECTANGLE_version.state = "DISABLED"
        input_STATEGROUP.state = "UNCONNECTED"
    }
}
