var opts = {
    angle: 0.09, // The span of the gauge arc
    lineWidth: 0.44, // The line thickness
    radiusScale: 1, // Relative radius
    pointer: {
        length: 0.7, // // Relative to gauge radius
        strokeWidth: 0.035, // The thickness
        color: '#000000' // Fill color
    },
    limitMax: true,     // If false, max value increases automatically if value > maxValue
    limitMin: false,     // If true, the min value of the gauge will be fixed
    colorStart: '#6FADCF',   // Colors
    colorStop: '#e70008',    // just experiment with them
    strokeColor: '#E0E0E0',  // to see which ones work best for you
    generateGradient: true,
    highDpiSupport: true,     // High resolution support
    renderTicks: {
        divisions: 5,
        divWidth: 1.1,
        divLength: 0.7,
        divColor: '#333333',
        subDivisions: 3,
        subLength: 0.5,
        subWidth: 0.6,
        subColor: '#136650'
    },
    staticZones: [
        {strokeStyle: "rgb(255,95,95)", min: 0, max: 40, height: 1.4},
        {strokeStyle: "rgb(12,194,173)", min: 40, max: 300, height: 1.4},
    ],
    staticLabels: {
        font: "10px sans-serif",  // Specifies font
        labels: [40, 300],  // Print labels at these values
        color: "#ff580c",  // Optional: Label text color
        fractionDigits: 0  // Optional: Numerical precision. 0=round off.
    },
};
var target = document.getElementById('gauge'); // your canvas element
var gauge = new Gauge(target).setOptions(opts); // create sexy gauge!
gauge.setTextField(document.getElementById("value"), 0); // second parameter is decimal points
gauge.maxValue = 300; // set max gauge value
gauge.setMinValue(0);  // Prefer setter over gauge.minValue = 0
gauge.animationSpeed = 32; // set animation speed (32 is default value)
gauge.set(0); // set actual value

function updateCounter() {
    fetch("/rpm").then(response => response.text()).then(RPM => {
        console.log(RPM);
        gauge.set(parseInt(RPM));
    });
}
setInterval(updateCounter, 300);