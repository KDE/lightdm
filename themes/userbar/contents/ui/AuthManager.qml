/*
This file is part of LightDM-KDE.

Copyright 2013 David Edmundson <kde@davidedmundson.co.uk>

LightDM-KDE is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LightDM-KDE is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LightDM-KDE.  If not, see <http://www.gnu.org/licenses/>.
*/
import QtQuick 1.0
import org.kde.qtextracomponents 0.1 as QtExtraComponents
import org.kde.lightdm 0.1 as LightDM


FocusScope {
    id: base

    /** Label to show for authentication failures*/
    property string feedbackText: ""
    /** Label to use for authentication, i.e "Password" or "Pin number" */
    property string authText: ""

    /** Button to show if authentication is not required*/
    property Component loginButtonItem

    /** Text field to show for all text prompts */
    property Component textFieldItem

    signal authComplete

    //private API
    property bool _hasShownPrompt: false

    function startAuth(username) {
        if (username == greeter.guestLoginName) {
            greeter.authenticateAsGuest();
        } else {
            greeter.authenticate(username);
        }
        authManager._hasShownPrompt = false;
//         authWidget.sourceComponent = undefined;
    }

    Connections {
        target: greeter;

        onShowPrompt: {
            console.log(text);
            authWidget.sourceComponent = base.textFieldItem
            base._hasShownPrompt = true;
        }

        onAuthenticationComplete: {
            if(greeter.authenticated) {
                //if we have prompted the user interactively and auth finishes, log in. Otherwise show a button. Prevents "accidental" logins
                if (base._hasShownPrompt) {
                    startSessionSync();
                }
                else {
                    authWidget.sourceComponent = base.loginButtonItem
                }
            } else {
                feedbackLabel = i18n("Sorry, incorrect password. Please try again.");
                authWidget.selectAll()
                authWidget.forceActiveFocus()
            }
        }
    }

    Loader {
        id: authWidget
        onStatusChanged: if (authWidget.status == Loader.Ready) console.log('Loaded')
    }

}
