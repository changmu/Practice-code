import QtQuick 2.4
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.2

Item {
    id: content
    width: 400
    height: 400
    property alias textArea1: textArea1
    property alias cancel: cancel
    property alias save: save

    ColumnLayout {
        id: columnLayout1
        height: 100
        anchors.right: parent.right
        anchors.rightMargin: 12
        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.top: parent.top
        anchors.topMargin: 12

        TextArea {
            id: textArea1
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }

    RowLayout {
        id: rowLayout1
        width: 100
        anchors.right: parent.right
        anchors.rightMargin: 12
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 12

        Button {
            id: save
            text: qsTr("Save")
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        Button {
            id: cancel
            text: qsTr("Cancel")
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
