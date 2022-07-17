import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import SourceLinkModel 0.1
import "../widgets"
import "../dialog"

Rectangle {

    id: root
    color: "transparent"

    property int loadTargetBangumiId: -1
    property string title

    signal backPage

    MouseArea {
        anchors.fill: parent
        onClicked: focus = true
    }

    SourceLinkModel{
        id: sourceLinkModel
        bangumiId: loadTargetBangumiId
        bangumiTitle: title

        property int groupNamesUpdateChangeSize: 0

        onGroupNamesChanged: groupNamesUpdateChangeSize++
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        Row {
            spacing: 4

            IconBtn {
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/resource/ic_previous.png"
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

                        Image {
                            visible: sourceLinkModel.groupNamesUpdateChangeSize, sourceLinkModel.isGroupSubscribed(modelData)
                            anchors.verticalCenter: parent.verticalCenter
                            sourceSize.width: 12
                            source: "qrc:/resource/ic_star.png"
                        }

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

                property var currentSubscribed: sourceLinkModel.groupSubscribed

                populate: Transition {
                    NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 250 }
                }

                delegate: Rectangle {
                    id: itemRow
                    width: bangumiLinkUrlList.width - 8
                    height: 32
                    radius: 4

                    property var isHoverd: false

                    color: isHoverd ? "#FFBC80" : "#E6DDC6"
                    Behavior on color {
                        ColorAnimation {
                            duration: 200
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: isHoverd = true
                        onExited: isHoverd = false
                        onClicked: newStatus = false
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 4
                        anchors.rightMargin: 4

                        ColorCheckBox {
                            checked: rowChecked
                            onClicked: rowChecked = checked
                        }

                        Text {
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter

                            text: display.title
                            font.pixelSize: 12
                            color: "#BF9270"
                            elide: Text.ElideMiddle
                        }

                        Image {
                            Layout.alignment: Qt.AlignVCenter

                            source: "qrc:/resource/ic_new.png"
                            visible: bangumiLinkUrlList.currentSubscribed && newStatus
                        }

                        IconBtn {
                            Layout.alignment: Qt.AlignVCenter

                            width: 16
                            height: 16
                            source: "qrc:/resource/ic_down.png"
                            opacity: downloaded ? 0.3 : 1

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

        RowLayout {
            spacing: 12

            IconBtn2 {
                text: "全选"
                enabled: !sourceLinkModel.downloading
                icon.source: "qrc:/resource/ic_tick.png"
                onClicked: sourceLinkModel.selectAllItems()
            }

            Item {
                width: 8
            }

            Text {
                id: contentText
                Layout.alignment: Qt.AlignVCenter

                text: "筛选："
                font.pixelSize: 14
                color: "#383838"
            }

            TextField {
                verticalAlignment: Qt.AlignVCenter
                clip: true
                
                text: sourceLinkModel.filterKeywords
                font.pixelSize: 14
                onTextChanged: sourceLinkModel.filterKeywords = text

                background: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 36
                    color: "#EDCDBB"
                    radius: 8
                }
            }

            KeywordNotice {
                verticalAlignment: Qt.AlignVCenter
            }

            Item {
                Layout.fillWidth: true
            }

            IconBtn2 {
                text: "下载选中"
                enabled: !sourceLinkModel.downloading
                icon.source: "qrc:/resource/ic_down.png"
                onClicked: sourceLinkModel.downloadSelectedRowLinks()
            }

            IconBtn2 {
                text: "订阅字幕组"
                icon.source: sourceLinkModel.groupSubscribed ? "qrc:/resource/ic_tick.png" : "qrc:/resource/ic_rss.png"
                onClicked: {
                    subscribeDialog.subscribeData = sourceLinkModel.getCurrentSubscribeGroup()
                    subscribeDialog.bangumiTitle = title
                    subscribeDialog.open()
                }
            }
        }
    }

    SubscribeDialog {
        id: subscribeDialog

        onSaved: sourceLinkModel.saveSubscribe(root.title, keywords)

        onCancelSubscribe: sourceLinkModel.removeSubscribe(subscribeDialog.subscribeData)
    }
}