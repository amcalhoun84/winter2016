// Andrew M. Calhoun (calhouna@oregonstate.edu)
//   Assignment: AJAX INTERACTION
//    Date: 11/7/2015

 var apiKey = '74b347fab4f8237aed99a4f9d92e5c09';

      document.addEventListener('DOMContentLoaded', bindButtonZip);
      document.addEventListener('DOMContentLoaded', bindButtonCity);

      function bindButtonZip(){
        document.getElementById('zipSubmit').addEventListener('click', function(event){
          var req = new XMLHttpRequest();
          var payload = {zip:null};
          payload.zip = document.getElementById('zip').value + ',us';
          req.open('GET', 'http://api.openweathermap.org/data/2.5/weather?zip=' + payload.zip + '&units=imperial' + '&appid=' + apiKey, false);
          req.addEventListener('load', function(){
            if(req.status >= 200 && req.status < 400) 
            {
              var response = JSON.parse(req.responseText);        
              document.getElementById('cityName').textContent = response.name;
              document.getElementById('temps').textContent = response.main.temp;
              document.getElementById('windSpeed').textContent = response.wind.speed;
              document.getElementById('humidity').textContent = response.main.humidity;
            } 

            else 
            {
              console.log("Error in network request: " + request.statusText);
            }});
            req.send(JSON.stringify(payload));
            event.preventDefault();
      });
    }


      function bindButtonCity(){
          document.getElementById('citySubmit').addEventListener('click', function(event){
          var req = new XMLHttpRequest();
          var payload = {city:null};
          payload.city = document.getElementById('city').value + ',us';
          req.open('GET', 'http://api.openweathermap.org/data/2.5/weather?q=' + payload.city + '&units=imperial' + '&appid=' + apiKey, false);
          req.addEventListener('load', function(){
            if(req.status >= 200 && req.status < 400) 
            {
              var response = JSON.parse(req.responseText);        
              document.getElementById('cityName').textContent = response.name;
              document.getElementById('temps').textContent = response.main.temp;
              document.getElementById('windSpeed').textContent = response.wind.speed;
              document.getElementById('humidity').textContent = response.main.humidity;
            } 

            else 
            {
              console.log("Error in network request: " + request.statusText);
            }});
            req.send(JSON.stringify(payload));
            event.preventDefault();
      });
}