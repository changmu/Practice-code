import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

TableView {
    id: tableView

    property int columnWidth: width / 3 - 1
    Layout.minimumWidth: splitView1.width * 2 / 5

    TableViewColumn {
        role: "customerId"
        title: qsTr("Customer Id")
        width: tableView.columnWidth
    }

    TableViewColumn {
        role: "firstName"
        title: qsTr("First Name")
        width: tableView.columnWidth
    }

    TableViewColumn {
        role: "lastName"
        title: qsTr("Last Name")
        width: tableView.columnWidth
    }
}
