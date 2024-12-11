#ifndef WEB_H
#define WEB_H

#include "html510.h"
#include "robot.h"
extern HTML510Server h;
const char body[] PROGMEM = R"===(
<!DOCTYPE html>
<html>
  <head>
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
    </style>
  </head>
  
  <body>
    <div class="control-panel">
      <input type="range" min="0" max="100" value="50" id="SpeedSlider">
      <span id="SpeedLabel">Speed (%): 50</span> <br>
      
      <div class="direction-buttons">
        <button id="forward" onmousedown="directionPress(1)" onmouseup="directionRelease(1)">Forward</button>
        <button id="backward" onmousedown="directionPress(2)" onmouseup="directionRelease(2)">Backward</button>
        <button id="left" onmousedown="directionPress(3)" onmouseup="directionRelease(3)">Left</button>
        <button id="right" onmousedown="directionPress(4)" onmouseup="directionRelease(4)">Right</button>
      </div>
    </div>
    
    <script>
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
      
      // Function to Handle Button Press
      function directionPress(direction) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            console.log(direction + " pressed");
          }
        };
        var req = "press?val=" + direction;
        xhttp.open("GET", req, true);
        xhttp.send();
      }
      
      // Function to Handle Button Release
      function directionRelease(direction) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            console.log(direction + " released");
          }
        };
        var req = "release?val=" + direction;
        xhttp.open("GET", req, true);
        xhttp.send();
      }

      // Map keys to directions
      const keyDirectionMap = {
        "w": 1, // Forward
        "s": 2, // Backward
        "a": 3, // Left
        "d": 4  // Right
      };

      // Track pressed keys to prevent duplicate events
      const activeKeys = {};

      // Add event listeners for keydown and keyup
      document.addEventListener("keydown", (event) => {
        const key = event.key.toLowerCase();
        if (key in keyDirectionMap && !activeKeys[key]) {
          activeKeys[key] = true; // Mark the key as active
          directionPress(keyDirectionMap[key]);
        }
      });

      document.addEventListener("keyup", (event) => {
        const key = event.key.toLowerCase();
        if (key in keyDirectionMap) {
          activeKeys[key] = false; // Mark the key as inactive
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
#endif