import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15
import Qt.labs.platform 1.1

import "components"
import "widgets"
import "subpages"
import "dialog"

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

            onRssClicked: {
                if (stackview.currentItem.objectName != "rssSubscribePage") {
                    stackview.push(rssSubscribePage)
                }
            }

            onSettingClicked: settingDialog.open()

            onCloseClicked: {
                mainWindow.hide()
                stackview.pop(null)
            }

            onDatabaseClicked: {
                if (stackview.currentItem.objectName != "bangumidatabasepage") {
                    stackview.push(bangumidatabasepage)
                }
            }
        }

        AnchorHLine {
            anchors.top: header.bottom
        }

        StackView {
            id: stackview
            initialItem: bangumipage
            clip: true

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

            BangumiListPage {
                onClickedItemBangumi: {
                    stackview.push(sourcelinkpage, {
                        "loadTargetBangumiId": parseInt(link.substring(link.lastIndexOf("/") + 1)),
                        "title": title
                    })
                }
            }
        }

        Component {
            id: sourcelinkpage

            SourceLinkPage {
                onBackPage: stackview.pop()
                objectName: "sourcelinkpage"
            }
        }

        Component {
            id: rssSubscribePage

            RssSubscribePage {
                onGotoSourceLinkPage: {
                    stackview.push(sourcelinkpage, {
                        "loadTargetBangumiId": id,
                        "title": title
                    })
                }

                onBackToHomePage: {
                    stackview.pop(null)
                }

                objectName: "rssSubscribePage"
            }
        }

        Component {
            id: bangumidatabasepage

            BangumiDatabasePage {
                onBackToHomePage: {
                    stackview.pop(null)
                }

                onBangumiClicked: {
                    console.log("bangumi clicked id:" + bangumiId)
                    stackview.push(bangumiDetailPage, {
                        "bangumiId": bangumiId
                    })
                }

                objectName: "bangumidatabasepage"
            }
        }

        Component {
            id: bangumiDetailPage
            
            BangumiDetailPage {
                onBackPage: stackview.pop()
            }
        }
    }

    DropShadow {
        anchors.fill: bodyRect
        radius: 8.0
        samples: 16
        color: "#BF9270"
        source: bodyRect
    }

    SettingDialog {
        id: settingDialog
    }

    PromptDialog {
        id: promptDialog

        onLinkClicked: {
            mainWindow.show()
            console.log("link clicked:" + id)
            if (stackview.currentItem.objectName != "sourcelinkpage") {
                stackview.push(sourcelinkpage, {
                    "loadTargetBangumiId": id,
                    "title": title
                })
            } else {
                stackview.currentItem.loadTargetBangumiId = id
                stackview.currentItem.title = title
            }
        }
    }

    Connections {
        target: rssReader

        function onNewRssItemFound(args) {
            promptDialog.links = args
            promptDialog.show()
        }
    }
}