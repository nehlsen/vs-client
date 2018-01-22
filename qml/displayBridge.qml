import QtQuick 2.7
import Qt.labs.settings 1.0
import org.nehlsen.vs 1.0
import org.nehlsen.vs.client 1.0
import "./" as VsModules

Rectangle {
    id: root
    anchors.fill: parent
    color: "black"
    state: "offline"

    Settings {
        id: settings
        property alias url: vs.server
        property string username: "my-username"
        property string password: "my-password"
        property string venue: "my-venue-token"
//        property int autoUpdateInterval: vs.autoUpdateInterval
        property int autoUpdateInterval: 10
        property alias cacheFolder: vs.cacheFolder
        property alias slideShowInterval: vs.interval
    }

    VenueShot {
        id: vs
//        server: myServer
        autoFetchPicturesEnabled: true
//        cacheFolder: myCacheFolder
//        interval: mySlideShowInterval
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
        onCurrentPictureChanged: {
            display.setPicture(currentPicture)
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
                    display.visible = true
                    loadingLabel.visible = false
                    vs.autoUpdateInterval = settings.autoUpdateInterval
                }
            }
        },
        State {
            name: "error"
        }
    ]

    VsModules.Loading {
        id: loadingLabel
    }
    VsModules.PictureSwitcher {
        id: display
    }
}
