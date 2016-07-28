/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0


Page
{
    id: page1

//    ListModel {
//        id: dummy
//        ListElement { text: "Alice" }
//        ListElement { text: "Bob" }
//        ListElement { text: "Jane" }
//        ListElement { text: "Harry" }
//        ListElement { text: "Wendy" }
//    }
    SilicaListView
    {
        PullDownMenu
        {
            MenuItem
            {
                text: "Discover"
                onClicked: dmodel.refresh();
            }
        }
        PushUpMenu
        {
            MenuItem
            {
                text: "Watson Voice Input"
                onClicked: pageStack.push(Qt.resolvedUrl("ThirdPage.qml"));
            }
            MenuItem
            {
                text: "Yandex Voice Input"
                onClicked: pageStack.push(Qt.resolvedUrl("YandexPage.qml"));
            }
        }

        anchors.fill: parent

        header: PageHeader {
            id: header
            title: "Available devices"
        }

        model: root.dmodel

        delegate: BackgroundItem {
            Label {
                // qml model-delegate-view thing is strange
                // you can't display model data() with displayRole, it works only with widgets
                // so, you can't reuse displayRole -- need to implement nameRoles
                // not nice at all
                // if you uncomment dummy ListModel from top
                // and replace the text entry at the bottom with
                // text: modelData
                // you get the good picture and all is fine
                // when trying to do the same with abstractmodel you will get
                // ReferenceError: textRole is not defined
                text: name
                x: Theme.horizontalPageMargin
            }
            onClicked:
            {
                pageStack.push(Qt.resolvedUrl("SecondPage.qml"));
                dmodel.activate(index);
            }
        }
    }
}


