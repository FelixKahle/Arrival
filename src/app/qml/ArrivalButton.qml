// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

import QtQuick
import QtQuick.Controls

Button {
    property string buttonText: ""
    property color buttonColor: "#5865f2"
    property color buttonHoverColor: "#4752c4"

    contentItem: Text {
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: buttonText
        color: "white"
        font.pixelSize: 18
    }

    background: Rectangle {
        implicitWidth: 150
        implicitHeight: 40
        color: parent.hovered ? buttonHoverColor : buttonColor
        radius: 4
    }
}
