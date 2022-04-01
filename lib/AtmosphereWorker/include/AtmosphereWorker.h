#pragma once

class AtmosphereWorker {
public:
    [[noreturn]] static void TaskHandler(void *param);
};