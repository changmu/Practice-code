import QtQuick 2.4
import my.customermodel.singleton 1.0

HistoryTableView {

    function readData() {
        CustomerModel.selection.forEach(function (rowIndex) {

            var history = CustomerModel.get(rowIndex).history
            var entries = history.split("~")

            model.clear()

            var index
            for (index = 0; index < entries.length; index++) {
                var entry = entries[index]
                var data = entry.split("|")
                model.append({
                                 date: data[0],
                                 type: data[1],
                                 text: data[2]
                             })
            }
        })
    }

    Connections {
        target: CustomerModel.selection
        onSelectionChanged: readData()
    }

    Component.onCompleted: readData()
}
