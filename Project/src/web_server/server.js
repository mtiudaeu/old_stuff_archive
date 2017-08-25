var express = require('express');
var app = express();
var server = require('http').createServer(app);
server.listen(process.env.PORT || 3000, "127.0.0.1");

var bodyParser = require('body-parser');
var linux_script = require('./lib/linux_script.js');

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
  extended : false
}));
app.use('/public', express.static('public'));


app.get('/test1', function (req, res) {
  res.sendFile('views/test1_webgl/test1.html', { root: __dirname});
});
app.get('/test2', function (req, res) {
  res.sendFile('views/test2_shapes/index.html', { root: __dirname});
});


//MDTMP
var clipboard = "";
app.get('/', function(req, res){
  res.send(clipboard);
});

app.post('/', function(req, res){
  //MDTMP var ret = linux_script.request(req.body.data);
  //MDTMP res.send(ret);
  clipboard = req.body.data;
  res.end();
});


