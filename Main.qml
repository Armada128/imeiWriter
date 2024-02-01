import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import SerialMoveToThread 1.0
import Qt.labs.platform as Platform

ApplicationWindow {
    id: window
    minimumWidth: 640
    minimumHeight: 640
    visible: true
    title: qsTr("IMEI")

    header: ToolBar {
        id: main_TOOLBAR
        ToolButton {
            id: main_TOOLBUTTON_serial
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: qsTr("连接设置")
            display: AbstractButton.TextOnly
            onClicked: main_DIALOG_serial.open()
        }
    }

    TabBar {
        id: main_TABBAR
        width: parent.width
        anchors.top: main_TOOLBAR.bottom
        TabButton {
            id: main_TABBUTTON_input
            text: qsTr("录入")
        }
        TabButton {
            id: main_TABBUTTON_select
            text: qsTr("选择设备")
        }
    }

    StackLayout {
        id: main_STACKLAYOUT
        anchors.top: main_TABBAR.bottom
        currentIndex: main_TABBAR.currentIndex
        width: parent.width; height: parent.height - main_TABBAR.height
        Item {
            id: main_INPUT
            Input {}
        }
        Item {
            id: main_SELECT
            Select {}
        }
    }

    Dialog {
        id: main_DIALOG_serial
        implicitWidth: 250; implicitHeight: 450
        x: 0.5 * (parent.width - width); y: 0.5 * (parent.height - height)
        modal: true
        background: Rectangle {
            width: main_DIALOG_serial.width; height: main_DIALOG_serial.height
        }
        header: ToolBar {
            height: 40
            Label {
                anchors.centerIn: parent
                text: qsTr("串口连接")
                font.pixelSize: 16
                color: "white"
            }
            ToolButton {
                anchors.right: parent.right; anchors.rightMargin: 2
                anchors.verticalCenter: parent.verticalCenter
                icon.source: "assets/close.svg"
                onClicked: {
                    main_DIALOG_serial.close()
                }
            }
        }
        topPadding: 0; bottomPadding: 5; leftPadding: 5; rightPadding: 5
        property real cellheight: 40
        property real textsize: 16
        property real textwidth: 80
        ColumnLayout {
            anchors.fill: parent
            spacing: 0
            Rectangle {
                Layout.fillWidth: true; Layout.preferredHeight: main_DIALOG_serial.cellheight
                RowLayout {
                    anchors.fill: parent
                    Label {
                        Layout.preferredWidth: main_DIALOG_serial.textwidth; Layout.fillHeight: true
                        Layout.alignment: Qt.AlignVCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: main_DIALOG_serial.textsize
                        text: qsTr("端口:")
                    }
                    ComboBox {
                        id: main_COMBOBOX_com
                        Layout.fillWidth: true; Layout.fillHeight: true
                        model: commodel
                        textRole: "display"
                        valueRole: "display"
                        displayText: currentText
                        enabled: myserial.conn === SerialMoveToThread.Disconnected
                    }
                }
            }
            Rectangle {
                Layout.fillWidth: true; Layout.preferredHeight: main_DIALOG_serial.cellheight
                RowLayout {
                    anchors.fill: parent
                    Label {
                        Layout.preferredWidth: main_DIALOG_serial.textwidth; Layout.fillHeight: true
                        Layout.alignment: Qt.AlignVCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: main_DIALOG_serial.textsize
                        text: qsTr("校验:")
                    }
                    ComboBox {
                        id: main_COMBOBOX_parity
                        Layout.fillWidth: true; Layout.fillHeight: true
                        model: paritymodel
                        textRole: "name"
                        valueRole: "value"
                        currentIndex: 2
                        enabled: myserial.conn === SerialMoveToThread.Disconnected
                    }
                }
            }
            Rectangle {
                Layout.fillWidth: true; Layout.preferredHeight: main_DIALOG_serial.cellheight
                RowLayout {
                    anchors.fill: parent
                    Label {
                        Layout.preferredWidth: main_DIALOG_serial.textwidth; Layout.fillHeight: true
                        Layout.alignment: Qt.AlignVCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: main_DIALOG_serial.textsize
                        text: qsTr("波特率:")
                    }
                    ComboBox {
                        id: main_COMBOBOX_baud
                        Layout.fillWidth: true; Layout.fillHeight: true
                        model: baudmodel
                        textRole: "display"
                        valueRole: "diaplay"
                        currentIndex: 7
                        enabled: myserial.conn === SerialMoveToThread.Disconnected
                    }
                }
            }
            Rectangle {
                Layout.fillWidth: true; Layout.preferredHeight: main_DIALOG_serial.cellheight
                RowLayout {
                    anchors.fill: parent
                    Label {
                        Layout.preferredWidth: main_DIALOG_serial.textwidth; Layout.fillHeight: true
                        Layout.alignment: Qt.AlignVCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: main_DIALOG_serial.textsize
                        text: qsTr("数据位:")
                    }
                    ComboBox {
                        id: main_COMBOBOX_databits
                        Layout.fillWidth: true; Layout.fillHeight: true
                        model: databitsmodel
                        textRole: "display"
                        valueRole: "diaplay"
                        currentIndex: 3
                        enabled: myserial.conn === SerialMoveToThread.Disconnected
                    }
                }
            }
            Rectangle {
                Layout.fillWidth: true; Layout.preferredHeight: main_DIALOG_serial.cellheight
                RowLayout {
                    anchors.fill: parent
                    Label {
                        Layout.preferredWidth: main_DIALOG_serial.textwidth; Layout.fillHeight: true
                        Layout.alignment: Qt.AlignVCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: main_DIALOG_serial.textsize
                        text: qsTr("停止位:")
                    }
                    ComboBox {
                        id: main_COMBOBOX_stopbits
                        Layout.fillWidth: true; Layout.fillHeight: true
                        model: stopbitsmodel
                        textRole: "display"
                        valueRole: "diaplay"
                        enabled: myserial.conn === SerialMoveToThread.Disconnected
                    }
                }
            }
            Rectangle {
                Layout.fillWidth: true; Layout.preferredHeight: main_DIALOG_serial.cellheight
                RowLayout {
                    anchors.fill: parent
                    Label {
                        Layout.preferredWidth: main_DIALOG_serial.textwidth; Layout.fillHeight: true
                        Layout.alignment: Qt.AlignVCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: main_DIALOG_serial.textsize
                        text: qsTr("流控制:")
                    }
                    ComboBox {
                        id: main_COMBOBOX_flowcontrol
                        Layout.fillWidth: true; Layout.fillHeight: true
                        model: flowcontrolmodel
                        textRole: "type"
                        valueRole: "value"
                        enabled: myserial.conn === SerialMoveToThread.Disconnected
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true; Layout.preferredHeight: main_DIALOG_serial.cellheight
                RowLayout {
                    anchors.fill: parent
                    Button {
                        id: main_BUTTON_apply
                        Layout.preferredWidth: 100; Layout.preferredHeight: 45
                        Layout.alignment: Qt.AlignCenter
                        text: myserial.conn === SerialMoveToThread.Connected ? qsTr("断开") : qsTr("连接")
                        enabled: {
                            if (myserial.conn === SerialMoveToThread.Connected) return true
                            else return main_COMBOBOX_com.currentText === qsTr("") ? false : true
                        }
                        onClicked: {
                            if(myserial.conn === SerialMoveToThread.Connected) myserial.disconnectSerial()
                            else {
                                commodel.onComSelected(main_COMBOBOX_com.currentIndex, main_COMBOBOX_com.currentText)
                                paritymodel.onParitySelected(main_COMBOBOX_parity.currentIndex, main_COMBOBOX_parity.currentText)
                                baudmodel.onBaudSelected(main_COMBOBOX_baud.currentIndex, main_COMBOBOX_baud.currentText)
                                databitsmodel.onDatabitsSelected(main_COMBOBOX_databits.currentIndex, main_COMBOBOX_databits.currentText)
                                stopbitsmodel.onStopbitsSelected(main_COMBOBOX_stopbits.currentIndex, main_COMBOBOX_stopbits.currentText)
                                flowcontrolmodel.onFlowcontrolSelected(main_COMBOBOX_flowcontrol.currentIndex, main_COMBOBOX_flowcontrol.currentText)
                                myserial.connectSerial()
                            }
                            main_DIALOG_serial.close()
                        }
                    }
                }
            }

        }
    }

    SerialMoveToThread {
        id: myserial
    }

    Connections {
        target: commodel
        function onComSelect(com) {
            myserial.onCom(com)
        }
    }

    Connections {
        target: paritymodel
        function onParitySelect(parity) {
            myserial.onParity(parity)
        }
    }

    Connections {
        target: baudmodel
        function onBaudSelect(baud) {
            myserial.onBaud(baud)
        }
    }

    Connections {
        target: databitsmodel
        function onDatabitsSelect(databits) {
            myserial.onParity(databits)
        }
    }

    Connections {
        target: stopbitsmodel
        function onStopbitsSelect(stopbits) {
            myserial.onStopbits(stopbits)
        }
    }

    Connections {
        target: flowcontrolmodel
        function onFlowcontrolSelect(flowcontrol) {
            myserial.onFlowcontrol(flowcontrol)
        }
    }

    Connections {
        target: myserial
        function onIdWritten() {
            mytoast.show(qsTr("ID已成功写入"), 3000)
        }
    }

    Connections {
        target: myserial
        function onRecorded() {
            mytoast.show(qsTr("记录已成功写入"), 3000)
        }
    }

    MyToast {
        id: mytoast
    }



//    Timer {
//        interval: 1000; running: true; repeat: true
//        onTriggered: console.log("current com: " + main_COMBOBOX_com.currentText + ", " + main_COMBOBOX_com.currentValue + ", " + main_COMBOBOX_com.currentIndex + ", " + commodel.rowCount())
//    }
}
