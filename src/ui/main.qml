import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

Item {
    width: 1280
    height: 768
    visible: true
    
    Rectangle {
        id: bodyRect
        anchors.fill: parent
        anchors.margins: 6

        radius: 9
        color: "#FFEDDB"

        Rectangle {
            id: header
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 6
            
            height: 32
            color: "transparent"

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

            Text {
                anchors.right: parent.right
                anchors.rightMargin: 8

                font.family: "Material Design Icons"
                font.pixelSize: 30
                text: "\uf06C9"

                MouseArea {
                    anchors.fill: parent
                    onClicked: mainWindow.hide()
                }
            }
        }

        Rectangle {
            anchors.top: header.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 1

            color: "#EDCDBB"
        }
    }

    DropShadow {
        anchors.fill: bodyRect
        radius: 8.0
        samples: 16
        color: "#BF9270"
        source: bodyRect
    }
}