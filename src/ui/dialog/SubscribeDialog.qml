import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

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

    property var subscribeData
    property var bangumiTitle: ""

    signal saved(string keywords)
    signal cancelSubscribe()

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
                source: "qrc:/resource/ic_rss.png"
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
                text: bangumiTitle
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
                text: subscribeData.groupName
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
                text: subscribeData.extra.rssLink
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
                    text: subscribeData.keywords

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
                icon.source: "qrc:/resource/ic_down.png"
                onClicked: {
                    saved(keywordsInput.text)
                    root.close()
                }
            }
    
            IconBtn2 {
                text: "取消订阅"
                icon.source: "qrc:/resource/ic_remove.png"
                onClicked: {
                    cancelSubscribe()
                    root.close()
                }
            }
        }
    }
}