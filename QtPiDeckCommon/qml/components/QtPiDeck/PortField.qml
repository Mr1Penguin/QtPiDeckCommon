import QtQuick 2.0
import QtQuick.Controls 2.15

Item {
    id: portField
    property alias port: textField.text
    signal changed(port: string)

    TextField {
        id: textField
        selectByMouse: true
        onEditingFinished: portField.changed(text)
        validator: IntValidator{ bottom: 1025; top: 65535 }
    }
}
