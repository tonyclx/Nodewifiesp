var express = require('express');
var app = express();
var mongoose = require('mongoose');
var config = require('./config');
var DeviceReadingController = require('./controllers/DeviceReadingController');
var LedOnOffController = require('./controllers/LedOnOffController');



var port = process.env.PORT || 3000;

app.use('/', express.static(__dirname + '/public'));

app.set('view engine', 'ejs');

mongoose.connect(config.getDbConnectionString());
setupController(app);
DeviceReadingController(app);
LedOnOffController(app);

apiController(app);

app.listen(port);