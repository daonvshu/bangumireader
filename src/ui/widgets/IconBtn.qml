import QtQuick 2.15
import QtGraphicalEffects 1.12

Item {
    id: root

    width: 24
    height: 24

    property var colorState: ["transparent", "#60ffffff", "#30ffffff"]

    property color overlay: colorState[0]
    property alias source: iconBtn.source

    signal clicked

    Image {
        id: iconBtn
        sourceSize: Qt.size(parent.width, parent.height)
        smooth: true
    }

    Behavior on overlay {
        ColorAnimation {
            target: root
            properties: "overlay"
            duration: 220
            easing.type: Easing.OutQuad
        }
    }

    ColorOverlay {
        anchors.fill: iconBtn
        source: iconBtn
        color: parent.overlay
    }

    MouseArea {
        anchors.fill: iconBtn
        hoverEnabled: true

        onEntered: parent.overlay = colorState[1]
        onExited: parent.overlay = colorState[0]
        onPressed: parent.overlay = colorState[2]
        onReleased: parent.overlay = colorState[1]
        onClicked: parent.clicked()
    }
}