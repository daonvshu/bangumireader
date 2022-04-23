import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

import "components"
import "widgets"
import "subpages"

Item {
    width: 1280
    height: 768
    visible: true
    
    Rectangle {
        id: bodyRect
        anchors.fill: parent
        anchors.margins: 6

        radius: 9
        color: "#FEF7DC"

        AppHeader {
            id: header 

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 6
        }

        AnchorHLine {
            anchors.top: header.bottom
        }

        StackView {
            id: stackview
            initialItem: bangumipage

            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                top: header.bottom
                margins: 16
            }
        }

        Component {
            id: bangumipage

            BangumiListPage {}
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