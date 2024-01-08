// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Arrival

Item {
    id: fileSelector

    // Styling
    property color textColor: Style.textColor
    property color backgroundColor: Style.controlBackgroundColor
    property color borderColor: Style.controlBorderColor
    property color borderSelectedColor:Style.controlBorderColorActive

    // States and variables.
    property list<string> fileExtensionFilters: ([])
    property bool multiple: false
    property bool enableSelectDialog: true
    property bool cantainsDrag: false
    property string helpText: qsTr("Drop files here")
    property list<url> fileUrls: ([])

    signal filesAccepted(var files)

    function deselectFiles() {
        fileUrls = ([]);
        fileSelectDialog.currentFiles = ([]);
    }

    function correctPath(path) {
        path = path.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/, "");
        return decodeURIComponent(path);
    }

    DropArea {
        anchors.fill: parent

        function validateFileExtension(filePath) {
            if (!filePath) {
                return false;
            }
            if (fileSelector.fileExtensionFilters.length === 0) {
                return true;
            }
            return (fileSelector.fileExtensionFilters.join(",").indexOf("(*." + (("" + filePath).split('.').pop()) + ")") > -1);
        }

        onEntered: (drag) => {
            let acceptedCount = 0;
            for(let i = 0; i < drag.urls.length; i++) {
                if (validateFileExtension(drag.urls[i])) {
                    acceptedCount++;
                }
            }

            drag.accepted = (((fileSelector.multiple && drag.urls.length > 0) || (!fileSelector.multiple && drag.urls.length === 1)) && acceptedCount === drag.urls.length);
            fileSelector.cantainsDrag = drag.accepted;

        }

        onExited: () => {
            fileSelector.cantainsDrag = false;
        }

        onDropped: (drop) => {
            fileSelector.filesAccepted(drop.urls);
            fileSelector.fileUrls = drop.urls;
            fileSelectDialog.currentFiles = drop.urls;
            fileSelector.cantainsDrag = false;
        }
    }

    FileDialog {
        id: fileSelectDialog
        nameFilters: fileExtensionFilters
        fileMode: (fileSelector.multiple)? FileDialog.OpenFiles : FileDialog.OpenFile
        title: qsTr("Select file")
        onAccepted: () => {
            fileSelector.fileUrls = fileSelectDialog.selectedFiles;
            fileSelector.filesAccepted(fileSelectDialog.selectedFiles);
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: () => {
            if (fileSelector.enableSelectDialog) {
                fileSelectDialog.open();
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        id: rectangle
        color: backgroundColor
        radius: 6
        border.width: 2
        border.color: borderColor

        states: [
            State {
                name: "validFileHover"
                when: fileSelector.cantainsDrag
                PropertyChanges {
                    target: rectangle
                    border.color: borderSelectedColor
                    border.width: 2
                }
            },
            State {
                name: "fileSelected"
                when: fileSelector.fileUrls.length > 0

                PropertyChanges {
                    target: rectangle
                    border.color: borderSelectedColor
                    border.width: 2
                }
                PropertyChanges {
                    target: text
                    text: fileSelector.correctPath(fileSelector.fileUrls[0].toString())
                }
            }
        ]

        Text {
            id: text
            anchors.centerIn: parent
            anchors.margins: 20
            width: parent.width - 10
            wrapMode: Text.Wrap
            color: textColor
            horizontalAlignment: Qt.AlignHCenter
            text: fileSelector.helpText
            font.pixelSize: 20
        }
    }
}
