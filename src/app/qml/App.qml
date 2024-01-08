// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Arrival

Item {
    anchors.fill: parent

    AppModel {
        id: appModel

        onParsingStarted: () => {
            loadingPopup.open();
        }

        onParsingCompleted: () => {
            loadingPopup.close();
            columnSelector.open();
        }
    }

    Popup {
        anchors.centerIn: parent
        id: loadingPopup
        modal: true
        width: 300
        height: 200
        closePolicy: Popup.NoAutoClose
        visible: false

        background: Rectangle {
            color: Style.backgroundColor
            radius: 6
        }

        Overlay.modal: Rectangle {
            color: "#BF070809"
        }

        enter: Transition {
            ParallelAnimation {
                NumberAnimation {
                    property: "opacity"
                    from: 0.0
                    to: 1.0
                    duration: 300
                }
                NumberAnimation {
                    property: "scale"
                    from: 0.4
                    to: 1.0
                    duration: 300
                }
            }
        }

        exit: Transition {
            ParallelAnimation {
                NumberAnimation {
                    property: "opacity"
                    from: 1.0
                    to: 0.0
                    duration: 300
                }
                NumberAnimation {
                    property: "scale"
                    from: 1.0
                    to: 0.8
                    duration: 300
                }
            }
        }

        ArrivalBusyIndicator {
            id: busyIndicator
            anchors.centerIn: parent
            width: 100
            height: 100
            running: false
        }

        onOpened: () => {
            busyIndicator.running = true;
        }

        onClosed: () => {
            busyIndicator.running = false;
        }
    }

    ArrivalHeaderSelector {
        id: columnSelector
        anchors.centerIn: parent
        width: parent.width - 200
        height: parent.height - 200
        table: appModel.jobTable
        selectedHeadersTemplateList: appModel.templateList
    }

    FileDialog {
        id: xlsxExportDialog
        nameFilters: ["XLSX (.xlsx)"]
        fileMode: FileDialog.SaveFile
        defaultSuffix: "xlsx"
        title: qsTr("Select file")
        onAccepted: () => {
            appModel.xlsxExport(xlsxExportDialog.selectedFile, jobList.columnsToShow);
        }
    }

    ColumnLayout {
        anchors.margins: 20
        anchors.fill: parent
        spacing: 20

        RowLayout {
            Layout.fillWidth: true
            spacing: 20

            InfoLabelArea {
                text: "Total " + (appModel.jobTable.hasData ? appModel.jobTable.rowCount : "-")
            }

            InfoLabelArea {
                color: Style.remainedEntryBackgroundColor
                border.width: 0
                text: "Remained  " + (appModel.jobTable.hasData ? (appModel.jobTable.rowCount - (appModel.jobTable.newAddedCount + appModel.jobTable.removedCount)) : "-")
            }

            InfoLabelArea {
                color: Style.newAddedEntryBackgroundColor
                border.width: 0
                text: "New  " + (appModel.jobTable.hasData ? appModel.jobTable.newAddedCount : "-")
            }

            InfoLabelArea {
                color: Style.removedEntryBackgroundColor
                border.width: 0
                text: "Removed  " + (appModel.jobTable.hasData ? appModel.jobTable.removedCount : "-")
            }

            Item {
                Layout.fillWidth: true
            }

            ArrivalButton {
                buttonText: "Select"
                onClicked: () => {
                    columnSelector.open();
                }
            }
        }

        ArrivalJobTable {
            id: jobList
            Layout.fillWidth: true
            Layout.fillHeight: true
            jobTable: appModel.jobTable
            columnsToShow: columnSelector.selectedHeaderIndices
        }

        ArrivalButton {
            id: exportButton
            buttonText: "Export Excel"
            Layout.alignment: Qt.AlignBottom | Qt.AlignRight
            onClicked: () => {
                if (appModel.jobTable.rowCount > 0 && jobList.columnsToShow.length > 0) {
                    xlsxExportDialog.currentFile = "Arrival-" + Qt.formatDateTime(new Date(), "dd_MM_yyyy") + "-" + Qt.formatTime(new Date(),"hh_mm") + ".xlsx";
                    xlsxExportDialog.open();
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            Layout.fillWidth: true
            spacing: 20

            FileSelectionArea {
                id: leftFileSelectionArea
                height: 120
                Layout.fillWidth: true

                multiple: false
                enableSelectDialog: true
                fileExtensionFilters: ["CSV (*.csv)"]
                helpText: qsTr("Drop the old CSV file here or click")
            }

            FileSelectionArea {
                id: rightFileSelectionArea
                height: 120
                Layout.fillWidth: true

                multiple: false
                enableSelectDialog: true
                fileExtensionFilters: ["CSV (*.csv)"]
                helpText: qsTr("Drop the new CSV file here or click")
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignRight | Qt.AlignBottom
            spacing: 12

            ArrivalButton {
                Layout.alignment: Qt.AlignBottom | Qt.AlignRight
                buttonText: "Clear"
                buttonColor: "#da373c"
                buttonHoverColor: "#a12828"
                onClicked: () => {
                    leftFileSelectionArea.deselectFiles();
                    rightFileSelectionArea.deselectFiles();
                    appModel.jobTable.clearTable();
                    columnSelector.resetTemplateSelection();
                }
            }

            ArrivalButton {
                Layout.alignment: Qt.AlignBottom | Qt.AlignRight
                buttonText: "Import CSV"
                onClicked: () => {
                    if (leftFileSelectionArea.fileUrls.length > 0 && rightFileSelectionArea.fileUrls.length > 0) {
                        appModel.parseCSV(leftFileSelectionArea.fileUrls[0], rightFileSelectionArea.fileUrls[0]);
                    }
                }
            }
        }
    }
}
