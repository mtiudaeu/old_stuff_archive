"use strict"

let TARGET_ID_INVALID = -1;

class ConnectionToServer {
  //---------------------------------------------------------------
  constructor(sendIceCandidate, sendDescription, onMessageFromServer,
              onOpenFromServer,
              // Peer connection callback.
              onDataChannelMessage, onDataChannelOpen, onAddStreams) {
    let localThis = this;
    function localHandleIceCandidate(event) {
      if (event.candidate != null) {
        return sendIceCandidate(localThis.serverConnection, event.candidate,
                                localThis.peerConnectionHolder.peerId);
      }
    }
    this.sendDescription = sendDescription;
    this.peerConnectionHolder = new ClientPeerConnection(
        localHandleIceCandidate, onDataChannelMessage, onDataChannelOpen, onAddStreams);
    // Peer id on server used to redirect request to appropriate peer.
    this.peerConnectionHolder.peerId = TARGET_ID_INVALID;

    this.serverConnection =
        new WebSocket('wss://' + window.location.hostname + ':80');

    this.serverConnection.onopen = onOpenFromServer;
    this.serverConnection.onclose = function onClose() {
      console.log("Server socket connection has been close.");
    };

    function localOnMessageFromServer(message) {
      onMessageFromServer(localThis, message);
    };
    this.serverConnection.onmessage = localOnMessageFromServer;
  }
  //---------------------------------------------------------------
  startConnectionWith(peerId) {
    this.peerConnectionHolder.peerId = peerId;
    let localThis = this;
    function handleOfferLocal(description) {
      localThis.sendDescription(localThis.serverConnection, description,
                                localThis.peerConnectionHolder.peerId);
    };
    this.peerConnectionHolder.createOffer(handleOfferLocal);
  }
  //---------------------------------------------------------------
  sendDataToPeer(data) { this.peerConnectionHolder.sendData(data); }
  //---------------------------------------------------------------
  addStreamToPeer(stream) { this.peerConnectionHolder.addStream(stream); }
}
