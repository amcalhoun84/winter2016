var express = require('express');
var handlebars = require('express-handlebars');
var session = require('expression-session');
var bodyParser = require('body-parser');

var app = express();

app.engine('handlebars');
app.set('view engine', 'handlebars');
app.set('port', 2401);

app.use(bodyParser.urlencoded({extended:false}));
app.use(bodyParser.json());

app.get('/', function(req,res)
{
	var context = {};

	var dataToDisplay = {};
	res.render('how-to');

}

app.use(function(req,res){
  res.status(404);
  res.render('404');
});

app.use(function(err, req, res, next){
  console.error(err.stack);
  res.type('plain/text');
  res.status(500);
  res.render('500');
});

app.listen(app.get('port'), function(){
		console.log('Express started on http://52.27.116.225:' + app.get('port') + '; press Ctrl-C to terminate.');