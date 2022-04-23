import QtQuick 2.15
import QtQuick.Controls 2.15

Button {
    id: control
    background: Rectangle {
        implicitWidth: 38
        implicitHeight: 38

        radius: 8
        color: control.checked ? "#E6DDC6" : "#EDCDBB"
    }

    contentItem: Text {
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        text: control.text
        color: "#C69B7B"
    }
}