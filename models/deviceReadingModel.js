var mongoose = require('mongoose');

var Schema = mongoose.Schema;

var devReadingSchema = new Schema({
    deviceName: String,
    volt: String
});

var DevReadingSchema = mongoose.model('DevReadingSchema', devReadingSchema);

module.exports = DevReadingSchema;