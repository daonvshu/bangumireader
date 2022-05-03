import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.0

import BangumiDatabaseListModel 0.1
import "../widgets"

Rectangle {
    id: root
    color: "transparent"

    signal backToHomePage

    MouseArea {
        anchors.fill: parent
        onClicked: focus = true
    }

    BangumiDatabaseListModel{
        id: databaseModel

        onSyncMessageChanged: {
            popup.messages = message
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        RowLayout {
            spacing: 4

            IconBtn {
                source: "../../resource/ic_previous.png"
                width: 20
                height: 20
        
                onClicked: backToHomePage()
            }

            Text {
                font.pixelSize: 16
                font.bold: true
                color: "#BF9270"
                text: "历年番剧数据"
            }

            Item {
                Layout.fillWidth: true
            }

            IconBtn2 {
                height: 32
                text: "同步"
                icon.source: "../../resource/ic_cloud_sync.png"

                onClicked: databaseModel.syncData()
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            color: "#E6DDC6"
            radius: 12

            ListView {
                id: bangumiListView
                anchors.fill: parent
                anchors.leftMargin: 8
                anchors.topMargin: 8
                anchors.bottomMargin: 8

                model: databaseModel
                clip: true
                spacing: 4

                populate: Transition {
                    NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 250 }
                }

                delegate: Loader {
                    sourceComponent: display.yearRow ? yearRow : (display.monthRow ? monthRow : dataRow)
                    property var innerData: display
                }

                ScrollBar.vertical: ScrollBar { 
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                }
            }
        }

        RowLayout {
            spacing: 12

            Text {
                id: contentText
                Layout.alignment: Qt.AlignVCenter

                text: "搜索："
                font.pixelSize: 14
                color: "#383838"
            }

            TextField {
                verticalAlignment: Qt.AlignVCenter
                clip: true
                
                text: databaseModel.filterKeywords
                font.pixelSize: 14
                onTextChanged: databaseModel.filterKeywords = text

                background: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 36
                    color: "#EDCDBB"
                    radius: 8
                }
            }

            Item {
                Layout.fillWidth: true
            }

            ButtonGroup {
                buttons: typeFilterRow.children
                onClicked: {
                    if (checkedButton != null && databaseModel.filterType === checkedButton.text) {
                        checkedButton.checked = false
                        checkedButton = null
                        databaseModel.filterType = ""
                    } else {
                        databaseModel.filterType = button.text
                    }
                }
            }

            Row {
                id: typeFilterRow
                spacing: 12

                Repeater {
                    model: ["tv", "ova", "movie", "web"]
                    delegate: Button {
                        id: filterBtn

                        text: modelData
                        checkable: true
                        contentItem: Text {
                            color: "#827962"
                            font.bold: true
                            font.pixelSize: 11
            
                            text: filterBtn.text
                        }
    
                        background: Rectangle {
                            radius: 6
                            color: getTypeColor(modelData)

                            border.width: 2
                            border.color: checked ? "#FF8243" : "transparent"
                        }
                    }
                }
            }
        }
    }

    Component {
        id: yearRow

        Item {
            implicitHeight: 28
            implicitWidth: bangumiListView.width - 8

            Rectangle {
                id: rectHeader
                anchors.fill: parent

                radius: 10
                color: "#73402A"
            }

            OpacityMask {
                anchors.fill: rectHeader
                maskSource: rectHeader
                source: GradientBg{
                    width: rectHeader.width
                    height: rectHeader.height
                }
            }

            Text {
                color: "white"
                font.bold: true
                font.pixelSize: 12

                text: innerData.data[0]

                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 12
            }
        }
    }

    Component {
        id: monthRow

        Rectangle {
            height: 28
            implicitWidth: bangumiListView.width - 8

            color: "transparent"

            Text {
                color: "#BF9270"
                font.bold: true
                font.pixelSize: 12

                text: innerData.data[0]

                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    Component {
        id: dataRow

        Flow {
            width: bangumiListView.width - 8
            spacing: 6

            Repeater {
                model: innerData.data

                delegate: Button {

                    hoverEnabled: true
                    
                    contentItem: Row {
                        spacing: 4
                        Text {
                            color: "#827962"
                            font.bold: true
                            font.pixelSize: 11
                            font.underline: hovered
            
                            text: modelData.title
                        }

                        Image {
                            visible: modelData.star == 1
                            anchors.verticalCenter: parent.verticalCenter
                            sourceSize.width: 12
                            source: "../../resource/ic_star.png"
                        }
                    }

                    background: Rectangle {
                        radius: 6
                        color: getTypeColor(modelData.type, hovered)
                        Behavior on color {
                            ColorAnimation {
                                duration: 150
                            }
                        }
                    }
                }
            }
        }
    }

    Popup {
        id: popup
        width: 300
        height: 200
        x: (root.width - width) / 2
        y: (root.height - height) / 2
        modal: true
        focus: true
        visible: databaseModel.syncStatus
        closePolicy: Popup.NoAutoClose

        property var messages: []

        contentItem: Rectangle {
            anchors.fill: parent
            color: "transparent"

            Column {
                anchors.fill: parent
                anchors.margins: 12
    
                Repeater {
                    model: popup.messages
    
                    delegate: Text {
                        text: modelData
                        font.pixelSize: 14
                    }
                }
            }

            IconBtn2 {
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: 12
                icon.source: "../../resource/ic_remove.png"
                text: "关闭"

                onClicked: databaseModel.syncStatus = false
            }
        }

        background: Rectangle {
            radius: 6
            color: "#FFEDDB"
        }
    }

    function getTypeColor(type, lighter = false, darker = false) {
        var target = "#4f1D364D"
        if (type === "tv") {
            target = "#4fFF7B54"
        } else if (type === "ova") {
            target = "#4f14ffec"
        } else if (type === "movie") {
            target = "#4fFE4CBD"
        }
        if (lighter) {
            target = Qt.lighter(target, 1.3)
        } else if (darker) {
            target = Qt.darker(target, 1.3)
        }
        return target
    }
}