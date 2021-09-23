import QtQuick 2.0
import QtTest 1.0
import QtPiDeck 1.0
import QtPiDeck.Tests 1.0

Command {
    viewModel: CommandViewModel {
        text: "Hello me"
        //imagePath: "/icons/maintheme/64x64/close.png"
    }

    function dp(value) {
        return qh ? qh.dp(value) : value;
    }

    function sp(value) {
        return qh ? qh.sp(value) : value;
    }

    id: command
    TestCase {
        name: "CommandTests"

        QmlTreeExplorer {
            id: qmlTreeExplorer
        }

        function test_defaultState() {
            var textChild = qmlTreeExplorer.findFirstChildByTypeName(command, "Text");
            verify(textChild !== null)
            verify(textChild.visible);
        }
    }
}
