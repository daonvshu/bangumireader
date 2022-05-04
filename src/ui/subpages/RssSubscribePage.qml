import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import RssSubscribeModel 0.1
import "../widgets"

Rectangle {
    id: root
    color: "transparent"

    signal backToHomePage
    signal gotoSourceLinkPage(int id, string title)

    MouseArea {
        anchors.fill: parent
        onClicked: focus = true
    }

    RssSubscribeModel{
        id: rssSubscribeModel
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        Row {
            spacing: 4

            IconBtn {
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/resource/ic_previous.png"
                width: 20
                height: 20
        
                onClicked: backToHomePage()
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter

                font.pixelSize: 16
                font.bold: true
                color: "#BF9270"
                text: "订阅列表"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            color: "#E6DDC6"
            radius: 12

            ListView {
                id: rssLinkList
                anchors.fill: parent
                anchors.leftMargin: 8
                anchors.topMargin: 8
                anchors.bottomMargin: 8

                model: rssSubscribeModel
                clip: true

                populate: Transition {
                    NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 250 }
                }

                delegate: Rectangle {
                    id: itemRow
                    width: rssLinkList.width - 8
                    height: 32
                    radius: 4

                    property var isHoverd: false

                    color: isHoverd ? "#FFBC80" : "#E6DDC6"
                    Behavior on color {
                        ColorAnimation {
                            duration: 200
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: isHoverd = true
                        onExited: isHoverd = false
                        onClicked: gotoSourceLinkPage(display.bangumiId, display.title)
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 4
                        anchors.rightMargin: 4

                        Text {
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter

                            text: getDisplayText(display)
                            font.pixelSize: 12
                            color: "#BF9270"
                            elide: Text.ElideMiddle
                        }
                    }
                }

                ScrollBar.vertical: ScrollBar { 
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                }
            }
        }
    }

    function getDisplayText(display) {
        var newSize = display.extra.newSize
        if (newSize == 0) {
            return display.title
        }
        return display.title + " (" + newSize + "+)"
    }

    onVisibleChanged: {
        if (visible) {
            rssSubscribeModel.refreshAll()
        }
    }
}