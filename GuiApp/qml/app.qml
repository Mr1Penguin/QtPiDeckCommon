import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

import QtPiDeck 1.0

Window {
    width: 800
    height: 600
    visible: true
    title: qsTr("Hello World")

    Command {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.leftMargin: 5
        onClicked: {
            console.log("Hi")
        }
    }
}