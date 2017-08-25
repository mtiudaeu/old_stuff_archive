"use strict"

let idOnServer;
let connectionToServer;
let chatClientProtocol;
let chatClientUI;

//---------------------------------------------------------------
function errorHandler(error) { console.log(error); }

//---------------------------------------------------------------
function pageInit(sendButtonName, receiveBoxName, messageInputName,
                  clientListName, localVideoName, remoteVideoName) {
  chatClientUI = new ChatClientUI(sendButtonName, receiveBoxName,
                                  messageInputName, clientListName, localVideoName, remoteVideoName);
  chatClientProtocol = new ChatClientProtocol(uiCallbacks.clientList.refresh);

  connectionToServer = new ConnectionToServer(
      chatClientProtocol.sendIceCandidate, chatClientProtocol.sendDescription,
      chatClientProtocol.onMessageFromServer,
      chatClientProtocol.onOpenFromServer, chatClientUI.onDataChannelMessage,
      chatClientUI.onDataChannelOpen, chatClientUI.onAddStreams);
}
