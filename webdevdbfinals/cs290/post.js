document.addEventListener('DOMContentLoaded', bindButton);

function bindButton()
{
document.getElementById('urlSubmit').addEventListener('click', function(event){
    var req = new XMLHttpRequest();
    var payload = { name:null, 
                    email:null, 
                    url:null, 
                    color:null };
    payload.name = document.getElementById('name').value;
    payload.email = document.getElementById('email').value;
    payload.url = document.getElementById('url').value;
    payload.color = document.getElementById('color').value;
    req.open('POST', 'http://httpbin.org/post', true);
    req.setRequestHeader('Content-Type', 'application/json');
    req.addEventListener('load',function(){
      if(req.status >= 200 && req.status < 400){
        var response = JSON.parse(req.status);
        var inputText = JSON.parse(req.responseText);

        alert(inputText.data);
        document.getElementById('status').textContent = response;    
        document.getElementById('datadump').textContent = inputText.data;
        
        // alert(response);
        // console.log(response);
      } else {
        console.log("Error in network request: " + req.statusText);
      }});
    req.send(JSON.stringify(payload));  
    event.preventDefault();
  });
}