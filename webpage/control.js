$(document).ready(function () {
    // Inicialización del joystick X-Y
    const joystick1 = nipplejs.create({
        zone: document.getElementById('joystick1'),
        mode: 'static',
        position: { left: '50%', top: '50%' },
        color: 'blue',
    });

    // Inicialización del joystick Z
    const joystick2 = nipplejs.create({
        zone: document.getElementById('joystick2'),
        mode: 'static',
        position: { left: '50%', top: '50%' },
        color: 'red',
    });

    // Evento para el joystick X-Y
    joystick1.on('move', function (evt, data) {
        if (data && data.vector) { // Verificar que data y data.vector existen
            const x = data.vector.x.toFixed(2);
            const y = data.vector.y.toFixed(2);
            console.log(`Joystick X-Y -> X: ${x}, Y: ${y}`);
        } else {
            console.warn('Joystick X-Y: Datos de movimiento incompletos', data);
        }
    });

    joystick1.on('end', function () {
        console.log('Joystick X-Y -> Liberado');
    });

    // Evento para el joystick Z
    joystick2.on('move', function (evt, data) {
        if (data && data.distance !== undefined) { // Verificar que data y data.distance existen
            const z = data.distance.toFixed(2);
            console.log(`Joystick Z -> Distance: ${z}`);
        } else {
            console.warn('Joystick Z: Datos de movimiento incompletos', data);
        }
    });

    joystick2.on('end', function () {
        console.log('Joystick Z -> Liberado');
    });

    // Limpia eventos al salir de la página
    $(window).on('beforeunload', function () {
        joystick1.destroy();
        joystick2.destroy();
    });
});
