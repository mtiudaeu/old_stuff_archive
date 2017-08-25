var fs = require('fs'),
    https = require('https'),
    express = require('express'),
    app = express();

https.createServer({
                     key : fs.readFileSync('key.pem'),
                     cert : fs.readFileSync('cert.pem')
                   },
                   app).listen(8888);

app.get('/', function (req, res) {
  console.log( 'GET : / ' );
  res.sendFile(__dirname + '/public/index.html');
});

