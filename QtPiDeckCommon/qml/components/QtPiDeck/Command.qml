import QtQuick 2.0
import QtPiDeck 1.0

Rectangle {
    signal clicked

    property CommandViewModel viewModel

    width: dp(128)
    height: width
    border.color: "black" // to be changed with theme
    border.width: dp(1)
    radius: 10
    Image {
        id: image
        source: viewModel.imagePath
        anchors.fill: parent
    }

    Text {
        text: viewModel.text
        visible: image.source == ""
        width: parent.width
        wrapMode: Text.Wrap
        font.pixelSize: sp(14)
        horizontalAlignment: Text.AlignHCenter
        anchors.centerIn: parent
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: parent.clicked()
    }

    Rectangle {
        id: status
        radius: 10
        anchors.centerIn: parent
        width: parent.width * 0.9
        height: width
        visible: false
        color: "transparent"
    }

    PropertyAnimation {
        id: statusOk
        target: status
        properties: "color"
        to: "green"
        duration: 250
        onStopped: {
            status.visible = false
            status.color = "transparent"
        }
    }

    PropertyAnimation {
        id: statusFailed
        target: status
        properties: "color"
        to: "red"
        duration: 250
        onStopped: {
            status.visible = false
            status.color = "transparent"
        }
    }

    function showOk() { 
        status.visible = true;
        statusOk.start();
    }
    function showFailed() { 
        status.visible = true;
        statusFailed.start();
    }
}
