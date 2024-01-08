// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

import QtQuick
import QtQuick.Controls
import Arrival

CheckBox {
    id: control
    checked: true

    indicator: Rectangle {
        implicitWidth: 26
        implicitHeight: 26
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: 4
        border.color: control.checked ? "#4752C4" : Style.controlBorderColor
        color: "#404244"
        border.width: 2

        Rectangle {
            width: 14
            height: 14
            x: 6
            y: 6
            radius: 2
            color: control.down ? "#4752C4" : "#5865F2"
            visible: control.checked
        }
    }
}
