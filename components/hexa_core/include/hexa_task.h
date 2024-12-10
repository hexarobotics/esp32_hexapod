#ifndef HEXA_TASK_H
    #define HEXA_TASK_H

namespace hexapod
{

    extern const char *HEXA_TASK_TAG;

    /**
     * Función principal que maneja las tareas del hexápodo.
     */
    void hexa_main_task(void *pvParameters);

}

#endif // HEXA_TASK_H