// Detectar si es un dispositivo móvil
const isMobile = /iPhone|iPad|iPod|Android/i.test(navigator.userAgent);

// Variables globales
let x = 0, y = 0, z = 0;

// Función para limitar valores a un rango válido
function clampInt16(value) {
    return Math.max(Math.min(Math.round(value), 32767), -32768);
}

// Inicializar joystick para PC y móviles
function initJoystick(containerId, axis) {
    const container = document.getElementById(containerId).parentNode;
    const joystick = document.getElementById(containerId);

    const outerRadius = container.offsetWidth / 2; // Radio del círculo exterior
    const innerRadius = joystick.offsetWidth / 2; // Radio del círculo interior

    // Variables para el desplazamiento
    let startX = 0, startY = 0;

    if (isMobile) {
        // Comportamiento para dispositivos móviles
        joystick.addEventListener('touchstart', (event) => {
            const touch = event.touches[0];
            startX = touch.clientX;
            startY = touch.clientY;
        });

        joystick.addEventListener('touchmove', (event) => {
            event.preventDefault(); // Evitar el scroll en móviles
            const touch = event.touches[0];
            const deltaX = touch.clientX - startX;
            const deltaY = touch.clientY - startY;

            const rect = container.getBoundingClientRect();
            const centerX = rect.left + outerRadius;
            const centerY = rect.top + outerRadius;

            const distance = Math.sqrt(deltaX ** 2 + deltaY ** 2);

            if (distance <= outerRadius - innerRadius) {
                joystick.style.left = `${50 + (deltaX / outerRadius) * 50}%`;
                joystick.style.top = `${50 + (deltaY / outerRadius) * 50}%`;
            } else {
                const angle = Math.atan2(deltaY, deltaX);
                joystick.style.left = `${50 + Math.cos(angle) * (outerRadius - innerRadius) / outerRadius * 50}%`;
                joystick.style.top = `${50 + Math.sin(angle) * (outerRadius - innerRadius) / outerRadius * 50}%`;
            }

            // Actualizar valores según el eje
            if (axis === 'xy') {
                x = clampInt16((deltaX / outerRadius) * 32767);
                y = clampInt16((deltaY / outerRadius) * 32767);
            } else if (axis === 'z') {
                z = clampInt16((deltaY / outerRadius) * 32767);
            }

            console.log(`Joystick ${axis.toUpperCase()}: X=${x}, Y=${y}, Z=${z}`);
        });

        joystick.addEventListener('touchend', () => {
            joystick.style.left = '50%';
            joystick.style.top = '50%';

            if (axis === 'xy') {
                x = 0;
                y = 0;
            } else if (axis === 'z') {
                z = 0;
            }

            console.log(`Joystick ${axis.toUpperCase()} reseteado: X=${x}, Y=${y}, Z=${z}`);
        });
    } else {
        // Comportamiento para dispositivos PC
        interact(joystick).draggable({
            listeners: {
                start(event) {
                    const rect = joystick.getBoundingClientRect();
                    startX = rect.left + rect.width / 2;
                    startY = rect.top + rect.height / 2;
                },
                move(event) {
                    const rect = container.getBoundingClientRect();
                    const centerX = rect.left + outerRadius;
                    const centerY = rect.top + outerRadius;

                    const deltaX = event.clientX - centerX;
                    const deltaY = event.clientY - centerY;

                    const distance = Math.sqrt(deltaX ** 2 + deltaY ** 2);

                    if (distance <= outerRadius - innerRadius) {
                        joystick.style.left = `${50 + (deltaX / outerRadius) * 50}%`;
                        joystick.style.top = `${50 + (deltaY / outerRadius) * 50}%`;
                    } else {
                        const angle = Math.atan2(deltaY, deltaX);
                        joystick.style.left = `${50 + Math.cos(angle) * (outerRadius - innerRadius) / outerRadius * 50}%`;
                        joystick.style.top = `${50 + Math.sin(angle) * (outerRadius - innerRadius) / outerRadius * 50}%`;
                    }

                    // Actualizar valores según el eje
                    if (axis === 'xy') {
                        x = clampInt16((deltaX / outerRadius) * 32767);
                        y = clampInt16((deltaY / outerRadius) * 32767);
                    } else if (axis === 'z') {
                        z = clampInt16((deltaY / outerRadius) * 32767);
                    }

                    console.log(`Joystick ${axis.toUpperCase()}: X=${x}, Y=${y}, Z=${z}`);
                },
                end() {
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
}

// Inicializar ambos joysticks
document.addEventListener('DOMContentLoaded', () => {
    initJoystick('joystick1', 'xy');
    initJoystick('joystick2', 'z');
});
