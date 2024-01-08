// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

pragma Singleton

import QtQuick

QtObject {

    // Fonts
    readonly property color textColor: "#F2F3F5"

    // Backgrounds
    readonly property color backgroundColor: "#1E1F22"
    readonly property color controlBackgroundColor: "#2B2D31"

    // Borders.
    readonly property color controlBorderColor: "#3A3D43"
    readonly property color controlBorderColorActive: "#5151FA"

    // Table specific
    readonly property color newAddedEntryBackgroundColor: "#248046"
    readonly property color removedEntryBackgroundColor: "#DA373C"
    readonly property color remainedEntryBackgroundColor: "#404249"
}
