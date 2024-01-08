// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Arrival

Popup {
    id: headerSelector

    // Styling
    property color backgroundColor: Style.backgroundColor
    property color templatesBackgroundColor: Style.controlBackgroundColor
    property color headerSelectorBackgroundColor: Style.controlBackgroundColor
    property color outlineColor: Style.controlBorderColor

    readonly property list<int> selectedHeaderIndices: headerListModel.selectedHeaderIndices
    required property JobTableBackend table
    required property SelectedHeadersTemplateList selectedHeadersTemplateList

    signal saveCurrentSelectionButtonClicked

    function resetTemplateSelection() {
        selectedHeadersTemplateListView.currentIndex = -1;
    }

    background: Rectangle {
        implicitHeight: parent.height
        color: backgroundColor
        border.width: 0
        radius: 12
    }

    function saveCurrentSelectionAsTemplate(templateName) {
        selectedHeadersTemplateList.addNewTemplate(table.formatIdentifier, templateName, selectedHeaderIndices);
    }

    HeaderListModel {
        id: headerListModel
        jobTable: table

        onSelectedHeader: () => {
            resetTemplateSelection();
        }

        onJobTableChanged: () => {
            resetTemplateSelection();
        }
    }

    SelectedHeadersTemplateModel {
        id: selectedHeadersTemplateModel
        showIdentifier: table.formatIdentifier
        templateList: selectedHeadersTemplateList
    }

    modal: true
    Overlay.modal: Rectangle {
        color: "#BF070809"
    }

    Popup {
        id: saveTemplatePopup
        anchors.centerIn: parent
        margins: 20
        modal: true
        width: 500
        height: 200
        visible: false
        background: Rectangle {
            color: backgroundColor
            radius: 6
        }

        onOpened: () => {
            textInput.selectAll();
            textInput.forceActiveFocus();
        }

        onClosed: () => {
            textInput.text = "New Name"
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

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20

            TextField {
                id: textInput
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                height: 50
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                font.pixelSize: 30
                text: "New Name"
                color: Style.textColor

                background: Rectangle {
                    color: Style.controlBackgroundColor
                    border.width: 2
                    border.color: Style.controlBorderColor
                    implicitWidth: textInput.width
                    height: textInput.height
                    radius: 6
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight | Qt.AlignBottom

                ArrivalButton {
                    width: 60
                    height: 40
                    Layout.alignment: Qt.AlignRight
                    buttonText: "Cancel"

                    onClicked: () => {
                        saveTemplatePopup.close();
                    }
                }

                ArrivalButton {
                    Layout.alignment: Qt.AlignRight
                    buttonText: "Save"

                    onClicked: () => {
                        headerSelector.saveCurrentSelectionAsTemplate(textInput.text);
                        saveTemplatePopup.close();
                    }
                }
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 20

            Rectangle {
                id: selectedHeadersTemplateBackground
                implicitWidth: 300
                Layout.fillHeight: true
                color: templatesBackgroundColor
                border.color: outlineColor
                border.width: 2
                radius: 6

                // Mask
                Rectangle {
                    implicitWidth: 300
                    Layout.fillHeight: true
                    border.color: outlineColor
                    border.width: 2
                    radius: 6
                    color: "transparent"
                    z: 6
                }

                ListView {
                    id: selectedHeadersTemplateListView
                    anchors.fill: parent
                    clip: true
                    focus: true
                    boundsBehavior: Flickable.StopAtBounds
                    model: selectedHeadersTemplateModel
                    anchors.margins: 2
                    currentIndex: -1

                    delegate: Rectangle {
                        id: bg
                        width: selectedHeadersTemplateBackground.width
                        height: 60
                        color: model.enabled ? (ListView.isCurrentItem ? "#404249" : (hovered ? "#36373D" : templatesBackgroundColor)) : templatesBackgroundColor

                        radius: 6

                        property bool hovered: false

                        RowLayout {
                            anchors.margins: 10
                            anchors.fill: parent
                            z: mouseArea.z + 1

                            Label {
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter

                                text: model.templateName
                                color: model.enabled ? "white" : "grey"
                                font.pixelSize: 16
                            }

                            Button {
                                contentItem: Text {
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    text: qsTr("Delete")
                                    color: "white"
                                    font.pixelSize: 14
                                }

                                background: Rectangle {
                                    implicitWidth: 90
                                    implicitHeight: 30
                                    color: parent.hovered ? "#a12828" : "#da373c"
                                    radius: 4
                                }

                                onClicked: () => {
                                    selectedHeadersTemplateList.removeItemAt(index);
                                }
                            }
                        }

                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                if (model.enabled) {
                                    selectedHeadersTemplateListView.currentIndex = index

                                    // Set the correct indices.
                                    var indices = selectedHeadersTemplateList.getTemplateIndices(index);
                                    headerListModel.selectedHeaderIndices = indices;
                                }
                            }

                            onEntered: () => {
                                bg.hovered = true;
                            }

                            onExited: () => {
                                bg.hovered = false;
                            }
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: headerSelectorBackgroundColor
                radius: 6
                border.width: 2
                border.color: outlineColor

                ListView {
                    anchors.fill: parent
                    clip: true
                    boundsBehavior: Flickable.StopAtBounds
                    model: headerListModel

                    delegate: Rectangle {
                        height: 40
                        color: "#1E1F22"

                        RowLayout {
                            anchors.fill: parent

                            ArrivalCheckBox {
                                Layout.alignment: Qt.AlignLeft
                                checked: model.selected
                                onClicked: model.selected = checked
                            }

                            Label {
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignLeft
                                horizontalAlignment: Qt.AlignLeft
                                verticalAlignment: Qt.AlignCenter
                                text: model.headerName
                                color: "white"
                                font.pixelSize: 16
                            }
                        }
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            spacing: 10

            ArrivalButton {
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                buttonText: "Cancel"
                onClicked: () => {
                    headerSelector.close();
                }
            }

            ArrivalButton {
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                buttonText: "Save Selection"
                onClicked: () => {
                    // Only if something is selected allow for saving.
                    if (selectedHeaderIndices.length > 0)
                    {
                        headerSelector.saveCurrentSelectionButtonClicked();
                        saveTemplatePopup.open();
                    }
                }
            }
        }
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
                from: 0.9
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
                to: 0.9
                duration: 300
            }
        }
    }
}

