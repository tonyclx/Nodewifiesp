var mongoose = require('mongoose');

var Schema = mongoose.Schema;

var ledOnOffSchema = new Schema({
    deviceName: String,
    ledOnOff: Boolean
});

var LedOnOff = mongoose.model('LedOnOff', ledOnOffSchema);

module.exports = LedOnOff;