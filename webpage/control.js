
// Variables globales para almacenar las coordenadas como int16_t
let x = 0, y = 0, z = 0;

// Variables para guardar los últimos valores enviados
let lastX = 0, lastY = 0, lastZ = 0;

// Función para limitar los valores al rango de int16_t
function clampInt16(value) {
    return Math.max(Math.min(Math.round(value), 32767), -32768);
}

// Función para enviar los datos de los joysticks al servidor
function sendJoystickData() {
    // Verificar si ha habido cambios en las coordenadas antes de enviarlas
    if (x !== lastX || y !== lastY || z !== lastZ) {
        // Crea un objeto con los datos a enviar
        const data = {
            x: x,
            y: y,
            z: z
        };

        // Enviar los datos al servidor
        fetch('/joystick-data', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(data)
        })
        .then(response => response.json())
        .then(result => {
            console.log('Datos enviados al ESP32:', result);
        })
        .catch(error => {
            console.error('Error enviando datos al ESP32:', error);
        });

        // Actualizamos los últimos valores enviados
        lastX = x;
        lastY = y;
        lastZ = z;
    } else {
        console.log('No hubo cambios en las coordenadas, no se envían datos.');
    }
}


$(document).ready(function () {
    // Crear joystick X-Y
    const joystick1 = nipplejs.create({
        zone: document.getElementById("joystick1"),
        mode: "static",
        position: { left: "50%", top: "50%" },
        color: "blue",
        size: 105 // Tamaño fijo para el joystick
    });

    // Crear joystick Z
    const joystick2 = nipplejs.create({
        zone: document.getElementById("joystick2"),
        mode: "static",
        position: { left: "50%", top: "50%" },
        color: "red",
        size: 105 // Tamaño fijo para el joystick
    });

    // Eventos para joystick X-Y
    joystick1.on("move", function (evt, data) {
        if (data && data.distance !== undefined && data.angle) {
            const distance = data.distance;
            const angleRad = data.angle.radian;

            // Calcular las coordenadas x, y
            let rawX = distance * Math.cos(angleRad);
            let rawY = distance * Math.sin(angleRad);

            // Limitar los valores a int16_t
            x = clampInt16(rawX);
            y = clampInt16(rawY);

            console.log(`Joystick X-Y -> X: ${x}, Y: ${y}`);
        }
    });

    joystick1.on("end", function () {
        console.log("Joystick X-Y -> Liberado");
        // Restablecer las coordenadas a cero al liberar el joystick
        x = 0;
        y = 0;
    });

    // Eventos para joystick Z
    joystick2.on("move", function (evt, data) {
        if (data && data.distance !== undefined) {
            let rawZ = data.distance;

            // Limitar Z a int16_t
            z = clampInt16(rawZ);

            console.log(`Joystick Z -> Distance: ${z}`);
        }
    });

    // Restablece Z a 0 cuando se suelta el joystick Z
    joystick2.on("end", function () {
        console.log("Joystick Z -> Liberado");
        z = 0;
    });

    // Función para verificar y enviar los datos cada 100 ms
    setInterval(sendJoystickData, 100);

});
