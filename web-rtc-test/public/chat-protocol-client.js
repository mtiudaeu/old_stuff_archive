"use strict"

let CHAT_MSG_SEND_DESCRIPTION = 'sendDescription';
let CHAT_MSG_SEND_ICE = 'sendIce';
let CHAT_MSG_GET_DESCRIPTION = 'getDescription';
let CHAT_MSG_GET_ICE = 'getIce';
let CHAT_MSG_CLIENT_LIST = 'clientList';
let CHAT_MSG_CLIENT_ID = 'initClientId';
let CHAT_MSG_SET_NAME = 'setName';

class ChatClientProtocol {
  constructor(refreshListCallback) {
    var localThis = this;
    this.onMessageFromServer = function(connectionToServer, message) {
      localThis.privOnMessageFromServer(localThis, connectionToServer, message);
    };
    this.refreshClientList = refreshListCallback;
  }
  //---------------------------------------------------------------
  sendIceCandidate(serverConnection, candidate, peerId) {
    serverConnection.send(JSON.stringify(
        {MSG_ID: CHAT_MSG_SEND_ICE, DATA: {'ice': candidate, 'toId': peerId}}));
  }
  //---------------------------------------------------------------
  sendDescription(serverConnection, description, peerId) {
    serverConnection.send(JSON.stringify(
        {MSG_ID: CHAT_MSG_SEND_DESCRIPTION, DATA: {'sdp': description, 'toId': peerId}}))
  }
  //---------------------------------------------------------------
  privOnMessageFromServer(chatClientProtocol, connectionToServer, message) {
    let signal = JSON.parse(message.data);
    switch (signal.MSG_ID) {
      case CHAT_MSG_CLIENT_LIST:
        chatClientProtocol.refreshClientList(signal.DATA);
        break;
      case CHAT_MSG_CLIENT_ID:
        idOnServer = signal.DATA;
        break;
      case CHAT_MSG_GET_DESCRIPTION:
        if (signal.DATA.sdp) {
          connectionToServer.peerConnectionHolder.peerId = signal.DATA.fromId;

          function handleOfferLocal(description) {
            chatClientProtocol.sendDescription(
                connectionToServer.serverConnection, description,
                connectionToServer.peerConnectionHolder.peerId);
          };

          connectionToServer.peerConnectionHolder.answerOffer(signal.DATA.sdp,
                                                              handleOfferLocal);
        }
      case CHAT_MSG_GET_ICE:
        if (signal.DATA.ice) {
          connectionToServer.peerConnectionHolder.setIceCandidate(
              signal.DATA.ice);
        }
        break;
    }
  }
  //---------------------------------------------------------------
  onOpenFromServer() {
    // FIXME add prompt for valid name.
    this.send(JSON.stringify({MSG_ID: CHAT_MSG_SET_NAME, DATA: {'name': 'MDTMP'}}));
  }
  //---------------------------------------------------------------
}
