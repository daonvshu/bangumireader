import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import BangumiListModel 0.1

import "../widgets"
import "../components"

Rectangle {

    color: "transparent"

    BangumiListModel{ id: bangumiListModel }

    signal clickedItemBangumi(string title, string link)

    ColumnLayout {
        anchors.fill: parent
        spacing: 12

        Row {
            spacing: 32

            Row {
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: "选择年份："
                    color: "#383838"
                }

                RoundComboBox {
                    id: yearComboBox
                    model: getYears()
                    currentIndex: model.length - 1

                    onActivated: {
                        bangumiListModel.year = 2013 + index
                    }
                }

            }

            ButtonGroup {
                id: seasonGroup
                buttons: seasons.children

                onClicked: {
                    bangumiListModel.season = button.text
                }
            }

            Row {
                id: seasons
                spacing: 6

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: "季度："
                    color: "#383838"
                }

                RoundButton {
                    checkable: true
                    checked: true
                    text: "春"
                }

                RoundButton {
                    checkable: true
                    text: "夏"
                }

                RoundButton {
                    checkable: true
                    text: "秋"
                }

                RoundButton {
                    checkable: true
                    text: "冬"
                }
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true

            color: "#FFEDDB"
            radius: 12

            BangumiListView {
                anchors.fill: parent
                anchors.margins: 12

                model: bangumiListModel

                onClickedBangumi: clickedItemBangumi(title, link)
            }
        }
    }

    Component.onCompleted: {
        bangumiListModel.year = 2013 + yearComboBox.currentIndex
        bangumiListModel.season = seasonGroup.checkedButton.text
    }

    function getYears() {
        var nowYear = (new Date()).getFullYear()
        var years = []
        for (var i = 2013; i <= nowYear; i++) {
            years.push(i + "年")
        }
        return years
    }
}