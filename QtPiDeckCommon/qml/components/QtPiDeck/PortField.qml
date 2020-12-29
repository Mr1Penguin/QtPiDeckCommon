import QtQuick 2.0
import QtQuick.Controls 2.15

TextField {
    id: portField
    selectByMouse: true
    onEditingFinished: portField.changed(text)
    validator: IntValidator{ bottom: 1025; top: 65535 }
}
