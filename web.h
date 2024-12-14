#ifndef WEB_H
#define WEB_H

#include "html510.h"
#include "robot.h"
extern HTML510Server h;
const char body[] PROGMEM = R"===(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
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
        <input type="number" id="xInput" placeholder="X Coordinate">
        <input type="number" id="yInput" placeholder="Y Coordinate">
        <input type="number" id="angleInput" placeholder="Angle (degrees)">
      </div>
    </div>

    <div class="action-buttons">
      <button id="startAttack" onclick="startAttack()">Start Attack</button>
      <button id="stopAttack" onclick="stopAttack()">Stop Attack</button>
    </div>

    <script>
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
          console.log(vals);
        };
        document.getElementById("state").innerHTML = `State: ${state} | Health: ${health} | Left RPM: ${leftRPM} | Right RPM: ${rightRPM}`;
        document.getElementById("speed").innerHTML = `Left Speed: ${lSpeed} | Right Speed: ${rSpeed} | Bearing: ${bearing}`;
        document.getElementById("position").innerHTML = `x: ${x} | y: ${y} | Forward Distance: ${forwardDistance} | Rightward Distance: ${rightwardDistance}`;
      }

      setInterval(updateState, 100);
      
      // Function to Handle Button Press
      function directionPress(direction) {
        var xhttp = new XMLHttpRequest();
        var req = "press?val=";
        if (activeKeys["w"] == 1) req += "w";
        if (activeKeys["a"] == 1) req += "a";
        if (activeKeys["s"] == 1) req += "s";
        if (activeKeys["d"] == 1) req += "d";
        console.log(req);
        xhttp.open("GET", req, true);
        xhttp.send();
      }
      
      // Function to Handle Button Release
      function directionRelease(direction) {
        var xhttp = new XMLHttpRequest();
        var req = "release?val=";
        if (activeKeys["w"] == 1) req += "w";
        if (activeKeys["a"] == 1) req += "a";
        if (activeKeys["s"] == 1) req += "s";
        if (activeKeys["d"] == 1) req += "d";
        console.log(req);
        xhttp.open("GET", req, true);
        xhttp.send();
      }

      // Function to send nav request
      function sendNavRequest() {
        const x = document.getElementById("xInput").value;
        const y = document.getElementById("yInput").value;
        const angle = document.getElementById("angleInput").value;
        if (x !== "" && y !== "" && angle !== "") {
          var xhttp = new XMLHttpRequest();
          xhttp.open("GET", `nav?val=${x},${y},${angle}`, true);
          xhttp.send();
        }
      }

      // Add event listeners to text fields
      ["xInput", "yInput", "angleInput"].forEach(id => {
        document.getElementById(id).addEventListener("keydown", function(event) {
          if (event.key === "Enter") {
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "nav", true);
            xhttp.send();
          }
        });
      });
      // Map keys to directions
      const keyDirectionMap = {
        "w": 1, // Forward
        "s": 2, // Backward
        "a": 3, // Left
        "d": 4  // Right
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
#endif