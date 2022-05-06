import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.12

import "../widgets"

Rectangle {
    height: 32
    color: "transparent"

    signal rssClicked
    signal settingClicked
    signal closeClicked
    signal databaseClicked

    Image {
        id: headerIcon
        anchors.verticalCenter: parent.verticalCenter
        sourceSize {
            width: 24
            height: 24
        }
        source: "qrc:/resource/logo.png"
    }

    Text {
        anchors.left: headerIcon.right
        anchors.leftMargin: 8
        anchors.verticalCenter: parent.verticalCenter

        font.pixelSize: 14
        font.bold: true
        color: "#BF9270"
        text: "BangumiReader " + appVersion
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

    Row {
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.verticalCenter: parent.verticalCenter

        spacing: 8

        IconBtn {
            source: "qrc:/resource/ic_github.png"
            onClicked: Qt.openUrlExternally("https://github.com/daonvshu/bangumireader")
        }

        IconBtn {
            source: "qrc:/resource/ic_database.png"
            onClicked: databaseClicked()
        }

        IconBtn {
            source: "qrc:/resource/ic_setting.png"
            onClicked: settingClicked()
        }

        IconBtn {
            source: "qrc:/resource/ic_rss.png"
            onClicked: rssClicked()
        }

        IconBtn {
            source: "qrc:/resource/ic_remove.png"
            onClicked: closeClicked()
        }
    }
}