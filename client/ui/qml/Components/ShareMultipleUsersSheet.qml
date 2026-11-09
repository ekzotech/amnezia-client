import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import QtCore

import Style 1.0

import "../Controls2"

SheetType {
    id: root

    property string serverId
    property int containerIndex
    property bool nativeFormat
    property int generatedCount: 0

    signal usersCreated()

    expandedHeight: content.implicitHeight + 16 + PageController.safeAreaBottomMargin

    function openFor(serverId, containerIndex, nativeFormat) {
        root.serverId = serverId
        root.containerIndex = containerIndex
        root.nativeFormat = nativeFormat
        root.generatedCount = 0
        names.textAreaText = ""
        root.open()
    }

    Connections {
        target: ExportController

        function onConfigsGenerated(count) {
            root.generatedCount = count
            if (count > 0) {
                folderDialog.open()
            } else {
                PageController.showNotificationMessage(qsTranslate("PageShare", "No users created"))
            }
        }
    }

    FolderDialog {
        id: folderDialog

        title: qsTranslate("PageShare", "Save configs to")

        currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]

        onAccepted: {
            if (!ExportController.saveConfigsToDirectory(selectedFolder)) {
                PageController.showErrorMessage(qsTranslate("PageShare", "Failed to save the configs"))
                return
            }

            PageController.showNotificationMessage(qsTranslate("PageShare", "%n config file(s) saved", "",
                                                               Number(root.generatedCount)))
            root.usersCreated()
            root.close()
        }

        onRejected: PageController.showNotificationMessage(qsTranslate("PageShare", "The configs were created but not saved"))
    }

    ColumnLayout {
        id: content

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 16
        anchors.leftMargin: 16
        anchors.rightMargin: 16

        spacing: 0

        SectionHeaderType {
            Layout.fillWidth: true
            Layout.bottomMargin: 24

            headerText: qsTranslate("PageShare", "Create users")
            descriptionText: qsTranslate("PageShare", "Enter up to 20 user names, one per line or separated by commas. A connection config is created for each user.")
        }

        TextAreaType {
            id: names

            Layout.fillWidth: true

            placeholderText: qsTranslate("PageShare", "User names")
        }

        ButtonType {
            Layout.fillWidth: true
            Layout.topMargin: 24

            text: qsTranslate("PageShare", "Create and save")
            leftImageSource: "qrc:/images/controls/save.svg"

            enabled: names.textAreaText.trim() !== ""

            onClicked: {
                PageController.showBusyIndicator(true)
                ExportController.generateConfigs(root.serverId, root.containerIndex, root.nativeFormat, names.textAreaText)
                PageController.showBusyIndicator(false)
                if (root.generatedCount > 0) {
                    names.textAreaText = ""
                }
            }
        }
    }
}
