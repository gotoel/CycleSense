let styler = new PseudoStyler();
var gauge;
var joystickL;
var lastRpm = 0;

var button_c;
var button_z;

configureButtons();
configureGauge();
configureJoystick();

function configureButtons() {
    button_c = document.getElementById("key_c");
    button_z = document.getElementById("key_z");
}

function configureJoystick() {
    joystickOpts.color = "red";
    joystickL = nipplejs.create(joystickOpts);
}

function configureGauge() {
    var target = document.getElementById('gauge'); // your canvas element
    gauge = new Gauge(target).setOptions(gaugeOpts); // create sexy gauge!
    gauge.setTextField(document.getElementById("value"), 0); // second parameter is decimal points
    gauge.maxValue = 300; // set max gauge value
    gauge.setMinValue(0);  // Prefer setter over gauge.minValue = 0
    gauge.animationSpeed = 32; // set animation speed (32 is default value)
    gauge.set(0);
}

function updateStats() {
    fetch("/stats").then(response => response.json()).then(stats => {
        //console.log(stats);

        if(stats.bike.rpm != lastRpm) {
            gauge.set(parseInt(stats.bike.rpm));
        }

        if(lastRpm != stats.bike.rpm &&
            ((lastRpm < 40 && stats.bike.rpm >= 40) || (lastRpm >= 40 && stats.bike.rpm < 40))) {
            if (joystickL != null && joystickL.length == 1) {
                joystickL[0].remove();
            }

            if(stats.bike.rpm >= 40) {
                joystickOpts.color = "#0CC2ADFF";
            } else {
                joystickOpts.color = "red";
            }

            joystickL = nipplejs.create(joystickOpts);
        }

        if (joystickL != null && joystickL.length == 1) {
            let joystickAxisX, joystickAxisY;
            joystickAxisX = ((stats.chuck.sticks.axis_left_x) * (100) / 255) - 50;
            joystickAxisY = ((stats.chuck.sticks.axis_left_y) * (100) / 255) - 50;

            joystickL[0].setPosition(null, {x: joystickAxisX, y: -joystickAxisY});
            joystickL[0].options.color = "red";
            //console.log(joystickL[0]);
        }

        button_c.className = (stats.chuck.buttons.button_c || stats.chuck.buttons.button_a) ?
            (stats.bike.rpm >= 40 ? "toggled" : "toggled-disabled") : "";
        button_z.className = (stats.chuck.buttons.button_z  || stats.chuck.buttons.button_b) ?
            (stats.bike.rpm >= 40 ? "toggled" : "toggled-disabled") : "";
        lastRpm = stats.bike.rpm;
    });
}


setInterval(updateStats, 100);


