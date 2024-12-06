// Configuración y constantes
const CONFIG = {
    sendInterval: 100, // Intervalo de envío de datos en ms
    maxJoystickValue: 32767,
};

// Variables globales
let joystickData = {};
let x = 0, y = 0, z = 0;
let lastX = 0, lastY = 0, lastZ = 0;

// Detectar dispositivo
const isMobile = /iPhone|iPad|iPod|Android/i.test(navigator.userAgent);
const isTablet = /iPad|Android(?!.*Mobile)/i.test(navigator.userAgent); // Detectar tablets

// Ajustar diseño específico para tablets
function adjustLayoutForTablet() {
    if (isTablet) {
        console.log('Tablet detected');
        document.body.classList.add('tablet-mode');
    }
}

// Función para limitar valores al rango de int16_t
function clampInt16(value) {
    return Math.max(Math.min(Math.round(value), CONFIG.maxJoystickValue), -CONFIG.maxJoystickValue);
}

// Inicializar depuración para divisiones principales
function initDebugForDivisions() {
    const containers = [
        document.querySelector('header'),       // Header
        document.querySelector('#joysticks'),  // Contenedor de joysticks
        document.body                          // Todo el body
    ];

    containers.forEach((container) => {
        if (container) {
            container.classList.add('debug-container');
        }
    });
}


// Función para enviar datos del joystick
function sendJoystickData() {
    if (x !== lastX || y !== lastY || z !== lastZ) {
        fetch('/joystick-data', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ x, y, z }),
        })
        .then((res) => res.json())
        .then((result) => console.log('Datos enviados:', result))
        .catch((err) => console.error('Error:', err));

        lastX = x;
        lastY = y;
        lastZ = z;
    }
}

// Clase para manejar un joystick
class Joystick {
    constructor(containerId, axis) {
        this.container = document.getElementById(containerId).parentNode;
        this.joystick = document.getElementById(containerId);
        this.axis = axis;
        this.outerRadius = 0;
        this.rect = {};
        this.activeTouchId = null;

        this.init();
    }

    init() {
        this.recalculateDimensions();

        if (isMobile || isTablet) {
            this.setupMobileEvents();
        } else {
            this.setupPCEvents();
        }
    }

    recalculateDimensions() {
        this.outerRadius = this.container.offsetWidth / 2;
        this.rect = this.container.getBoundingClientRect();
    }

    calculatePosition(touch) {
        const centerX = this.rect.left + this.outerRadius;
        const centerY = this.rect.top + this.outerRadius;

        const deltaX = touch.clientX - centerX;
        const deltaY = touch.clientY - centerY;

        const distance = Math.sqrt(deltaX ** 2 + deltaY ** 2);
        const limitedDistance = Math.min(distance, this.outerRadius);
        const angle = Math.atan2(deltaY, deltaX);

        return {
            x: limitedDistance * Math.cos(angle) / this.outerRadius,
            y: limitedDistance * Math.sin(angle) / this.outerRadius,
            rawX: deltaX,
            rawY: deltaY,
        };
    }

    setupMobileEvents() {
        this.container.addEventListener('touchstart', (event) => {
            if (this.activeTouchId === null) {
                this.recalculateDimensions();
                const touch = event.changedTouches[0];
                this.activeTouchId = touch.identifier;
                this.updatePosition(touch);
            }
        }, { passive: false });

        this.container.addEventListener('touchmove', (event) => {
            const touch = Array.from(event.touches).find(t => t.identifier === this.activeTouchId);
            if (touch) {
                this.updatePosition(touch);
            }
        }, { passive: false });

        this.container.addEventListener('touchend', (event) => {
            const touch = Array.from(event.changedTouches).find(t => t.identifier === this.activeTouchId);
            if (touch) {
                this.resetPosition();
            }
        });
    }

    setupPCEvents() {
        let isDragging = false;

        this.joystick.addEventListener('mousedown', (event) => {
            this.recalculateDimensions();
            isDragging = true;
            this.updatePosition(event);
        });

        window.addEventListener('mousemove', (event) => {
            if (isDragging) {
                this.updatePosition(event);
            }
        });

        window.addEventListener('mouseup', () => {
            if (isDragging) {
                isDragging = false;
                this.resetPosition();
            }
        });
    }

    updatePosition(touch) {
        const position = this.calculatePosition(touch);
        this.joystick.style.left = `${50 + position.x * 50}%`;
        this.joystick.style.top = `${50 + position.y * 50}%`;

        if (this.axis === 'xy') {
            x = clampInt16(position.rawX / this.outerRadius * CONFIG.maxJoystickValue);
            y = clampInt16(-position.rawY / this.outerRadius * CONFIG.maxJoystickValue);
        } else if (this.axis === 'z') {
            z = clampInt16(position.rawX / this.outerRadius * CONFIG.maxJoystickValue);
        }
    }

    resetPosition() {
        this.joystick.style.left = '50%';
        this.joystick.style.top = '50%';
        this.activeTouchId = null;

        if (this.axis === 'xy') {
            x = 0;
            y = 0;
        } else if (this.axis === 'z') {
            z = 0;
        }
    }
}

// Ajustar diseño de los joysticks
function adjustJoysticksLayout() {
    Object.values(joystickData).forEach((joystick) => joystick.recalculateDimensions());
}


// Inicializar joysticks
document.addEventListener('DOMContentLoaded', () => {
    joystickData['joystick1'] = new Joystick('joystick1', 'xy');
    joystickData['joystick2'] = new Joystick('joystick2', 'z');

    setInterval(sendJoystickData, CONFIG.sendInterval);

    const fullscreenBtn = document.getElementById('fullscreen-btn');

    // Agregar eventos para los botones
    document.getElementById("button-1").addEventListener("click", () => {
        console.log("Botón Tripod presionado");
    });

    document.getElementById("button-2").addEventListener("click", () => {
        console.log("Botón Ripple presionado");
    });

    document.getElementById("button-3").addEventListener("click", () => {
        console.log("Botón Wave presionado");
    });

    fullscreenBtn.addEventListener('click', () => {
        if (!document.fullscreenElement) {
            document.documentElement.requestFullscreen();
        } else {
            document.exitFullscreen();
        }
    });

    window.addEventListener('resize', adjustJoysticksLayout);
    adjustJoysticksLayout();
    adjustLayoutForTablet(); // Llama al ajuste de tabletas

    initDebugForDivisions();
});
