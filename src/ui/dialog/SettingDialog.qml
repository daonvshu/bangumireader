import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import QmlSettingDialog 0.1
import "../widgets"

Dialog {
    id: root
    width: 480
    height: 300

    x: (mainWindow.width - width) / 2
    y: (mainWindow.height - height) / 2 - 32

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    QmlSettingDialog {
        id: qmlSetting
    }

    background: Rectangle {
        color: "#FEF7DC"

        radius: 6
    }

    header: Rectangle {
        height: 42

        color: "transparent"
        Row {
            anchors.fill: parent
            anchors.leftMargin: 6

            spacing: 4

            Image {
                anchors.verticalCenter: parent.verticalCenter
                sourceSize.width: 16
                source: "qrc:/resource/ic_setting.png"
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "设置"
                font.pixelSize: 14
                color: "#BF9270"
            }
        }

        Rectangle {
            width: parent.width
            height: 1

            anchors.bottom: parent.bottom
            anchors.bottomMargin: 1
            color: "#EDCDBB"
        }
    }

    contentItem: Rectangle {
        color: "transparent"

        ColumnLayout {
            spacing: 12

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 12

            ColorCheckBox {
                text: "播放提示音"
                isChecked: qmlSetting.playSound
                onCheckedChanged: qmlSetting.playSound = checked
            }

            ColorCheckBox {
                text: "开机自启动"
                isChecked: qmlSetting.autoStart
                onCheckedChanged: qmlSetting.autoStart = checked
            }

            Row {
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: "定时同步间隔："
                    font.pixelSize: 16
                    color: "#BF9270"
                }

                RoundComboBox {
                    model: getSyncModel("text")
                    currentIndex: getSyncModel("data").indexOf(qmlSetting.syncInterval)

                    onActivated: {
                        qmlSetting.syncInterval = getSyncModel("data")[index]
                    }
                }
            }
        }
    }

    footer: Rectangle {
        height: 48

        color: "transparent"

        RowLayout {
            anchors.right: parent.right
            anchors.rightMargin: 12

            spacing: 12
    
            IconBtn2 {
                text: "关闭"
                icon.source: "qrc:/resource/ic_remove.png"
                onClicked: {
                    root.close()
                }
            }
        }
    }

    function getSyncModel(key) {
        var model = [
            {"text": "1分钟", "data": 1 * 60},
            {"text": "5分钟", "data": 5 * 60},
            {"text": "15分钟", "data": 15 * 60},
            {"text": "30分钟", "data": 30 * 60},
            {"text": "1小时", "data": 1 * 3600},
            {"text": "2小时", "data": 2 * 3600},
            {"text": "4小时", "data": 4 * 3600},
            {"text": "6小时", "data": 6 * 3600},
            {"text": "12小时", "data": 12 * 3600},
            {"text": "24小时", "data": 24 * 3600},
        ]
        return model.map((v) => v[key])
    }
}