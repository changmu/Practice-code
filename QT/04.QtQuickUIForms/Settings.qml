import QtQuick 2.4
import my.customermodel.singleton 1.0

SettingsForm {
    id: form
    anchors.fill: parent

    function readData() {

        form.title.model = ["Mr.", "Ms."]

        CustomerModel.selection.forEach(function (rowIndex) {
            form.firstName.text = CustomerModel.get(rowIndex).firstName
            form.lastName.text = CustomerModel.get(rowIndex).lastName
            form.customerId.text = CustomerModel.get(rowIndex).customerId
            form.title.currentIndex = form.title.find(CustomerModel.get(rowIndex).title)
        })

        save.enabled = true
        cancel.enabled = true
        gridLayout1.enabled = true
    }

    function writeData() {
        CustomerModel.selection.forEach(function (rowIndex) {
            var notes = CustomerModel.get(rowIndex).notes
            CustomerModel.set(rowIndex, {
                                  firstName: form.firstName.text,
                                  lastName: form.lastName.text,
                                  customerId: form.customerId.text,
                                  title: form.title.currentText,
                                  notes: notes
                              })
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
