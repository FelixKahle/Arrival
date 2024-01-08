// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Arrival

Window {
    id: root
    width: 1280
    height: 760
    minimumWidth: 1100
    minimumHeight: 600
    visible: true
    title: qsTr("Arrival")
    color: Style.backgroundColor

    App {
        anchors.fill: parent
    }
}
