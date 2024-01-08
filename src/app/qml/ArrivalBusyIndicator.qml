// Copyright 2023 WorldCourier. All rights reserved.
//
// Author: Felix Kahle, A123234, felix.kahle@worldcourier.de

import QtQuick 2.12
import QtQuick.Controls 2.12

BusyIndicator {
    id: control
    property color indicatorColor: "white"//"#5865f2"

    contentItem: Item {
        implicitWidth: parent.width
        implicitHeight: parent.height

        Item {
            id: item
            x: parent.width / 2 - parent.width / 2
            y: parent.height / 2 - parent.height / 2
            width: parent.width
            height: parent.height
            opacity: control.running ? 1 : 0

            RotationAnimator {
                target: item
                running: control.visible && control.running
                from: 0
                to: 360
                loops: Animation.Infinite
                duration: 2000
            }

            Repeater {
                id: repeater
                model: 8

                Rectangle {
                    x: item.width / 2 - width / 2
                    y: item.height / 2 - height / 2
                    implicitWidth: 10
                    implicitHeight: 10
                    radius: 5
                    color: indicatorColor
                    transform: [
                        Translate {
                            y: -Math.min(item.width, item.height) * 0.5 + 5
                        },
                        Rotation {
                            angle: index / repeater.count * 360
                            origin.x: 5
                            origin.y: 5
                        }
                    ]
                }
            }
        }
    }
}
