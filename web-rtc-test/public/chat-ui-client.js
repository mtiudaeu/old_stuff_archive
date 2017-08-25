"use strict"

let uiCallbacks;

class ChatClientUI {
  constructor(sendButtonName, receiveBoxName, messageInputName, clientListName,
              localVideoName, remoteVideoName) {
    // UI Callbacks to activate data send.
    let sendButton = {
      uiObject: document.getElementById(sendButtonName),
      enable: function() { sendButton.uiObject.disabled = false; }
    };
    sendButton.uiObject.addEventListener('click', function() {
      var message = uiCallbacks.messageInput.getMessage();
      connectionToServer.sendDataToPeer(message);
    }, false);
    sendButton.uiObject.disabled = true;

    // UI Callbacks to receive data.
    let receiveBox = {
      uiObject: document.getElementById(receiveBoxName),
      receivedMsg: function(data) {
        var el = document.createElement("p");
        var txtNode = document.createTextNode(data);
        el.appendChild(txtNode);
        receiveBox.uiObject.appendChild(el);
      }
    }

    // UI Callbacks to capture data to send.
    let messageInput = {
      uiObject: document.getElementById(messageInputName),
      getMessage: function() {
        let message = messageInput.uiObject.value;
        messageInput.uiObject.value = "";
        messageInput.uiObject.focus();
        return message;
      }
    }

    // UI Callbacks to list all available clients.
    let clientList = {
      uiObject: document.getElementById(clientListName),
      refresh: function(list) {
        // Clear list.
        $('#' + clientList.uiObject.id).empty();
        // Add every client in the list with appropriate button to initiate
        // call.
        for (var i in list) {
          let el = document.createElement("li");
          let txtNode = document.createTextNode(list[i].name);
          el.appendChild(txtNode);
          clientList.uiObject.appendChild(el);
          if (list[i].id !== idOnServer) {
            let buttonNode = document.createElement("button");
            let buttonTxtNode = document.createTextNode("Call");
            buttonNode.appendChild(buttonTxtNode);
            el.appendChild(buttonNode);

            (function(idCopy) {
              buttonNode.addEventListener('click', function() {
                // FIXME get success status.
                connectionToServer.startConnectionWith(idCopy);
              }, false);
            })(list[i].id);
          }
        }
      }
    }

    let localVideo = {
      uiObject: document.getElementById(localVideoName),
      addStream : function(stream) {
        localVideo.uiObject.src = URL.createObjectURL(stream);
      }
    };

    let remoteVideo = {
      uiObject: document.getElementById(remoteVideoName),
      addStream : function(stream) {
        remoteVideo.uiObject.src = URL.createObjectURL(stream);
      }
    };

    uiCallbacks = {
      messageInput: messageInput,
      receiveBox: receiveBox,
      sendButton: sendButton,
      clientList: clientList,
      localVideo: localVideo,
      remoteVideo: remoteVideo
    }
  }
  //---------------------------------------------------------------
  onDataChannelMessage(event) {
    uiCallbacks.receiveBox.receivedMsg(event.data);
  }

  //---------------------------------------------------------------
  onDataChannelOpen() {
    this.send("Connection established.");
    uiCallbacks.sendButton.enable();
  }
  //---------------------------------------------------------------
  onAddStreams(remoteStream, localStream) {
     uiCallbacks.remoteVideo.addStream(remoteStream);
     uiCallbacks.localVideo.addStream(localStream);
  }
}
