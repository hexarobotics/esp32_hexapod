$(document).ready(function () {
    // Función para ajustar el tamaño de los joysticks dinámicamente
    function resizeJoysticks() {
        const joystickContainers = document.querySelectorAll('.joystick-container');
        const joysticks = document.querySelectorAll('#joystick1, #joystick2');

        joystickContainers.forEach((container, index) => {
            const containerWidth = container.offsetWidth;
            const containerHeight = container.offsetHeight;

            // Calcular tamaño en base al espacio disponible (90% del menor de ancho o alto)
            const joystickSize = Math.min(containerWidth, containerHeight) * 0.9;

            // Aplicar el tamaño calculado
            joysticks[index].style.width = `${joystickSize}px`;
            joysticks[index].style.height = `${joystickSize}px`;
        });
    }

    // Ejecutar la función al cargar la página y al redimensionar la ventana
    resizeJoysticks();
    $(window).on('resize', resizeJoysticks);

    // Inicializar los joysticks con nipple.js
    const joystick1 = nipplejs.create({
        zone: document.getElementById('joystick1'),
        mode: 'static',
        position: { left: '50%', top: '50%' },
        color: 'blue',
    });

    const joystick2 = nipplejs.create({
        zone: document.getElementById('joystick2'),
        mode: 'static',
        position: { left: '50%', top: '50%' },
        color: 'red',
    });

    // Eventos para Joystick X-Y
    joystick1.on('move', function (evt, data) {
        if (data && data.distance !== undefined && data.angle) {
            const distance = data.distance.toFixed(2);
            const angleRad = data.angle.radian;
            const x = (distance * Math.cos(angleRad)).toFixed(2);
            const y = (distance * Math.sin(angleRad)).toFixed(2);
            console.log(`Joystick X-Y -> X: ${x}, Y: ${y}, Distancia: ${distance}`);
        }
    });

    joystick1.on('end', function () {
        console.log('Joystick X-Y -> Liberado');
    });

    // Eventos para Joystick Z
    joystick2.on('move', function (evt, data) {
        if (data && data.distance !== undefined) {
            const z = data.distance.toFixed(2);
            console.log(`Joystick Z -> Distance: ${z}`);
        }
    });

    joystick2.on('end', function () {
        console.log('Joystick Z -> Liberado');
    });
});
