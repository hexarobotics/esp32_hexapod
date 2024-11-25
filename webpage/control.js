
$(document).ready(function() {
    // Initialize the joysticks using nipple.js
    const options = {
        zone: document.getElementById('joystick1'),
        mode: 'static',
        position: { left: '50%', top: '50%' },
        color: 'blue',
    };
    const joystick1 = nipplejs.create(options);

    const options2 = {
        zone: document.getElementById('joystick2'),
        mode: 'static',
        position: { left: '50%', top: '50%' },
        color: 'red',
    };
    const joystick2 = nipplejs.create(options2);

    joystick1.on('move', function(evt, data) {
        console.log('Joystick X-Y:', data);
    });

    joystick2.on('move', function(evt, data) {
        console.log('Joystick Z:', data);
    });
});
