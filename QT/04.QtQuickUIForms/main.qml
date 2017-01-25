import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import my.customermodel.singleton 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Qt Quick UI Forms")

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
        Menu {
            title: qsTr("&Edit")
            MenuItem {
                action: cutAction
            }
            MenuItem {
                action: copyAction
            }
            MenuItem {
                action: pasteAction
            }
        }
        Menu {
            title: qsTr("&Help")
            MenuItem {
                text: qsTr("About...")
                onTriggered: aboutDialog.open()
            }
        }
    }

    Action {
        id: copyAction
        text: qsTr("&Copy")
        shortcut: StandardKey.Copy
        iconName: "edit-copy"
        enabled: (!!activeFocusItem && !!activeFocusItem["copy"])
        onTriggered: activeFocusItem.copy()
    }

    Action {
        id: cutAction
        text: qsTr("Cu&t")
        shortcut: StandardKey.Cut
        iconName: "edit-cut"
        enabled: (!!activeFocusItem && !!activeFocusItem["cut"])
        onTriggered: activeFocusItem.cut()
    }

    Action {
        id: pasteAction
        text: qsTr("&Paste")
        shortcut: StandardKey.Paste
        iconName: "edit-paste"
        enabled: (!!activeFocusItem && !!activeFocusItem["paste"])
        onTriggered: activeFocusItem.paste()
    }

    MainForm {
        anchors.fill: parent
        Layout.minimumWidth: 800
        Layout.minimumHeight: 480
        Layout.preferredWidth: 768
        Layout.preferredHeight: 480
        tableView1.model: CustomerModel

        Component.onCompleted: CustomerModel.selection = tableView1.selection
    }

    MessageDialog {
        id: aboutDialog
        icon: StandardIcon.Information
        title: qsTr("About")
        text: "Qt Quick UI Forms"
        informativeText: qsTr("This example demonstrates how to separate the "
                              + "implementation of an application from the UI "
                              + "using ui.qml files.")
    }
}
