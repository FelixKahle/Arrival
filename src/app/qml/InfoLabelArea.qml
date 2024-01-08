// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

import QtQuick
import QtQuick.Controls

Rectangle {
    property string text: ""

    width: 200
    height: 40
    color: Style.controlBackgroundColor
    radius: 6
    border.width: 2
    border.color: Style.controlBorderColor

    Label {
        color: Style.textColor
        font.pixelSize: 16
        anchors.centerIn: parent
        text: parent.text
    }
}
