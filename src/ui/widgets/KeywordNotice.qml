import QtQuick 2.15
import QtQuick.Controls 2.15

Image {
    id: noticeIcon
    source: "qrc:/resource/ic_notice.png"

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onEntered: noticePopup.open()
        onExited: noticePopup.close()
    }

    Popup {
        id: noticePopup
        width: 260
        height: 48
        x: noticeIcon.width + 6
        y: -noticeIcon.height / 2 - 6
        closePolicy: Popup.CloseOnPressOutside

        background: Rectangle {
            color: "#EDCDBB"
            border.width: 1
            border.color: "#E3B7A0"

            radius: 6
        }

        contentItem: Rectangle {
            color: "transparent"

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "使用'&'和'|'组合关键字，允许使用括号运算，\n如：A|(B&C)，表示：A存在或B、C同时存在"
                font.pixelSize: 12
                color: "#383838"
            }
        }
    }
}