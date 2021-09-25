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

    function dp(value) {
        return qh ? qh.dp(value) : value;
    }

    function sp(value) {
        return qh ? qh.sp(value) : value;
    }

    Command {
        id: cmd
        property int c: 0

        viewModel: CommandViewModel {
            text: "Hello me " + cmd.c + " time" + (cmd.c == 1 ? "" : "s")
            imagePath: cmd.c % 10 == 0 ? "/icons/maintheme/64x64/close.png" : ""
        }
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.leftMargin: 5
        onClicked: {
            c++;
            if (c % 10 == 5) {
                cmd.showOk();
            }

            if (c % 10 == 8) {
                cmd.showFailed();
            }
        }
    }
}
