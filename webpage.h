const char MAIN_page[] PROGMEM = R"=====(

<!DOCTYPE HTML>
<html lang="en">

<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <script src="https://cdn.jsdelivr.net/npm/@jaames/iro@5"></script>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" rel="stylesheet"
        integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC" crossorigin="anonymous">

    <style>
        #circle {
            position: relative;
            width: 400px;
            height: 400px;
            border-radius: 50%;
            background: conic-gradient(#fff, #fff)
        }

        #circle-in {
            position: absolute;
            top: 35px;
            left: 35px;
            width: 330px;
            height: 330px;
            border-radius: 50%;
            background: #333;
        }

        #picker {
            position: absolute;
            top: 50%;
            left: 50%;
            height: 50px;
            margin-top: -25px;
            width: 50%;
            pointer-events:none;
            touch-action: none;
            -webkit-transform-origin: center left;
            -moz-transform-origin: center left;
            -ms-transform-origin: center left;
            -o-transform-origin: center left;
            transform-origin: center left;
        }

        #picker-circle {
            width: 50px;
            height: 50px;
            border-radius: 50%;
            border-color: black;
            border: 3px;
            background: #fff;
            margin: 0 -8px 0 auto;
            cursor: move;
            pointer-events:all;
            touch-action:auto;
        }

    </style>

    <title>Jelle's Lamp Controller</title>
</head>

)=====";
const char MAIN_page0[] PROGMEM = R"=====(

<body style = "background-color: #333;">

    <div >
        <ul class="nav nav-justified nav-pills" id="myTab" role="tablist">
            <li class="nav-item" role="presentation">
                <button class="nav-link " id="white-tab" data-bs-toggle="tab" data-bs-target="#white" type="button"
                    role="tab" aria-controls="white" aria-selected="true" style= "height: 75px;"><h1>WHITE</h1></button>
            </li>
            <li class="nav-item" role="presentation">
                <button class="nav-link active" id="RGB-tab" data-bs-toggle="tab" data-bs-target="#RGB" type="button"
                    role="tab" aria-controls="RGB" aria-selected="false"style= "height: 75px;"><h1>RGB</h1></button>
            </li>
        </ul>
        <div class="tab-content" id="myTabContent">
            <div class="tab-pane fade " id="white" role="tabpanel" aria-labelledby="home-tab">
                <div class="row mx-auto" style="width: 400px; margin-top: 50px">
                    <div id="whitepicker" style = "padding-left: 25px; padding-right: 25px;"></div>
                </div>
                <div class = "row mx-auto" style ="margin-top: 50px; max-width: 200px;">
                    <button type="button" class="btn btn-primary" onclick="SendWhiteBytes()" >Send Data</button>
                </div>
                
            </div>
            <div class="tab-pane fade show active " id="RGB" role="tabpanel" aria-labelledby="profile-tab">
                <div class="row mx-auto" style="width: 400px; margin-top: 50px">
                    <div id="circle">
                        <div id="circle-in">
                            <div id="RGBpicker" style="margin: 40px;"></div>
                        </div>
                        <div id="picker">
                            <div id="picker-circle" class="border"></div>
                        </div>
                    </div>
                </div>
                <div class = "row mx-auto" style ="margin-top: 50px; max-width: 200px;">
                    <button type="button" class="btn btn-primary" onclick="removeActivePicker()" >Remove Color</button>

                </div>
                <div class = "row mx-auto" style ="margin-top: 50px; max-width: 200px;">
                    <button type="button" class="btn btn-primary" onclick="SendRGBBytes()" >Send Data</button>

                </div>
            </div>
        </div>
    </div>

)=====";

const char MAIN_page1[] PROGMEM = R"=====(

