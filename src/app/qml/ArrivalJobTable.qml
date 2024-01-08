// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Arrival

Item {
    id: jobListComponent

    required property var jobTable
    property list<int> columnsToShow

    // Styling
    property int minCellWidth: 400
    property color backgroundColor: Style.controlBackgroundColor
    property color newAddedEntryBackgroundColor: Style.newAddedEntryBackgroundColor
    property color removedEntryBackgroundColor: Style.removedEntryBackgroundColor
    property color remainedEntryBackgroundColor: Style.remainedEntryBackgroundColor
    property color textColor: Style.textColor
    property color headerTextColor: Style.textColor
    property color borderColor: Style.controlBorderColor

    property int headerCount: 0

    function selectRowColor(rowSate) {
        switch(rowSate) {
        case 0: return newAddedEntryBackgroundColor;
        case 1: return removedEntryBackgroundColor;
        case 2: return remainedEntryBackgroundColor;
        }
        return backgroundColor;
    }

    JobTableModel {
        id: jobTableModel
        jobTable: jobListComponent.jobTable
        columnsToShow: jobListComponent.columnsToShow

        onColumnsToShowChanged: () => {
            // Force the repeater to update.
            // Ugly. But it works.
            headerCount = 0;

            // Set to the correct amount.
            headerCount = jobListComponent.columnsToShow.length;
        }
    }

    // Mask
    Rectangle {
        anchors.fill: parent
        border.width: 2
        border.color: borderColor
        color: "transparent"
        z: 6
        radius: 6
    }

    Rectangle {
        anchors.fill: parent
        color: backgroundColor
        radius: 6
        border.width: 2
        border.color: borderColor

        TableView {
            id: tableView
            anchors.fill: parent
            // Should be 2 mathematically. Looks trash.
            anchors.margins: 2
            columnSpacing: 0
            rowSpacing: 2
            clip: true
            // Shift the content upwards by the height of the columnsHeader element.
            // This ensures that the first row of the table is not hidden by the columnsHeader and
            // is visible when the table is initially displayed.
            contentY: -columnsHeader.implicitHeight
            topMargin: columnsHeader.implicitHeight
            boundsBehavior: Flickable.StopAtBounds
            ScrollBar.vertical: ScrollBar {
                topPadding: columnsHeader.implicitHeight
                active: true
            }
            ScrollBar.horizontal: ScrollBar {
                active: true
                policy: ScrollBar.AlwaysOn
            }
            model: jobTableModel
            columnWidthProvider: function(column) {
                var divisor = jobTableModel.columns() > 0 ? jobTableModel.columns() : 1;
                return Math.max(minCellWidth, parent.width / divisor);
            }
            rowHeightProvider: function (column) {
                return 80;
            }

            delegate: Rectangle {
                color: selectRowColor(tableView.model.rowState(row))
                border.width: 0

                TextInput {
                    anchors.margins: 10
                    width: tableView.columnWidthProvider(modelData) - 20 // Margins times two.
                    color: textColor
                    font.pixelSize: 16
                    anchors.centerIn: parent
                    horizontalAlignment: Qt.AlignLeft
                    verticalAlignment: Qt.AlignCenter
                    text: model.display
                    wrapMode: Text.Wrap
                    readOnly: true
                    selectByMouse: true
                    // Standart Windows Selection Color.
                    selectionColor: "#2d8bfa"
                }
            }

            Rectangle {
                x: tableView.contentX
                y: tableView.contentY
                z: 1
                color: backgroundColor
                implicitWidth: jobListComponent.width
                height: 40
            }

            Row {
                id: columnsHeader
                y: tableView.contentY
                z: 2
                spacing: 0

                Repeater {
                    id: headerRepeater
                    model: headerCount

                    Rectangle {
                        implicitWidth: tableView.columnWidthProvider(modelData)
                        height: 40
                        color: backgroundColor

                        Label {
                            anchors.fill: parent
                            anchors.margins: 10
                            horizontalAlignment: Qt.AlignLeft
                            verticalAlignment: Qt.AlignCenter
                            text: tableView.model.headerData(modelData, Qt.Horizontal)
                            color: headerTextColor
                            font.pixelSize: 16
                            wrapMode: Text.Wrap
                        }
                    }
                }
            }
        }
    }
}
