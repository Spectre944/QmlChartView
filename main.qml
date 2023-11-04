import QtQuick
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects
import QtCharts 6.3
import Qt.labs.qmlmodels 1.0
import QtQuick.Dialogs

import "."

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    menuBar: MenuBar {
        Menu {
            title: qsTr("&Файл")
            Action { text: qsTr("Відкрити спектр")
                onTriggered: { fileDialog.open() }
            }
            Action { text: qsTr("Зберегти спектр") }
            Action { text: qsTr("Зберегти CSV") }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        onAccepted: {
            var filePath = fileDialog.selectedFile.toString().replace("file:///",'')
            console.log("You chose: " + filePath)
            //fileSelected(filePath)
            winSpectr.updateSpectrum(filePath)
        }
        onRejected: {
            console.log("Canceled")
        }
    }

    WinSpectr {
        id: winSpectr
        anchors.fill: parent
    }
}
