import QtQuick 2.15

import QtQuick.Shapes 1.15

Shape {
    id: root

    ShapePath {
        fillGradient: LinearGradient{
            x1: 0; y1: 0
            x2: 5; y2: 5
            spread: ShapeGradient.RepeatSpread
    
            GradientStop { position: 0.0; color: "#FF73402A"}
            GradientStop { position: 0.5; color: "#FF73402A"}
            GradientStop { position: 0.50002; color: "#FF7A4C39"}
            GradientStop { position: 1.0; color: "#FF7A4C39"}
        }

        startX: 0; startY: 0
        PathLine {x: root.width; y: 0}
        PathLine {x: root.width; y: root.height}
        PathLine {x: 0; y: root.height}
    }
}