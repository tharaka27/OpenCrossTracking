import QtQuick 2.9
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.5
import QtQuick.Extras 1.2
import QtQuick.Controls.Styles.Desktop 1.0
import QtQuick.Layouts 1.3
import QtGraphicalEffects.private 1.0

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Row {
        id: row
        anchors.fill: parent
        spacing: 0

        Column {
            id: column
            x: 0
            width: 100
            height: 480

            Image {
                id: image
                width: 100
                height: 100
                source: "images/person.png"
            }

            Button {
                id: button
                width: 100
                text: qsTr("Calibrate")
                checkable: false
            }
        }

        Column {
            id: column1
            width: 540
            height: 480
            transformOrigin: Item.Center

            Grid {
                id: grid
                width: 540
                height: 480
                spacing: 4
                columns: 2
                rows: 4

                Text { text: "Three"; font.bold: true; }
                Text { x: 280; y: 20; text: "Three"; font.bold: true; }

                Image {
                    id: image1
                    y: 0
                    width: 260
                    height: 210
                    source: "images/eye.png"
                }

                Image {
                    id: image2
                    x: 280
                    y: 0
                    width: 260
                    height: 210
                    source: "images/cropped_eye.png"
                }

                Text { text: "Three"; font.bold: true; }
                Text { x: 280; y: 254; text: "Three"; font.bold: true; }

                Image {
                    id: image3
                    x: 0
                    width: 260
                    height: 220
                    source: "images/eye.png"
                }

                Image {
                    id: image4
                    x: 280
                    y: 260
                    width: 260
                    height: 220
                    source: "images/cropped_eye.png"
                }
            }
        }
    }
}