<!-- white stuff -->
    <script>
        
        var whitecolorPicker = new iro.ColorPicker('#whitepicker', {
            layout: [
                {
                    component: iro.ui.Slider,
                    options: {
                        // can also be 'saturation', 'value', 'red', 'green', 'blue', 'alpha' or 'kelvin'
                        sliderType: 'kelvin'
                    }
                },
                {
                    component: iro.ui.Slider,
                    options: {
                        // can also be 'saturation', 'value', 'red', 'green', 'blue', 'alpha' or 'kelvin'
                        sliderType: 'value'
                    }
                },
                
            ],
            margin: 50,
            width: 350,
        });

        function SendWhiteBytes(){
            var whitebytes = [];

            whitebytes.push(Math.round(whitecolorPicker.color.value * 10.23));
            tempValue = Math.max(2200,Math.min(11000,whitecolorPicker.color.kelvin))
            whitebytes.push(Math.floor(map_range(tempValue, 2200, 11000, 0, 1024)));

            const xhttp = new XMLHttpRequest();
            xhttp.open("GET", whitebytes + "/W", true);
            xhttp.send();

        };

        function map_range(value, low1, high1, low2, high2) {
            return low2 + (high2 - low2) * (value - low1) / (high1 - low1);
        }
    </script>

<!-- rgbstuff -->
    <script>
        class Picker{
            constructor(element, rotation, color){
                this.element = element;
                this.rotation = rotation;
                this.color = color;
            }
        };

        var circle = document.getElementById('circle');
        var picker = document.getElementById('picker');

        var pickerCircleArray = [new Picker(picker.firstElementChild, 0, "#ffffff")];
        var activePicker = null;
        var activePickerCircle = pickerCircleArray[0];
        var rect = circle.getBoundingClientRect();

        var center = {
                x: rect.left + rect.width / 2,
                y: rect.top + rect.height / 2
            };

        var transform = (function () {
                var prefs = ['t', 'WebkitT', 'MozT', 'msT', 'OT'],
                    style = document.documentElement.style,
                    p
                for (var i = 0, len = prefs.length; i < len; i++) {
                    if ((p = prefs[i] + 'ransform') in style) return p
                }

                alert('your browser doesnt support css transforms!')
        })();

        var mouseDown = false;
        document.body.onmousedown = function() { 
            mouseDown = true;
            
            event.preventDefault()
            document.body.style.cursor = 'move'
            checkClickElement();
        }
        document.body.onmouseup = function() {
            mouseDown = false;
            document.body.style.cursor = null;
            activePicker = null;
        }
        document.body.onmousemove = function(){
            movePicker();
        }

        document.body.ontouchstart = function() { 
            mouseDown = true;
            
            event.preventDefault()
            document.body.style.cursor = 'move'
            checkClickElement();
        }
        document.body.ontouchend = function() {
            mouseDown = false;
            document.body.style.cursor = null;
            activePicker = null;
        }
        document.body.ontouchmove = function(){
            movePicker();
        }

        function rotate (x, y) 
        {
            var deltaX = x - center.x,
                deltaY = y - center.y;

            return (Math.atan2(deltaY, deltaX) * 180 / Math.PI +360) % 360
        };

)=====";

