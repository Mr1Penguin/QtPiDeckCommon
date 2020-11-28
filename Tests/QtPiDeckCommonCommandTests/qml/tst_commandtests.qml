import QtQuick 2.0
import QtTest 1.0
import QtPiDeck 1.0
import QtPiDeck.Tests 1.0

Command {
    id: command
    TestCase {
        name: "CommandTests"

        TestHelper {
            id: testHelper
        }

        function test_defaultState() {
            var textChild = testHelper.findFirstChildByTypeName(command, "Text");
            verify(textChild !== null)
            verify(textChild.visible);
        }
    }
}
