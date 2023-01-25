let joystickOpts = {
    zone: document.getElementById("joystickL"),
    mode: "static",
    color: "red",
    size: 130,
    shape: "circle",
    restOpacity: 255,
    lockX: true,
    lockY: true,
}

let gaugeOpts = {
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