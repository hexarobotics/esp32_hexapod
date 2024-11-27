// Variables globales para coordenadas
let x = 0, y = 0, z = 0;
let lastX = 0, lastY = 0, lastZ = 0;

// Función para limitar valores dentro del rango permitido
function clampInt16(value) {
    return Math.max(Math.min(Math.round(value), 32767), -32768);
}

// Función para enviar datos del joystick
function sendJoystickData() {
    if (x !== lastX || y !== lastY || z !== lastZ) {
        const data = { x, y, z };

        // Log de los valores actuales
        console.log(`Enviando datos: X=${x}, Y=${y}, Z=${z}`);

        fetch('/joystick-data', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data),
        })
            .then((response) => response.json())
            .then((result) => console.log('Datos enviados:', result))
            .catch((error) => console.error('Error enviando datos:', error));

        lastX = x;
        lastY = y;
        lastZ = z;
    }
}

// Inicializar joystick con Interact.js
function initJoystick(containerId, axis) {
    const container = document.getElementById(containerId).parentNode;
    const joystick = document.getElementById(containerId);

    const outerRadius = container.offsetWidth / 2; // Radio del círculo exterior
    const innerRadius = joystick.offsetWidth / 2; // Radio del círculo interior

    interact(joystick).draggable({
        listeners: {
            move(event) {
                const rect = container.getBoundingClientRect();
                const centerX = rect.left + outerRadius;
                const centerY = rect.top + outerRadius;

                const deltaX = event.clientX - centerX;
                const deltaY = event.clientY - centerY;

                const distance = Math.sqrt(deltaX ** 2 + deltaY ** 2);

                // Limitar el joystick al borde del círculo exterior
                if (distance <= outerRadius) {
                    joystick.style.left = `${50 + (deltaX / outerRadius) * 50}%`;
                    joystick.style.top = `${50 + (deltaY / outerRadius) * 50}%`;
                } else {
                    const angle = Math.atan2(deltaY, deltaX);
                    joystick.style.left = `${50 + (Math.cos(angle) * outerRadius) / outerRadius * 50}%`;
                    joystick.style.top = `${50 + (Math.sin(angle) * outerRadius) / outerRadius * 50}%`;
                }

                // Actualizar valores según el eje
                if (axis === 'xy') {
                    x = clampInt16((deltaX / outerRadius) * 32767);
                    y = clampInt16((deltaY / outerRadius) * 32767);
                } else if (axis === 'z') {
                    z = clampInt16((deltaY / outerRadius) * 32767);
                }

                // Log de valores
                console.log(`Joystick ${axis.toUpperCase()}: X=${x}, Y=${y}, Z=${z}`);
            },
            end(event) {
                // Resetear el joystick al centro
                joystick.style.left = '50%';
                joystick.style.top = '50%';

                if (axis === 'xy') {
                    x = 0;
                    y = 0;
                } else if (axis === 'z') {
                    z = 0;
                }

                console.log(`Joystick ${axis.toUpperCase()} reseteado: X=${x}, Y=${y}, Z=${z}`);
            },
        },
    });
}

// Inicializar joysticks al cargar el DOM
document.addEventListener('DOMContentLoaded', () => {
    initJoystick('joystick1', 'xy');
    initJoystick('joystick2', 'z');
    setInterval(sendJoystickData, 100); // Enviar datos cada 100 ms
});
