import QtQuick 2.15
import QtQuick.Controls 2.15

import "dialog"

Item {
    PromptDialog {
        id: promptDialog
        links: dataLinks
        visible: true

        onLinkClicked: {
            mainView.loadTarget(id, title);
        }
    }
}