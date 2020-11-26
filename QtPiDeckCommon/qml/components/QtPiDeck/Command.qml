import QtQuick 2.0

Rectangle {
    signal onClicked()

    width: 96 //to be changed during client testing
    height: width
    border.color: "black" // to be changed with theme
    border.width: 1
    radius: 10
    Image { // to be tested on client side
        id: image
        source: ""
        anchors.fill: parent
        //horizontalAlignment: horizontalCenter
        //verticalAlignment: verticalAlignment
    }
    Text {
        text: qsTr("What is this very very long text it can't even be put in one line")
        visible: image.source == ""
        width: parent.width
        wrapMode: Text.Wrap
        horizontalAlignment: Text.AlignHCenter
        anchors.centerIn: parent
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: parent.onClicked()
    }

    // not finished
    Image {
        id: commandStatus
        visible: false
        source: ""
        state: "hidden"
        opacity: 0
        anchors.fill: parent
        //horizontalAlignment: horizontalCenter
        //verticalAlignment: verticalAlignment

        states: [
            State {
                name: "success"
                PropertyChanges {
                    target: commandStatus
                    visible: true
                }
                PropertyChanges {
                    target: commandStatus
                    opacity: 1
                }
                PropertyChanges {
                    target: commandStatus
                    source: "" //ok image
                }
            },
            State {
                name: "failure"
                PropertyChanges {
                    target: commandStatus
                    visible: true
                }
                PropertyChanges {
                    target: commandStatus
                    opacity: 1
                }
                PropertyChanges {
                    target: commandStatus
                    source: "" //fail image
                }
            },
            State {
                name: "hiding"
            },
            State {
                name: "hidden"
                PropertyChanges {
                    target: commandStatus
                    source: "" //fail image
                }
            }
        ]

        transitions: [
            Transition {
                from: ""
                to: "success,failure"
                reversible: true
                SequentialAnimation {
                    id: anim
                    NumberAnimation {
                        target: commandStatus
                        property: "opacity"
                        from: 0;
                        to: 1;
                        duration: 100
                    }
                }
            }
        ]
    }

    function showOk() {commandStatus.state = "success"}
    function showFailed() {commandStatus.state = "failure"}
}