const char MAIN_page2[] PROGMEM = R"=====(

        function checkClickElement()
        {
            //get the element that is clicked
            var targ;
            if (!e) {
                var e = window.event;
            }
            if (e.target) {
                targ = e.target;
            } 
            else if (e.srcElement) {
                targ = e.srcElement;
            }
            //if clicked on a empty spot
            if(targ.id == "circle"){
                addPicker();
            }
            else if(targ.id == "picker-circle"){
                console.log("setting new active element");
                activePickerCircle = getPickerByElement(targ);
                activePicker = targ.parentNode;
            }
        }

        function addPicker()
        {
            // add a new picker element
            var newPicker = document.createElement("div");
            newPicker.id = "picker";
            var newPickerCircle = document.createElement("div");
            newPickerCircle.id = "picker-circle";
            newPickerCircle.className = "border";
            newPicker.appendChild(newPickerCircle);
            circle.appendChild(newPicker);

            //set the new active color
            activePicker = newPicker;
            activePickerCircle = new Picker(newPickerCircle,0,"#ffffff");
            pickerCircleArray.push(activePickerCircle);
            console.log("You added a picker, there are now: " + pickerCircleArray.length + " pickers");
            movePicker(window.event);

            rgbcolorPicker.color.hexString = activePickerCircle.color;
        };

        function removeActivePicker()
        {
            //remove the picker element
            if(activePickerCircle){
                
                for( var i = 0; i < pickerCircleArray.length; i++){ 
                    if ( pickerCircleArray[i] == activePickerCircle) { 
                        pickerCircleArray.splice(i, 1); 
                    }
                }
                activePickerCircle.element.parentNode.remove();
                console.log("there are still: " + pickerCircleArray.length + " pickers active");
            } 
            var newactivePickerCircleParent = document.getElementById('picker');
            if(newactivePickerCircleParent) activePickerCircle = getPickerByElement(newactivePickerCircleParent.firstElementChild);
        };

        function movePicker(event){
            if(!mouseDown || !activePicker) return;
            
            event = event || window.event;
            var rot = rotate(event.pageX, event.pageY);
            activePicker.style[transform] = 'rotate(' + rot + 'deg)';
            activePickerCircle.rotation = rot;
            updatePickerOrder();
        }

        function updatePickerOrder(){
            pickerCircleArray.sort((a, b) => (a.rotation > b.rotation) ? 1 : -1)

            var startAngle = 0;
            var gradientText = "conic-gradient(";
            for( var i = 0; i < pickerCircleArray.length; i++){ 
                if(i==0){
                    startAngle = (90 + pickerCircleArray[i].rotation)
                    gradientText += "from " + startAngle + "deg,";
                } 
                gradientText += " " + pickerCircleArray[i].color;
                gradientText += " " + (pickerCircleArray[i].rotation + 90 - startAngle) + "deg,";
            }
            gradientText += " " + pickerCircleArray[0].color;

            gradientText += ")";
            circle.style.background = gradientText;
        }


)=====";

const char MAIN_page3[] PROGMEM = R"=====(

        function getPickerByElement(element){

            for( var i = 0; i < pickerCircleArray.length; i++){ 
                if ( pickerCircleArray[i].element == element) { 
                    activePickerCircle = pickerCircleArray[i];
                    rgbcolorPicker.color.hexString = pickerCircleArray[i].color;
                    return pickerCircleArray[i];
                }
            }
        }

                var rgbcolorPicker = new iro.ColorPicker('#RGBpicker', {
            layout: [
                {
                    component: iro.ui.Wheel,
                },
            ],
            margin: 100,
            width: 250,
        });
        rgbcolorPicker.on('input:change', function(color) {
            // log the current color as a HEX string
            activePickerCircle.color = color.hexString;
            activePickerCircle.element.style.backgroundColor = color.hexString;
            updatePickerOrder();
        });

        function SendRGBBytes(){
          var colorbytes = [];

          for( var i = 0; i < pickerCircleArray.length; i++){ 
              var pos = Math.round(pickerCircleArray[i].rotation * 255 / 360.0);
              var colors = hexToHSL(pickerCircleArray[i].color);
              colorbytes.push(pos);
              colorbytes.push(Math.round(colors[0] * 255 /360));
              colorbytes.push(Math.round(colors[1] * 255));
              colorbytes.push(Math.round(colors[2] * 255));
          }
          const xhttp = new XMLHttpRequest();
          xhttp.open("GET", colorbytes + "/C", true);
          xhttp.send();

        };

        function hexToHSL(hex) {
            var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
            r = parseInt(result[1], 16)/255.0;
            g = parseInt(result[2], 16)/255.0;
            b = parseInt(result[3], 16)/255.0;
            let v=Math.max(r,g,b), c=v-Math.min(r,g,b);
            let h= c && ((v==r) ? (g-b)/c : ((v==g) ? 2+(b-r)/c : 4+(r-g)/c)); 
            return [60*(h<0?h+6:h), v&&c/v, v];
        }

    </script>

    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.bundle.min.js"
        integrity="sha384-MrcW6ZMFYlzcLA8Nl+NtUVF0sA7MsXsP1UyJoMp4YLEuNSfAP+JcXn/tWtIaxVXM"
        crossorigin="anonymous"></script>
</body>

</html>

)=====";

