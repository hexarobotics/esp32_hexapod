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
        const data = { x, y, z };

        // Enviar los datos al servidor
        fetch('/joystick-data', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(data),
        })
            .then((response) => response.json())
            .then((result) => {
                console.log('Datos enviados al ESP32:', result);
            })
            .catch((error) => {
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

// Inicializar el joystick X-Y
function initJoystickXY() {
    const container = document.getElementById('joystick1');
    const joystick = document.createElement('div');
    joystick.className = 'joystick';
    container.appendChild(joystick);

    const maxOffset = () => container.offsetWidth / 2 - joystick.offsetWidth / 2;

    let isDragging = false;

    // Eventos del mouse
    joystick.addEventListener('mousedown', () => {
        isDragging = true;
    });
    document.addEventListener('mouseup', () => {
        isDragging = false;
        x = 0;
        y = 0;
        joystick.style.left = '50%';
        joystick.style.top = '50%';
    });
    document.addEventListener('mousemove', (event) => {
        if (!isDragging) return;

        const rect = container.getBoundingClientRect();
        const centerX = rect.left + container.offsetWidth / 2;
        const centerY = rect.top + container.offsetHeight / 2;

        const rawX = event.clientX - centerX;
        const rawY = event.clientY - centerY;

        const distance = Math.sqrt(rawX * rawX + rawY * rawY);
        const maxDist = maxOffset();

        if (distance > maxDist) {
            const angle = Math.atan2(rawY, rawX);
            x = clampInt16(maxDist * Math.cos(angle));
            y = clampInt16(maxDist * Math.sin(angle));
        } else {
            x = clampInt16(rawX);
            y = clampInt16(rawY);
        }

        joystick.style.left = `${50 + (x / maxDist) * 50}%`;
        joystick.style.top = `${50 + (y / maxDist) * 50}%`;
    });
}

// Inicializar el joystick Z
function initJoystickZ() {
    const container = document.getElementById('joystick2');
    const joystick = document.createElement('div');
    joystick.className = 'joystick';
    container.appendChild(joystick);

    const maxOffset = () => container.offsetWidth / 2 - joystick.offsetWidth / 2;

    let isDragging = false;

    // Eventos del mouse
    joystick.addEventListener('mousedown', () => {
        isDragging = true;
    });
    document.addEventListener('mouseup', () => {
        isDragging = false;
        z = 0;
        joystick.style.left = '50%';
        joystick.style.top = '50%';
    });
    document.addEventListener('mousemove', (event) => {
        if (!isDragging) return;

        const rect = container.getBoundingClientRect();
        const centerY = rect.top + container.offsetHeight / 2;

        let rawZ = event.clientY - centerY;

        // Limitar Z al rango permitido
        z = clampInt16(rawZ);
        const maxDist = maxOffset();

        if (Math.abs(z) > maxDist) {
            z = clampInt16(maxDist * Math.sign(z));
        }

        joystick.style.top = `${50 + (z / maxDist) * 50}%`;
    });
}

// Inicializar joysticks al cargar el DOM
document.addEventListener('DOMContentLoaded', () => {
    initJoystickXY();
    initJoystickZ();

    // Verificar y enviar los datos cada 100 ms
    setInterval(sendJoystickData, 100);
});
