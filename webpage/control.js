$(document).ready(function() {
    // Inicializa el joystick X-Y
    const joystick1 = nipplejs.create({
        zone: document.getElementById('joystick1'), // Zona donde se inicializa
        mode: 'static', // Fijo en su contenedor
        position: { left: '50%', top: '50%' }, // Centrado dentro del círculo
        color: 'blue', // Color del joystick
    });

    // Inicializa el joystick Z
    const joystick2 = nipplejs.create({
        zone: document.getElementById('joystick2'),
        mode: 'static',
        position: { left: '50%', top: '50%' },
        color: 'red',
    });

    // Evento para el joystick X-Y
    joystick1.on('move', function(evt, data) {
        if (data) {
            const x = data.vector.x.toFixed(2); // Movimiento en X
            const y = data.vector.y.toFixed(2); // Movimiento en Y
            console.log(`Joystick X-Y -> X: ${x}, Y: ${y}`);
        }
    });

    // Evento para el joystick Z
    joystick2.on('move', function(evt, data) {
        if (data) {
            const z = data.distance.toFixed(2); // Movimiento en Z (distancia)
            console.log(`Joystick Z -> Distance: ${z}`);
        }
    });
});
