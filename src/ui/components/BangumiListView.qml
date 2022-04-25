import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.0

import "../widgets"

ListView {
    id: bangumiListView
    clip: true
    spacing: 6

    cacheBuffer: 1000

    delegate: Loader {
        sourceComponent: type == 0 ? delegateHeader : delegateContent
        property var innerData: type == 0 ? header : contentData
    }

    signal clickedBangumi(string title, string link)

    Component {
        id: delegateHeader

        Item {
            implicitHeight: 28
            implicitWidth: bangumiListView.width

            Rectangle {
                id: rectHeader
                anchors.fill: parent

                radius: 10
                color: "#FF73402A"
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

                text: getWeekName(innerData)

                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 12
            }
        }
    }

    Component {
        id: delegateContent

        Item {
            implicitWidth: bangumiListView.width
            implicitHeight: (implicitWidth - 12 * 4) / 5 + 28

            ListView {
                anchors.fill: parent
                model: innerData
                orientation: ListView.Horizontal 
                spacing: 12

                delegate: Rectangle {
                    id: contentRoot

                    width: (bangumiListView.width - 12 * 4) / 5
                    height: width + 28

                    color: "transparent"

                    Column {
                        anchors.fill: parent
                        spacing: 4

                        Image {
                            id: thumbnailImg
                            width: contentRoot.width
                            height: width
    
                            fillMode: Image.PreserveAspectCrop
                            source: "https://mikanani.me" + modelData.thumbnail
                            sourceSize.width: width * 2
                            sourceSize.height: height * 2
                            smooth: true

                            opacity: modelData.link.length !== 0 ? 1.0 : 0.3
    
                            layer.enabled: true
                            layer.smooth: true
                            layer.effect: OpacityMask {
                                maskSource: Rectangle {
                                    width: thumbnailImg.width
                                    height: thumbnailImg.height
                                    radius: 12
                                }
                            }
                        }
    
                        Text {
                            width: contentRoot.width
                            horizontalAlignment: Text.AlignHCenter

                            text: modelData.title
                            color: "#BF9270"
                            font.pixelSize: 14
                            font.bold: true
                            elide: Text.ElideMiddle
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var linkStr = modelData.link
                            if (linkStr.length !== 0) {
                                clickedBangumi(modelData.title, modelData.link)
                            }
                        }
                    }
                }
            }
        }
    }

    function getWeekName(week) {
        if (week < 7) {
            return "星期" + "日一二三四五六".substr(week, 1)
        }
        if (week == 7) {
            return "剧场版"
        }
        return "OVA"
    }
}