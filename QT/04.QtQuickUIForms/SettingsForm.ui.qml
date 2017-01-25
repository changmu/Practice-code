import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0

Item {
    id: content

    property alias customerId: customerId
    property alias lastName: lastName
    property alias firstName: firstName
    property alias gridLayout1: gridLayout1
    property alias rowLayout1: rowLayout1

    property alias save: save
    property alias cancel: cancel
    property alias title: title

    GridLayout {
        id: gridLayout1
        rows: 4
        columns: 3
        rowSpacing: 8
        columnSpacing: 8
        anchors.right: parent.right
        anchors.rightMargin: 12
        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.top: parent.top
        anchors.topMargin: 12

        Label {
            id: label1
            text: qsTr("Title")
        }

        Label {
            id: label2
            text: qsTr("First Name")
        }

        Label {
            id: label3
            text: qsTr("Last Name")
        }

        ComboBox {
            id: title
        }

        TextField {
            id: firstName
            text: ""
            Layout.fillHeight: true
            Layout.fillWidth: true
            placeholderText: qsTr("First Name")
        }

        TextField {
            id: lastName
            Layout.fillHeight: true
            Layout.fillWidth: true
            placeholderText: qsTr("Last Name")
        }

        Label {
            id: label4
            text: qsTr("Customer Id")
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        TextField {
            id: customerId
            width: 0
            height: 0
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.columnSpan: 3
            placeholderText: qsTr("Customer Id")
        }
    }

    RowLayout {
        id: rowLayout1
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
