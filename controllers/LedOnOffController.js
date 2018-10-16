var LedOnOff = require('../models/ledOnOffModel');
var bodyParser = require('body-parser');

module.exports = function (app) {

    app.use(bodyParser.json());
    app.use(bodyParser.urlencoded({ extended: true }));

    app.get('/api/lights/:devName', function (req, res) {

        LedOnOff.find({ deviceName: req.params.devName }, function (err, lights) {
            if (err) throw err;

            res.send(lights);
        });

    });

    app.get('/api/light/:id', function (req, res) {

        LedOnOff.findById({ _id: req.params.id }, function (err, light) {
            if (err) throw err;

            res.send(light);
        });

    });

    app.post('/api/light', function (req, res) {

        if (req.body.id) {
            LedOnOff.findByIdAndUpdate(req.body.id, 
                { ledOnOff: req.body.ledOnOff }
                , function (err, light) {
                if (err) throw err;

                res.send('light status update Success');
            });
        }

        else {

            var newLedOnOff = LedOnOff({
                deviceName: 'test',
                ledOnOff: req.body.ledOnOff
            });
            newLedOnOff.save(function (err) {
                if (err) throw err;
                res.send('light post Success');
            });

        }

    });

    app.delete('/api/light', function (req, res) {

        LedOnOff.findByIdAndRemove(req.body.id, function (err) {
            if (err) throw err;
            res.send('light del Success');
        })

    });

}