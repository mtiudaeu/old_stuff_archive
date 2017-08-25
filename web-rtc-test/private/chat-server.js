"use strict"

let WebSocketServer = require('ws').Server;

// FIXME possible overflow...
let wsIdCounter = 0;
let wsConnections = {};
let wss;

// Communication protocol
let CHAT_MSG_SEND_DESCRIPTION = 'sendDescription';
let CHAT_MSG_SEND_ICE = 'sendIce';
let CHAT_MSG_GET_DESCRIPTION = 'getDescription';
let CHAT_MSG_GET_ICE = 'getIce';
let CHAT_MSG_CLIENT_LIST = 'clientList';
let CHAT_MSG_CLIENT_ID = 'initClientId';
let CHAT_MSG_SET_NAME = 'setName';


module.exports = {
  init: function(httpsServer) {
    wss = new WebSocketServer({server: httpsServer});

    wss.on('connection', function(ws) {
      ws.id = wsIdCounter++;
      let wsObject = { name: "Noob", ws: ws };
      wsConnections[ws.id] = wsObject;
      console.log("Connection open (id:" + ws.id + ")");
      ws.send(JSON.stringify({MSG_ID: CHAT_MSG_CLIENT_ID, DATA: ws.id}));

      ws.on('message', function(message) {
        let jsonMessage = JSON.parse(message);
        let targetId;
        let jsonResponce;
         switch (jsonMessage.MSG_ID) {
           case CHAT_MSG_SET_NAME:
             wsConnections[ws.id].name = jsonMessage.DATA.name;
             sendClientList();
             break;
           case CHAT_MSG_SEND_DESCRIPTION:
             targetId = jsonMessage.DATA.toId;
             jsonResponce = {MSG_ID: CHAT_MSG_GET_DESCRIPTION, DATA: jsonMessage.DATA};
             delete jsonResponce.DATA.toId;
             jsonResponce.DATA.fromId = ws.id;
             wsConnections[targetId].ws.send(JSON.stringify(jsonResponce));
             break;
           case CHAT_MSG_SEND_ICE:
             targetId = jsonMessage.DATA.toId;
             jsonResponce = {MSG_ID: CHAT_MSG_GET_ICE, DATA: jsonMessage.DATA};
             delete jsonResponce.DATA.toId;
             wsConnections[targetId].ws.send(JSON.stringify(jsonResponce));
             break;
           default:
             console.log("Received invalid message : " + jsonMessage.MSG_ID);
             break;
         }
      });
      ws.on('close', function() {
        console.log("Connection closed (id:" + this.id + ")");
        delete wsConnections[this.id];
        sendClientList();
      });

    });

    wss.broadcast = function(data) {
      for (let i in this.clients) {
        this.clients[i].send(data);
      }
    };

  }
};

let sendClientList = function() {
  let clientList = [];
  for (let i in wsConnections) {
    clientList.push({id: wsConnections[i].ws.id, name: wsConnections[i].name});
  }
  wss.broadcast(JSON.stringify({MSG_ID: CHAT_MSG_CLIENT_LIST, DATA: clientList}));
}
