import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../widgets"

Dialog {
    id: root
    width: 420
    height: 300

    x: (mainWindow.width - width) / 2
    y: (mainWindow.height - height) / 2 - 32

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

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
                source: "../../resource/ic_rss.png"
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "订阅字幕组"
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

        MouseArea {
            anchors.fill: parent
            onClicked: focus = true
        }

        GridLayout {
            columns: 2
            rowSpacing: 16
            columnSpacing: 12

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 12

            Text {
                text: "番剧名："
                font.pixelSize: 16
                color: "#BF9270"
            }

            Text {
                Layout.fillWidth: true
                text: "番剧名"
                font.pixelSize: 16
                color: "#BF9270"
                elide: Text.ElideMiddle
            }

            Text {
                text: "字幕组："
                font.pixelSize: 16
                color: "#BF9270"
            }

            Text {
                Layout.fillWidth: true
                text: "字幕组"
                font.pixelSize: 16
                color: "#BF9270"
                elide: Text.ElideMiddle
            }

            Text {
                text: "订阅地址："
                font.pixelSize: 16
                color: "#BF9270"
            }

            Text {
                Layout.fillWidth: true
                text: "订阅地址"
                font.pixelSize: 16
                color: "#BF9270"
                elide: Text.ElideMiddle
            }

            Text {
                text: "关键词筛选："
                font.pixelSize: 16
                color: "#BF9270"
            }

            Rectangle {
                Layout.fillWidth: true
                height: 32
                color: "transparent"

                TextField {
                    anchors.verticalCenter: parent.verticalCenter
                    id: keywordsInput

                    clip: true

                    background: Rectangle {
                        implicitWidth: 200
                        implicitHeight: 32
                        color: "#EDCDBB"
                        radius: 8
                    }
                }

                KeywordNotice {
                    anchors.left: keywordsInput.right
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter

                    Layout.alignment: Qt.AlignVCenter
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
                text: "保存设置"
                icon.source: "../../resource/ic_down.png"
            }
    
            IconBtn2 {
                text: "取消订阅"
                icon.source: "../../resource/ic_remove.png"
            }
        }
    }
}