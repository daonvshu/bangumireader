import QtQuick 2.15
import QtQuick.Controls 2.15

Button {
    id: control
    height: 38

    background: Rectangle {
        id: rectBg

        implicitHeight: control.height

        opacity: enabled ? 1.0 : 0.3

        radius: 8
        color: control.down ? "#E6DDC6" : "#EDCDBB"

        Behavior on color {
            ColorAnimation {
                target: rectBg
                duration: 220
                easing.type: Easing.OutQuad
            }
        }
    }

    contentItem: Row {
        spacing: 4

        Image {
            anchors.verticalCenter: parent.verticalCenter
            sourceSize.width: control.height * 0.56
            source: control.icon.source

            opacity: enabled ? 1.0 : 0.3
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
    
            opacity: enabled ? 1.0 : 0.3
    
            text: control.text
            color: "#C69B7B"
        }
    }
}