// Función para ajustar el tamaño de los joysticks dinámicamente
function adjustJoysticks()
{
    const joysticks = document.querySelectorAll("#joystick1, #joystick2");
    const width = window.innerWidth;
    const height = window.innerHeight;

    // Tamaño dinámico basado en el menor entre ancho y alto
    const size = Math.min(width * 0.4, height * 0.4); // 35% del ancho o alto

    joysticks.forEach(joystick => {
        joystick.style.width = `${size}px`;
        joystick.style.height = `${size}px`;
    });
}

// Inicializa los joysticks y eventos al cargar la página
$(document).ready(function ()
{
    // Ajusta los tamaños iniciales
    adjustJoysticks();

    // Joystick X-Y
    const joystick1 = nipplejs.create({
        zone: document.getElementById('joystick1'),
        mode: 'static',
        position: { left: '50%', top: '50%' },
        color: 'blue',
        size: 200
    });

    // Cambia el tamaño del círculo interior
    joystick1.on("added", function (evt, joystick) {
        joystick.ui.front.style.width = "40%"; // 40% del tamaño del círculo exterior
        joystick.ui.front.style.height = "40%";
    });

    // Eventos para Joystick X-Y
    joystick1.on('move', function (evt, data)
    {
        if (data && data.distance !== undefined && data.angle)
        {
            const distance = data.distance.toFixed(2);
            const angleRad = data.angle.radian;
            const x = (distance * Math.cos(angleRad)).toFixed(2);
            const y = (distance * Math.sin(angleRad)).toFixed(2);
            console.log(`Joystick X-Y -> X: ${x}, Y: ${y}, Distancia: ${distance}`);
        }
    });

    joystick1.on('end', function ()
    {
        console.log('Joystick X-Y -> Liberado');
    });

    // Eventos para Joystick Z

    // Joystick Z
    const joystick2 = nipplejs.create(
    {
        zone: document.getElementById("joystick2"),
        mode: "static",
        position: { left: "50%", top: "50%" },
        color: "red",
        size: 200
    });

    joystick2.on('move', function (evt, data) {
        if (data && data.distance !== undefined) {
            const z = data.distance.toFixed(2);
            console.log(`Joystick Z -> Distance: ${z}`);
        }
    });

    // Ajustar el tamaño de los joysticks dinámicamente al cambiar el tamaño de la ventana
    window.addEventListener("resize", adjustJoysticks);
});
