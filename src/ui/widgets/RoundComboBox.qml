import QtQuick 2.15
import QtQuick.Controls 2.15

ComboBox {
    id: control

    indicator: Canvas {
        id: canvas
        x: control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 12
        height: 8
        contextType: "2d"

        Connections {
            target: control
            function onPressedChanged() { canvas.requestPaint(); }
        }

        onPaint: {
            context.reset();
            context.moveTo(0, 0);
            context.lineTo(width, 0);
            context.lineTo(width / 2, height);
            context.closePath();
            context.fillStyle = control.pressed ? "#FFBC80" : "#FF9F45";
            context.fill();
        }
    }

    background: Rectangle {
        implicitWidth: 120
        implicitHeight: 38

        radius: 8
        color: "#EDCDBB"
    }

    contentItem: Text {
        leftPadding: 12
        verticalAlignment: Text.AlignVCenter

        text: control.displayText
        color: "#C69B7B"
    }
}