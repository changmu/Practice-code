import QtQuick 2.4
import my.customermodel.singleton 1.0

NotesForm {
    id: form

    function readData() {
        CustomerModel.selection.forEach(function (rowIndex) {
            form.textArea1.text = CustomerModel.get(rowIndex).notes
        })

        save.enabled = true
        cancel.enabled = true
        form.textArea1.enabled = true
    }

    function writeData() {
        CustomerModel.selection.forEach(function (rowIndex) {
            var data = CustomerModel.get(rowIndex)
            data.notes = form.textArea1.text
            CustomerModel.set(rowIndex, data)
        })
    }

    cancel.onClicked: readData()
    save.onClicked: writeData()

    Connections {
        target: CustomerModel.selection
        onSelectionChanged: form.readData()
    }

    Component.onCompleted: readData()
}
