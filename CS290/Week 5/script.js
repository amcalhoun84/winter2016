/* name: Andrew M. Calhoun
    Email: calhouna@oregonstate.edu
    Date Due: 11/2/2015
    Assignment 4: Navigating the DOM
    File: script.js
*/


function createTable() {
    var i;
    var j;
    var x = 0;
    var y = 1;
    var body = document.getElementsByTagName('body')[0];
    var table = document.createElement('table');
    table.style.width = '75%';
    table.setAttribute('border', '1');
    var tBody = document.createElement('tbody');
    for (i = 0; i < 4; i++) {
        var tr = document.createElement('tr');
        for (j = 1; j < 5; j++) {
                if(i === 0)
                {
                	var th = document.createElement('th');
                	th.appendChild(document.createTextNode("Header " + j));
                	tr.appendChild(th);		
                }
                else 
                {
	                var current_cell = document.createElement('td');
	                current_cell.appendChild(document.createTextNode(i + ", " + j));
	                tr.appendChild(current_cell);
        		}
		}
        tBody.appendChild(tr);
    }
    table.appendChild(tBody);



    var elements = document.getElementById('td');
    table.children[0].children[y].children[x].style.borderWidth = "medium";

    var btnUp = document.createElement("BUTTON");
    var bUpTxt = document.createTextNode("UP");
    btnUp.appendChild(bUpTxt);
    btnUp.onclick = function() { 

        y = y - 1;
        if(y < 1)
        {
            y = 1;
            console.log("Cannot go farther");
        }


        table.children[0].children[y].children[x].style.borderWidth = 'medium'; 
        table.children[0].children[y+1].children[x].style.borderWidth = 'thin';

    }

    var btnDown = document.createElement("BUTTON");
    var bDwnTxt = document.createTextNode("DOWN");
    btnDown.appendChild(bDwnTxt);
    btnDown.onclick = function() { 

        y = y + 1;
        if (y > 3)
        {
            y = 3;
            console.log("Cannot go farther.");
        }

        
        table.children[0].children[y].children[x].style.borderWidth = 'medium'; 
        table.children[0].children[y-1].children[x].style.borderWidth = 'thin';
    }


    var btnMark = document.createElement("BUTTON");
    var bMrkTxt = document.createTextNode("MARK CELL");
    btnMark.appendChild(bMrkTxt);
    btnMark.onclick = function() { table.children[0].children[y].children[x].style.backgroundColor = "yellow";
    }


    var btnLft = document.createElement("BUTTON");
    var bLftTxt = document.createTextNode("LEFT");
    btnLft.appendChild(bLftTxt);
    btnLft.onclick = function() { 
    
    x = x - 1;
    if(x < 0)
    {
        x = 0;
        console.log("Cannot go farther.");
    }

    table.children[0].children[y].children[x].style.borderWidth = "medium";
    table.children[0].children[y].children[x+1].style.borderWidth = "thin";

    }

    var btnRgt = document.createElement("BUTTON");
    var bRgtTxt = document.createTextNode("RIGHT");
    btnRgt.appendChild(bRgtTxt);
    btnRgt.onclick = function() { 
    
    x = x + 1;
    if(x > 3)
    {
        x = 3;
        console.log("Cannot go farther.");
    }

    table.children[0].children[y].children[x].style.borderWidth = "medium";
    table.children[0].children[y].children[x-1].style.borderWidth = "thin";

    }

    body.appendChild(table);
    body.appendChild(btnUp);
    body.appendChild(btnDown);
    body.appendChild(btnMark);
    body.appendChild(btnLft);
    body.appendChild(btnRgt); 

}
 