// Detectar si es un dispositivo móvil
const isMobile = /iPhone|iPad|iPod|Android/i.test(navigator.userAgent);

// Variables globales
let x = 0, y = 0, z = 0;
let lastX = 0, lastY = 0, lastZ = 0;

// Función para limitar valores a un rango válido
function clampInt16(value) {
    return Math.max(Math.min(Math.round(value), 32767), -32768);
}

// Función para enviar datos de los joysticks al servidor cada 100ms
function sendJoystickData() {
    if (x !== lastX || y !== lastY || z !== lastZ) {
        const data = { x, y, z };

        fetch('/joystick-data', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data),
        })
            .then((response) => response.json())
            .then((result) => {
                console.log('Datos enviados al servidor:', result);
            })
            .catch((error) => {
                console.error('Error enviando datos al servidor:', error);
            });

        lastX = x;
        lastY = y;
        lastZ = z;
    }
}

// Función para calcular la posición del joystick
function calculateJoystickPosition(touch, container, outerRadius) {
    const rect = container.getBoundingClientRect();
    const centerX = rect.left + outerRadius;
    const centerY = rect.top + outerRadius;

    const deltaX = touch.clientX - centerX;
    const deltaY = touch.clientY - centerY;

    const distance = Math.sqrt(deltaX ** 2 + deltaY ** 2);
    const angle = Math.atan2(deltaY, deltaX);

    // Normalizar la distancia al radio exterior
    const normalizedDistance = Math.min(distance, outerRadius);

    return {
        x: normalizedDistance * Math.cos(angle) / outerRadius,
        y: normalizedDistance * Math.sin(angle) / outerRadius,
        rawX: deltaX,
        rawY: deltaY,
    };
}

// Inicializar joystick para PC y móviles
function initJoystick(containerId, axis) {
    const container = document.getElementById(containerId).parentNode;
    const joystick = document.getElementById(containerId);

    const outerRadius = container.offsetWidth / 2;
    const innerRadius = joystick.offsetWidth / 2;

    let activeTouchId = null;

    if (isMobile) {
        container.addEventListener('touchstart', (event) => {
            if (activeTouchId === null) {
                const touch = event.changedTouches[0];
                activeTouchId = touch.identifier;

                const position = calculateJoystickPosition(touch, container, outerRadius);
                joystick.style.left = `${50 + position.x * 50}%`;
                joystick.style.top = `${50 + position.y * 50}%`;

                if (axis === 'xy') {
                    x = clampInt16(position.rawX / outerRadius * 32767);
                    y = clampInt16(position.rawY / outerRadius * 32767);
                } else if (axis === 'z') {
                    z = clampInt16(position.rawY / outerRadius * 32767);
                }

                event.preventDefault();
            }
        }, { passive: false });

        container.addEventListener('touchmove', (event) => {
            const touch = Array.from(event.touches).find(t => t.identifier === activeTouchId);
            if (!touch) return;

            const position = calculateJoystickPosition(touch, container, outerRadius);
            joystick.style.left = `${50 + position.x * 50}%`;
            joystick.style.top = `${50 + position.y * 50}%`;

            if (axis === 'xy') {
                x = clampInt16(position.rawX / outerRadius * 32767);
                y = clampInt16(position.rawY / outerRadius * 32767);
            } else if (axis === 'z') {
                z = clampInt16(position.rawY / outerRadius * 32767);
            }

            event.preventDefault();
        }, { passive: false });

        container.addEventListener('touchend', (event) => {
            const touch = Array.from(event.changedTouches).find(t => t.identifier === activeTouchId);
            if (!touch) return;

            activeTouchId = null;
            joystick.style.left = '50%';
            joystick.style.top = '50%';

            if (axis === 'xy') {
                x = 0;
                y = 0;
            } else if (axis === 'z') {
                z = 0;
            }
        });
    } else {
        // Comportamiento para dispositivos PC
        interact(joystick).draggable({
            listeners: {
                move(event) {
                    const position = calculateJoystickPosition(event, container, outerRadius);
                    joystick.style.left = `${50 + position.x * 50}%`;
                    joystick.style.top = `${50 + position.y * 50}%`;

                    if (axis === 'xy') {
                        x = clampInt16(position.rawX / outerRadius * 32767);
                        y = clampInt16(position.rawY / outerRadius * 32767);
                    } else if (axis === 'z') {
                        z = clampInt16(position.rawY / outerRadius * 32767);
                    }
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
                },
            },
        });
    }
}

// Inicializar ambos joysticks y configurar el envío de datos cada 100ms
document.addEventListener('DOMContentLoaded', () => {
    initJoystick('joystick1', 'xy');
    initJoystick('joystick2', 'z');

    // Configurar el intervalo de envío de datos cada 100ms
    setInterval(sendJoystickData, 100);

    // Agregar funcionalidad de pantalla completa
    const fullscreenBtn = document.getElementById('fullscreen-btn');

    fullscreenBtn.addEventListener('click', () => {
        if (!document.fullscreenElement) {
            document.documentElement.requestFullscreen().catch((err) => {
                alert(`Error al entrar en modo pantalla completa: ${err.message} (${err.name})`);
            });
            fullscreenBtn.value = "Exit";
        } else {
            document.exitFullscreen();
            fullscreenBtn.value = "Full Screen";
        }
    });

    // Cambiar el texto del botón cuando se cambia el estado de pantalla completa
    document.addEventListener('fullscreenchange', () => {
        if (!document.fullscreenElement) {
            fullscreenBtn.value = "Full Screen";
        } else {
            fullscreenBtn.value = "Exit";
        }
    });
});
