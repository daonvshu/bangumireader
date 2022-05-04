import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.12

import BangumiDetailModel 0.1
import "../widgets"

Rectangle {
    id: root
    color: "transparent"

    property var bangumiId

    signal backPage

    BangumiDetailModel{
        id: detailModel
        bangumiId: root.bangumiId
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        RowLayout {
            spacing: 4

            IconBtn {
                source: "qrc:/resource/ic_previous.png"
                width: 20
                height: 20
        
                onClicked: backPage()
            }

            Text {
                font.pixelSize: 16
                font.bold: true
                color: "#BF9270"
                text: detailModel.database.title
            }

            Rectangle {
                width: 16
                height: 16

                color: "transparent"

                Image {
                    id: btnStar
                    anchors.centerIn: parent

                    sourceSize.width: 12
                    source: "qrc:/resource/ic_star.png"
    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: detailModel.star = !detailModel.star
                    }
                }

                ColorOverlay {
                    anchors.fill: btnStar
                    source: btnStar
                    color: "#E6DDC6"
            
                    visible: !detailModel.star
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            color: "#E6DDC6"
            radius: 12

            RowLayout {
                anchors.fill: parent
                anchors.margins: 12

                Flickable {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignTop
                    contentHeight: descriptionContent.height
                    clip: true

                    ScrollBar.vertical: ScrollBar { }

                    GridLayout {
                        id: descriptionContent
                        anchors.left: parent.left
                        anchors.right: parent.right

                        columns: 2
                        columnSpacing: 4
                        rowSpacing: 6

                        Text {
                            font.pixelSize: 12
                            color: "#BF9270"
                            text: "简介："
                            Layout.alignment: Qt.AlignTop
                        }

                        Text {
                            Layout.fillWidth: true
                            font.pixelSize: 12
                            color: "#BF9270"
                            text: detailModel.detail.summary
                            wrapMode: Text.WrapAnywhere
                        }

                        Text {
                            font.pixelSize: 12
                            color: "#BF9270"
                            text: "放送平台："
                        }

                        Row {
                            spacing: 4
                            Repeater {
                                model: getPlaySites(detailModel.database.sites)

                                delegate: Button {
                                    hoverEnabled: true

                                    contentItem: Text {
                                        color: "#827962"
                                        font.bold: true
                                        font.pixelSize: 11
                                        font.underline: hovered
                        
                                        text: modelData.tag
                                    }
                
                                    background: Rectangle {
                                        radius: 6
                                        color: hovered ? Qt.lighter("#4fFF7B54", 1.3) : "#4fFF7B54"
                                        Behavior on color {
                                            ColorAnimation {
                                                duration: 150
                                            }
                                        }
                                    }

                                    onClicked: Qt.openUrlExternally(modelData.link)
                                }
                            }
                        }

                        Text {
                            font.pixelSize: 12
                            color: "#BF9270"
                            text: "官方网站："
                        }

                        Text {
                            font.pixelSize: 11
                            text: "<a href=\"" + detailModel.database.officialSite + "\">" + detailModel.database.officialSite + "</a>"

                            onLinkActivated: Qt.openUrlExternally(detailModel.database.officialSite)
                        }
                    }
                }

                Column {
                    Layout.minimumWidth: 220
                    Layout.fillHeight: true
                    spacing: 4

                    Image {
                        sourceSize.width: 220
                        source: detailModel.detail.image
                    }

                    Text {
                        font.pixelSize: 12
                        color: "#BF9270"
                        text: "集数：" + detailModel.detail.eps
                    }

                    Text {
                        font.pixelSize: 12
                        color: "#BF9270"
                        text: "放送：" + detailModel.detail.airDate + "（星期" + "一二三四五六日"[detailModel.detail.airWeekday-1] + "）"
                    }
                }
            }
        }
    }

    function getPlaySites(urlStr) {
        var sites = urlStr.split(",")
        if (sites.length === 0) {
            return []
        }

        sites.pop()
        var sitesMap = []
        for (var i = 0; i < sites.length / 2; i++) {
            sitesMap.push({"tag": sites[i*2], "link": sites[i*2+1]})
        }
        return sitesMap
    }
}