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
        if (data && data.distance !== undefined && data.angle) {
            const distance = data.distance.toFixed(2); // Distancia desde el centro
            const angleRad = data.angle.radian; // Ángulo en radianes
            const x = (distance * Math.cos(angleRad)).toFixed(2); // Calcular X
            const y = (distance * Math.sin(angleRad)).toFixed(2); // Calcular Y
            console.log(`Joystick X-Y -> X: ${x}, Y: ${y}, Distancia: ${distance}`);
        } else {
            console.warn('Joystick X-Y: Movimiento no detectado', data);
        }
    });

    joystick1.on('end', function () {
        console.log('Joystick X-Y -> Liberado');
    });

    // Evento para el joystick Z
    joystick2.on('move', function (evt, data) {
        if (data && data.distance !== undefined) {
            const z = data.distance.toFixed(2); // Distancia como control Z
            console.log(`Joystick Z -> Distance: ${z}`);
        } else {
            console.warn('Joystick Z: Movimiento no detectado', data);
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
