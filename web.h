#ifndef WEB_H
#define WEB_H
#include "config.h"
#include "html510.h"
#include "robot.h"
extern HTML510Server h;
const char body[] PROGMEM = R"===(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Control Panel</title>
    <style>
      /* Center the control panel */
      .control-panel {
        display: flex;
        flex-direction: column;
        align-items: center;
        margin-top: 20px;
      }
      
      /* Arrange buttons in a directional layout */
      .direction-buttons {
        display: grid;
        grid-template-areas: 
          ". forward ."
          "left . right"
          ". backward .";
        gap: 10px;
        margin-top: 10px;
      }
      
      .direction-buttons button {
        padding: 10px 20px;
        font-size: 16px;
      }
      
      #forward { grid-area: forward; }
      #backward { grid-area: backward; }
      #left { grid-area: left; }
      #right { grid-area: right; }
      
      .input-fields {
        margin-top: 20px;
        display: flex;
        flex-direction: column;
        align-items: center;
      }
      
      .input-fields input {
        margin: 5px 0;
        padding: 8px;
        font-size: 14px;
        width: 200px;
      }

      .status-display {
        text-align: center;
        font-family: Arial, sans-serif;
        margin-bottom: 20px;
      }

      .action-buttons {
        margin-top: 20px;
        display: flex;
        gap: 10px;
      }

      .action-buttons button {
        padding: 10px 20px;
        font-size: 16px;
      }

      /* Style the canvas container */
      .canvas-container {
        display: flex;
        flex-direction: column;
        align-items: center;
        margin-top: 20px;
        border: 1px solid #ccc;
        padding: 10px;
      }

      #coordCanvas {
        border: 1px solid #000;
      }

      .angle-selector {
        margin-top: 20px;
      }

      .submit-button {
        margin-top: 10px;
      }
    </style>
  </head>
  
  <body>
    <div class="status-display">
      <p id="state">State: -- | Health: -- | Left RPM: -- | Right RPM: --</p>
      <p id="speed">Left Speed: -- | Right Speed: -- | Bearing: --</p>
      <p id="position">x: -- | y: -- | Forward Distance: -- | Rightward Distance: --</p>
    </div>

    <div class="control-panel">
      <input type="range" min="0" max="100" value="50" id="SpeedSlider">
      <span id="SpeedLabel">Speed (%): 50</span> <br>
      
      <div class="direction-buttons">
        <button id="forward" onmousedown="directionPress(1)" onmouseup="directionRelease(1)">Forward</button>
        <button id="backward" onmousedown="directionPress(2)" onmouseup="directionRelease(2)">Backward</button>
        <button id="left" onmousedown="directionPress(3)" onmouseup="directionRelease(3)">Left</button>
        <button id="right" onmousedown="directionPress(4)" onmouseup="directionRelease(4)">Right</button>
      </div>
      
      <div class="input-fields">
        <input type="number" id="angleInput" placeholder="Angle (degrees)">
      </div>

      <div class="canvas-container">
        <!-- Rectangle with 2:3 ratio (height:width), for example:
             width = 300px, height = 200px -->
        <canvas id="coordCanvas" width="300" height="200"></canvas>
        <p>Click inside the rectangle to set new_x, new_y</p>
      </div>

      <div class="submit-button">
        <button id="enterButton" onclick="onEnter()">Enter</button>
      </div>
    </div>

    <div class="action-buttons">
      <button id="startAttack" onclick="startAttack()">Start Attack</button>
      <button id="stopAttack" onclick="stopAttack()">Stop Attack</button>
    </div>

    <div class="bottom-right-buttons">
      <style>
        .bottom-right-buttons {
          position: absolute;
          bottom: 20px;
          right: 20px;
          display: grid;
          grid-template-columns: repeat(3, auto);
          gap: 10px;
        }

        .bottom-right-buttons button {
          padding: 10px 15px;
          font-size: 14px;
        }
      </style>
      <button onclick="sendAuton(0)">Attack Red Nexus</button>
      <button onclick="sendAuton(2)">Attack Red Tower</button>
      <button onclick="sendAuton(4)">Attack Red Upper Tower</button>
      <button onclick="sendAuton(6)">Wall Follow</button>
      <button onclick="sendAuton(-4)">Attack Nearest</button>
      <button onclick="sendAuton(7)">Wall Follow Dumb</button>
      <button onclick="sendAuton(1)">Attack Blue Nexus</button>
      <button onclick="sendAuton(3)">Attack Blue Tower</button>
      <button onclick="sendAuton(5)">Attack Blue Upper Tower</button>
    </div>

    <script>
      function sendAuton(id) {
        var req = "";
        if (id == -4) {
          req = "attackClosest";
        } else {
          req = "attackStructure?val=" + id;
        }
        console.log(req);
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", req, true);
        xhttp.send();
      }

      var state = 0;
      var health = 100;
      var leftRPM = 0;
      var rightRPM = 0;
      var lSpeed = 0;
      var rSpeed = 0;
      var bearing = 0;
      var x = 0;
      var y = 0;
      var forwardDistance = 0;
      var rightwardDistance = 0;
      // Update Speed Slider
      document.getElementById("SpeedSlider").oninput = function() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          document.getElementById("SpeedLabel").innerHTML = this.responseText || "Speed (%): " + document.getElementById("SpeedSlider").value;
        };
        var str = "speed?val=";
        var req = str.concat(this.value);
        xhttp.open("GET", req, true);
        xhttp.send();
      }

      function startAttack() {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "startAttack", true);
        xhttp.send();
      }

      function stopAttack() {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "stopAttack", true);
        xhttp.send();
      }

      function updateState() {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "updateState", true);
        xhttp.send();
        xhttp.onreadystatechange = function() {
          var res = this.responseText;
          var vals = res.split(",");
          state = vals[0];
          health = vals[1];
          leftRPM = vals[2];
          rightRPM = vals[3];
          lSpeed = vals[4];
          rSpeed = vals[5]; 
          bearing = vals[6];
          x = vals[7];
          y = vals[8];
          forwardDistance = vals[9];
          rightwardDistance = vals[10];
        };
        document.getElementById("state").innerHTML = `State: ${state} | Health: ${health} | Left RPM: ${leftRPM} | Right RPM: ${rightRPM}`;
        document.getElementById("speed").innerHTML = `Left Speed: ${lSpeed} | Right Speed: ${rSpeed} | Bearing: ${bearing}`;
        document.getElementById("position").innerHTML = `x: ${x} | y: ${y} | Forward Distance: ${forwardDistance} | Rightward Distance: ${rightwardDistance}`;
      }

      setInterval(updateState, 200);
      setInterval(drawScene, 200);
      
      // Function to Handle Button Press
      function directionPress(direction) {
        var xhttp = new XMLHttpRequest();
        activeKeys[directionKeyMap[direction]] = 1; 
        var req = "press?val=";
        // if (activeKeys["w"] == 1) req += "w";
        // if (activeKeys["a"] == 1) req += "a";
        // if (activeKeys["s"] == 1) req += "s";
        // if (activeKeys["d"] == 1) req += "d";
        req += direction;
        console.log(req);
        xhttp.open("GET", req, true);
        xhttp.send();
      }
      
      // Function to Handle Button Release
      function directionRelease(direction) {
        var xhttp = new XMLHttpRequest();
        activeKeys[directionKeyMap[direction]] = 0; 
        var req = "release?val=";
        // if (activeKeys["w"] == 1) req += "w";
        // if (activeKeys["a"] == 1) req += "a";
        // if (activeKeys["s"] == 1) req += "s";
        // if (activeKeys["d"] == 1) req += "d";
        req += direction;
        console.log(req);
        xhttp.open("GET", req, true);
        xhttp.send();
      }

      // Function to send nav request
      function onEnter() {
        const angle = document.getElementById("angleInput").value;
        if (angle !== "" && new_x !== null && new_y !== null) {
          var xhttp = new XMLHttpRequest();
          console.log(`nav?val=${new_x},${new_y},${angle}`);
          xhttp.open("GET", `nav?val=${new_x},${new_y},${angle}`, true);
          xhttp.send();
        }
      }
      // Map keys to directions
      const keyDirectionMap = {
        "w": 1, // Forward
        "s": 2, // Backward
        "a": 3, // Left
        "d": 4  // Right
      };
      const directionKeyMap = {
        1: "w", // Forward
        2: "s", // Backward
        3: "a", // Left
        4: "d"  // Right
      };

      // Track pressed keys to prevent duplicate events
      const activeKeys = {"w": 0, "a": 0, "s": 0, "d": 0};

      // Add event listeners for keydown and keyup
      document.addEventListener("keydown", (event) => {
        const key = event.key.toLowerCase();
        if (key in keyDirectionMap && activeKeys[key] == 0) {
          activeKeys[key] = 1; // Mark the key as active
          directionPress(keyDirectionMap[key]);
        }
      });

      document.addEventListener("keyup", (event) => {
        const key = event.key.toLowerCase();
        if (key in keyDirectionMap) {
          activeKeys[key] = 0; // Mark the key as inactive
          directionRelease(keyDirectionMap[key]);
        }
      });

      let new_x = null;
      let new_y = null;
      let canvas = document.getElementById('coordCanvas');
      let ctx = canvas.getContext('2d');

      // Draw function
      function drawScene() {
        // Clear the canvas
        ctx.clearRect(0, 0, canvas.width, canvas.height);

        // Draw the rectangle border (already has border from CSS, but we can ensure)
        ctx.strokeStyle = '#000000';
        ctx.lineWidth = 1;
        ctx.strokeRect(0, 0, canvas.width, canvas.height);

        // Get the input x,y
        var xVal = (x - 1400) / 4800.0 * canvas.width;
        var yVal = (y - 2400) / 2800.0 * canvas.height;

        // Transform coordinates: origin at top-right
        // Positive x goes left, positive y goes down
        // canvas origin (0,0) is top-left by default.
        // To get top-right as origin: x_on_canvas = canvas.width - xVal, y_on_canvas = yVal
        let dotCanvasX = canvas.width - xVal;
        let dotCanvasY = yVal;

        // Draw the blue dot
        ctx.fillStyle = 'blue';
        ctx.beginPath();
        ctx.arc(dotCanvasX, dotCanvasY, 5, 0, 2 * Math.PI);
        ctx.fill();

        // If we have a clicked location (new_x, new_y), draw a black cross
        if (new_x !== null && new_y !== null) {
          var xVal = (new_x - 1400) / 4800.0 * canvas.width;
          var yVal = (new_y - 2400) / 2800.0 * canvas.height;
          let crossCanvasX = canvas.width - xVal;
          let crossCanvasY = yVal;
          ctx.strokeStyle = 'black';
          ctx.beginPath();
          // Horizontal line of the cross
          ctx.moveTo(crossCanvasX - 5, crossCanvasY);
          ctx.lineTo(crossCanvasX + 5, crossCanvasY);
          // Vertical line of the cross
          ctx.moveTo(crossCanvasX, crossCanvasY - 5);
          ctx.lineTo(crossCanvasX, crossCanvasY + 5);
          ctx.stroke();
        }
      }

      // Handle canvas click
      canvas.addEventListener('click', function(e) {
        let rect = canvas.getBoundingClientRect();
        let clickX = e.clientX - rect.left;
        let clickY = e.clientY - rect.top;

        // Convert back from canvas coordinates to our defined system
        // canvasX = width - new_x => new_x = width - canvasX
        // canvasY = new_y => new_y = canvasY
        console.log("raw_new_x", clickX, "new_y", clickY)
        new_x = canvas.width - clickX;
        new_y = clickY;

        new_x = 1400 + 4800.0 * new_x / canvas.width;
        new_y = 2400 + 2800.0 * new_y / canvas.height;
        console.log("new_x", new_x, "new_y", new_y)

        // Redraw with new cross
      });
    </script>
  </body>
</html>
)===";
void handleRoot();
void getSpeed();
void press();
void release();
void nav();
void startAttack();
void stopAttack();
void updateState();
void attackClosest();
void attackStructure();
#endif