import QtQuick 2.7
import QtMultimedia 5.7
import Qt.labs.settings 1.0
import org.nehlsen.vs 1.0
import org.nehlsen.vs.client 1.0

Rectangle {
    id: root
    anchors.fill: parent
    color: "#646464"
    state: "offline"

    Settings {
        id: settings
        property alias serverService: vs.serverService
        property alias serverStorage: vs.serverStorage
        property string username: "my-username"
        property string password: "my-password"
        property string venue: "my-venue-token"
        property alias cacheFolder: vs.cacheFolder
        property alias slideShowInterval: vs.interval
    }

    VenueShot {
        id: vs
//        server: myServer
        onStatusChanged: {
            if (status == ClientStatus.Offline) {
                root.state = "offline"
            } else if (status == ClientStatus.Online) {
                root.state = "online"
            } else if (status == ClientStatus.Ready) {
                root.state = "ready"
            } else if (status == ClientStatus.Error) {
                root.state = "error"
            }
        }
    }

    onStateChanged: {
        console.info("state changed " + state)
    }

    states: [
        State {
            name: "offline"
            StateChangeScript {
                script: vs.acquireToken(settings.username, settings.password)
            }
        },
        State {
            name: "online"
            StateChangeScript {
                script: vs.venueToken = settings.venue
            }
        },
        State {
            name: "ready"
            StateChangeScript {
                script: {
                    rectCapturePublish.visible = true
                    loadingLabel.visible = false
                }
            }
        },
        State {
            name: "error"
        }
    ]

    Text {
        id: loadingLabel
        anchors.fill: parent
        text: "Loading..."
    }

    Rectangle {
        id: rectCapturePublish
        anchors.fill: parent
        color: root.color
        border.color: "red"
        visible: false
        state: "capture"

        property string capturedImagePath

        Camera {
            id: camera

            imageCapture {
                onImageCaptured: {
                    photoPreview.source = preview
                }
                onImageSaved: {
                    rectCapturePublish.capturedImagePath = path
                }
            }
        }

        states: [
            State {
                name: "capture"
                PropertyChanges {
                    target: pageCapture
                    visible: true
                }
                PropertyChanges {
                    target: pagePreview
                    visible: false
                }
            },
            State {
                name: "capture-delay"
                PropertyChanges {
                    target: pageCapture
                    visible: true
                }
                PropertyChanges {
                    target: pagePreview
                    visible: false
                }
            },
            State {
                name: "preview"
                PropertyChanges {
                    target: pageCapture
                    visible: false
                }
                PropertyChanges {
                    target: pagePreview
                    visible: true
                }
            }
        ]

        Rectangle {
            id: pageCapture
            anchors.fill: parent
            visible: false
            color: root.color

            VideoOutput {
                id: livePreview
                source: camera
                focus: visible // to receive focus and capture key events when visible
                anchors.fill: parent
    //            anchors.top: parent.top
    //            anchors.bottom: parent.bottom
    //            anchors.left: parent.left
                width: 200
            }

            Rectangle {
                id: btnCapture
                color: "steelblue"
                anchors.bottom: livePreview.bottom
                anchors.right: livePreview.right
                anchors.bottomMargin: 15
                anchors.rightMargin: 15
                width: 50
                height: 50
                radius: 25
                opacity: 0.75

                Image {
                    source: "../image/camera-shutter.svg"
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent;
                    anchors.margins: 3
                }

                MouseArea {
                    anchors.fill: parent;
                    onClicked: {
//                        camera.imageCapture.capture();
//                        rectCapturePublish.state = "preview"
                        rectCapturePublish.state = "capture-delay"
                        delayCapture.start()
                    }
                }
            }

            Rectangle {
                id: delayDisplay
                color: "#ff5500"
                anchors.bottom: livePreview.bottom
                anchors.right: livePreview.right
                anchors.bottomMargin: 15
                anchors.rightMargin: 15
                width: 50
                height: 50
                radius: 25
                opacity: 0.75
                visible: delayCapture.running

                Text {
                    text: delayCapture.seconds
                    style: Text.Outline
                    styleColor: "red"
                    fontSizeMode: Text.Fit
                    anchors.centerIn: delayDisplay
                    font.pixelSize: delayDisplay.height -6
                }
            }

            Timer {
                id: delayCapture
                property int defaultSeconds: 3
                property int seconds: defaultSeconds
                repeat: true
                interval: 1000
                onTriggered: {
                    delayCapture.seconds--;
                    if (delayCapture.seconds == 0) {
                        running = false;
                        delayCapture.seconds = delayCapture.defaultSeconds
                        camera.imageCapture.capture();
                        rectCapturePublish.state = "preview"
                    }
                }
            }
        }

        Rectangle {
            id: pagePreview
            anchors.fill: parent
            visible: false
            color: root.color

            Image {
                id: photoPreview
                anchors.fill: parent
//                anchors.left: livePreview.left
//                anchors.top: livePreview.bottom
//                width: 200
                fillMode: Image.PreserveAspectFit
            }

            Rectangle {
                id: btnDiscard
                color: "red"
                anchors.top: photoPreview.top
                anchors.left: photoPreview.left
                anchors.topMargin: 15
                anchors.leftMargin: 15
                width: 50
                height: 50
                radius: 25
                opacity: 0.75

                Image {
                    source: "../image/cancel.svg"
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent;
                    anchors.margins: 12
                }

                MouseArea {
                    anchors.fill: parent;
                    onClicked: {
                        rectCapturePublish.state = "capture"
                    }
                }
            }

            Rectangle {
                id: btnPublish
                color: "green"
                anchors.top: photoPreview.top
                anchors.right: photoPreview.right
                anchors.topMargin: 15
                anchors.rightMargin: 15
                width: 50
                height: 50
                radius: 25
                opacity: 0.75

                Image {
                    source: "../image/upload.svg"
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent;
                    anchors.margins: 5.5
                }

                MouseArea {
                    anchors.fill: parent;
                    onClicked: {
                        vs.postPicture(rectCapturePublish.capturedImagePath)
                        rectCapturePublish.state = "capture"
                    }
                }
            }
        }
    }
}
