var DevReading = require('../models/deviceReadingModel');
var bodyParser = require('body-parser');

module.exports = function(app) {
    
    app.use(bodyParser.json());
    app.use(bodyParser.urlencoded({ extended: true }));
    
    app.get('/api/readings/:devName', function(req, res) {
        
        DevReading.find({ deviceName: req.params.devName }, function(err, led) {
            if (err) throw err;
            
            res.send(led);
        });
        
    });
    
    app.get('/api/reading/:id', function(req, res) {
       
        DevReading.findById({ _id: req.params.id }, function(err, led) {
           if (err) throw err;
           
           res.send(led);
       });
        
    });
    
    app.post('/api/reading', function(req, res) { 
        
        if (req.body.id) {
            DevReading.findByIdAndUpdate(req.body.id, { 
                volt: req.body.volt
            }, function(err, reading) {
                if (err) throw err;
                
                res.send('Update Success');
            });
        }
        
        else {
           
            var newDevReading = DevReading ({
               deviceName: 'test',
               volt: req.body.volt
           });
            newDevReading.save(function(err) {
               if (err) throw err;
               res.send('Reading Post Success');
           });
            
        }
        
    });
    
    app.delete('/api/reading', function(req, res) {
        
        DevReading.findByIdAndRemove(req.body.id, function(err) {
            if (err) throw err;
            res.send('del Success');
        })
        
    });


    app.use(function(req, res, next){
        console.log('in mdware');
        if (req.is('text/*')) {
          req.text = '';
          req.setEncoding('utf8');
          req.on('data', function(chunk){ req.text += chunk });
          req.on('end', next);
        } else {
          next();
        }
      });
      
      app.post('/api/text', function(req, res){
          console.log('in text');
        res.send('got "' + req.text + '"');
      });
    
}