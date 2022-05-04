import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.15

Window {
    id: root

    property var links: []

    width: 400
    height: links.length * 28 + 60

    x: screen.width - width
    y: screen.desktopAvailableHeight - height

    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    color: "transparent"

    signal linkClicked(int id, string title)

    Rectangle {
        id: bodyRect
        anchors.fill: parent
        anchors.margins: 6

        radius: 9
        color: "#FEF7DC"

        Column {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 8

            Row {
                spacing: 4

                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    sourceSize.width: 16
                    source: "qrc:/resource/ic_notice.png"
                }

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: "更新提示"
                    font.pixelSize: 14
                    color: "#BF9270"
                }
            }

            Rectangle {
                width: root.width - 30
                height: 1
                color: "#EDCDBB"
            }

            Repeater {
                model: links
                delegate: Text {
                    id: titleText
                    width: root.width - 30
                    height: 28 - 8

                    property bool hovered

                    text: "[" + modelData.group + "]" + modelData.title
                    font.pixelSize: 12
                    font.underline: hovered

                    color: hovered ? "#E3B7A0" : "#BF9270"
                    elide: Text.ElideMiddle

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: titleText.hovered = true
                        onExited: titleText.hovered = false
                        onClicked: {
                            linkClicked(modelData.id, modelData.title)
                            hovered = false
                            root.close()
                        }
                    }
                }
            }
        }
    }

    DropShadow {
        anchors.fill: bodyRect
        radius: 8.0
        samples: 16
        color: "#BF9270"
        source: bodyRect
    }

    Timer {
        interval: 10000
        running: visible
        onTriggered: root.close()
    }
}