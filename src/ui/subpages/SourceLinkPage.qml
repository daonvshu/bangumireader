import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1

import SourceLinkModel 0.1
import "../widgets"

Rectangle {

    color: "transparent"

    property int loadTargetBangumiId: -1
    property string title

    signal backPage

    SourceLinkModel{
        id: sourceLinkModel
        bangumiId: loadTargetBangumiId
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        Row {
            spacing: 4

            IconBtn {
                anchors.verticalCenter: parent.verticalCenter
                source: "../../resource/ic_previous.png"
                width: 20
                height: 20
        
                onClicked: backPage()
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter

                font.pixelSize: 16
                font.bold: true
                color: "#BF9270"
                text: title
            }
        }

        ButtonGroup {
            buttons: headerGroup.children
            onClicked: {
                sourceLinkModel.groupName = button.buttonText
            }
        }

        Flow {
            id: headerGroup

            Layout.fillWidth: true
            spacing: 6
    
            Repeater {
                model: sourceLinkModel.groupNames

                delegate: Button {
                    id: control
                    checkable: true
                    checked: index == 0

                    property alias buttonText: contentText.text

                    contentItem: Row {
                        spacing: 4

                        Text {
                            id: contentText
                            anchors.verticalCenter: parent.verticalCenter
    
                            text: modelData
                            font.pixelSize: 14
                            color: "#383838"
                        }

                        Rectangle {
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#BF9270"
                            width: 16
                            height: 16
                            radius: 8

                            Text {
                                anchors.centerIn: parent
                                text: sourceLinkModel.groupSize(modelData)
                                font.pixelSize: 10
                                color: "white"
                            }
                        }
                    }

                    background: Rectangle {
                        color: control.checked ? "#EDCDBB" : "#FFEDDB"
                        radius: 6
                    }
                }
            }
        }
    
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            color: "#E6DDC6"
            radius: 12

            ListView {
                id: bangumiLinkUrlList
                anchors.fill: parent
                anchors.leftMargin: 8
                anchors.topMargin: 8
                anchors.bottomMargin: 8

                model: sourceLinkModel
                clip: true

                populate: Transition {
                    NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 250 }
                }

                delegate: Rectangle {
                    width: bangumiLinkUrlList.width - 8
                    height: 32

                    color: "transparent"

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 4
                        anchors.rightMargin: 4

                        CheckBox {
                            id: checkbox

                            nextCheckState: function (args) {
                                rowChecked = !rowChecked
                            }

                            indicator: Rectangle {
                                implicitWidth: 18
                                implicitHeight: 18
                                x: checkbox.leftPadding
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
                                    visible: rowChecked
                                }
                            }
                        }

                        Text {
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter

                            text: display.title
                            font.pixelSize: 12
                            color: "#BF9270"
                            elide: Text.ElideMiddle
                        }

                        IconBtn {
                            Layout.alignment: Qt.AlignVCenter

                            width: 16
                            height: 16
                            source: "../../resource/ic_down.png"

                            onClicked: {
                                download = true
                            }
                        }
                    }
                }

                ScrollBar.vertical: ScrollBar { 
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                }
            }
        }

        Row {
            Layout.alignment: Qt.AlignRight
            spacing: 12

            IconBtn2 {
                text: "下载选中"
                enabled: !sourceLinkModel.downloading
                icon.source: "../../resource/ic_down.png"
                onClicked: sourceLinkModel.downloadSelectedRowLinks()
            }

            IconBtn2 {
                text: "订阅字幕组"
                icon.source: "../../resource/ic_rss.png"
            }
        }
    }
}