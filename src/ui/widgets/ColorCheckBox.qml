import QtQuick 2.15
import QtQuick.Controls 2.15

CheckBox {
    id: control

    contentItem: Text {
        leftPadding: control.indicator.width + (control.text.length === 0 ? 0 : control.spacing)
        text: control.text
        font.pixelSize: 16
        color: "#BF9270"
    }

    indicator: Rectangle {
        implicitWidth: 18
        implicitHeight: 18
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: 3
        border.color: "#BF9270"

        Rectangle {
            width: 12
            height: 12
            x: 3
            y: 3
            radius: 2
            color: "#BF9270"
            visible: control.checked
        }
    }
}