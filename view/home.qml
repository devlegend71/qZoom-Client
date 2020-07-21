import QtQuick 2.0
import QtQuick.Controls 1.4
import "../components" as C

Rectangle {
    color: "dimgray"

    Row {
        id: optionRow
        visible: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        spacing: 32
        Rectangle {
            width: 200
            height: 200
            color: "#141414"
            radius: 10
            Text {
                text: "Join"
                color: "white"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    optionRow.visible = false;
                    joinSessionColumn.visible = true
                    backButton.visible = true
                }
            }
        }

        Rectangle {
            width: 200
            height: 200
            color: "#141414"
            radius: 10
            Text {
                text: "Host"
                color: "white"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Rectangle {
            width: 200
            height: 200
            color: "#141414"
            radius: 10
            Text {
                text: "Log in"
                color: "white"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    optionRow.visible = false;
                    loginColumn.visible = true
                    backButton.visible = true
                }
            }
        }
    }

    Column {
        id: joinSessionColumn
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        spacing: 32
        TextField {
            id: roomIdField
            width: 200
            //height: 32
            text: qsTr("")
            ///font.pixelSize: 24
            placeholderText: qsTr("Room ID")
            selectByMouse: true
            focus: true
        }

        TextField {
            id: roomPasswordField
            width: 200
            //height: 32
            text: qsTr("")
            ///font.pixelSize: 24
            placeholderText: qsTr("Password")
            selectByMouse: true
            focus: true
        }

        C.PushButton {
            id: joinSessionButton
            text: qsTr("Join Session")
            //anchors.verticalCenterOffset: 64
            //visible: false
            font.pixelSize: 32
            onClicked: joinSession()
            onDoubleClicked: changePage("session")
        }

        Text {
            id: errorText
            text: " "
            //visible: true
            font.pixelSize: 18
            color: "red"
        }
    }

    Column {
        id: loginColumn
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        spacing: 32
        TextField {
            id: loginUsernameField
            width: 200
            //height: 32
            text: qsTr("")
            ///font.pixelSize: 24
            placeholderText: qsTr("Username")
            selectByMouse: true
            focus: true
        }

        TextField {
            id: loginPasswordField
            width: 200
            //height: 32
            text: qsTr("")
            ///font.pixelSize: 24
            placeholderText: qsTr("Password")
            //echoMode: "Password"
            selectByMouse: true
            focus: true
        }

        C.PushButton {
            id: loginButton
            text: qsTr("Login")
            //anchors.verticalCenterOffset: 64
            //visible: false
            font.pixelSize: 32
            onClicked: login()
        }
    }

    C.PushButton {
        id: backButton
        text: "Back"
        font.pixelSize: 32
        visible: false
        onClicked: clear()
    }

    function joinSession() {
        if (sessionHandler.joinSession(roomIdField.text, roomPasswordField.text)) {
            changePage("session")
        }
        else {
            console.log("no such session")
            errorText.text = "Failed to join session"
        }
    }

    function login() {
        if (userHandler.login(loginUsernameField.text, loginPasswordField.text)) {
            console.log("Successfully logged in!")
            clear()
        }
        else {
            console.log("Failed to login")
            errorText.text = "Failed to login"
        }
    }

    function clear() {
        joinSessionColumn.visible = false
        loginColumn.visible = false
        optionRow.visible = true
        errorText.text = " "
        backButton.visible = false
    }

    Component.onCompleted: {
        setTitle("qZoom :: Home");
    }
}