import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.12

import "../widgets"

Rectangle {
    height: 32
    color: "transparent"

    signal rssClicked

    Image {
        id: headerIcon
        anchors.verticalCenter: parent.verticalCenter
        sourceSize {
            width: 24
            height: 24
        }
        source: "qrc:/log.ico"
    }

    Text {
        anchors.left: headerIcon.right
        anchors.leftMargin: 8
        anchors.verticalCenter: parent.verticalCenter

        font.pixelSize: 14
        font.bold: true
        color: "#BF9270"
        text: "BangumiReader"
    }

    MouseArea {
        anchors.fill: parent
        
        property var clickPos

        onPressed: {
            clickPos = Qt.point(mouse.x, mouse.y)
        }

        onPositionChanged: {
            var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
            mainWindow.setX(mainWindow.x + delta.x)
            mainWindow.setY(mainWindow.y + delta.y)
        }
    }

    IconBtn {
        id: btnClose
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.verticalCenter: parent.verticalCenter

        source: "../../resource/ic_remove.png"

        onClicked: mainWindow.hide()
    }

    IconBtn {
        anchors.right: btnClose.left
        anchors.rightMargin: 8
        anchors.verticalCenter: parent.verticalCenter

        source: "../../resource/ic_rss.png"

        onClicked: rssClicked()
    }
}