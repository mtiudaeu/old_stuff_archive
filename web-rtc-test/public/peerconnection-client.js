"use strict"

class ClientPeerConnection {
  //---------------------------------------------------------------
  constructor(onIceCandidate, onDataChannelMessage, onDataChannelOpen,
              onAddStreams) {
    let config = {
      'iceServers': [
        {'urls': 'stun:stun.services.mozilla.com'},
        {'urls': 'stun:stun.l.google.com:19302'},
      ]
    };
    this.peerConnection = new RTCPeerConnection(config);
    this.peerConnection.onicecandidate = onIceCandidate;
    let localPeerConn = this.peerConnection;
    navigator.getUserMedia({"audio": true, "video": true}, function(stream) {
      localPeerConn.addStream(stream);
    });
    function localOnAddStream(evt) {
      let remoteStream = evt.stream;
      let mediaStreams = localPeerConn.getLocalStreams();
      if (mediaStreams.length > 0) {
        onAddStreams(remoteStream, mediaStreams[0]);
      }
    }
    this.peerConnection.onaddstream = localOnAddStream;

    this.peerConnection.ondatachannel =
        function(event) { event.channel.onmessage = onDataChannelMessage; };

    this.dataChannel = this.peerConnection.createDataChannel("dataChannel");
    this.dataChannel.onmessage = onDataChannelMessage;
    this.dataChannel.onopen = onDataChannelOpen;
    // FIXME handle for that?
    this.dataChannel.onclose =
        function() { console.log("The Data Channel is Closed"); };
    this.dataChannel.onerror =
        function(error) { console.log("Data Channel Error:", error); };
  }
  //---------------------------------------------------------------
  createOffer(handleOffer) {
    var thisPeerConn = this.peerConnection;
    thisPeerConn.createOffer()
        .then(function(
            description){thisPeerConn.setLocalDescription(description)})
        .then(function(){handleOffer(thisPeerConn.localDescription)})
        .catch (errorHandler);
  }
  //---------------------------------------------------------------
  answerOffer(remoteDescription, handleOffer) {
    this.peerConnection.setRemoteDescription(
                            new RTCSessionDescription(remoteDescription))
        .catch (errorHandler);

    // Only create answers in response to offers
    if (remoteDescription.type == 'offer') {
      var thisPeerConn = this.peerConnection;
      thisPeerConn.createAnswer()
          .then(function(
              description){thisPeerConn.setLocalDescription(description)})
          .then(function(){handleOffer(thisPeerConn.localDescription)})
          .catch (errorHandler);
    }
  }
  //---------------------------------------------------------------
  setIceCandidate(iceCandidate) {
    this.peerConnection.addIceCandidate(new RTCIceCandidate(iceCandidate))
        .catch (errorHandler);
  }
  //---------------------------------------------------------------
  sendData(msg) { this.dataChannel.send(msg); }
  //---------------------------------------------------------------
  addStream(stream) { this.peerConnection.addStream(stream); }
}
