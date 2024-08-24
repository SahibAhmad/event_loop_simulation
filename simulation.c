#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

// A simple event structure
typedef struct {
    void (*callback)(void); // Pointer to the callback function
    time_t trigger_time;    // When the event should be triggered (for async tasks)
    bool active;            // If the event is active
    bool is_sync;           // Whether the task is synchronous or asynchronous
} Event;

// Synchronous Task 1
void syncTask1() {
    printf("Synchronous Task 1 executed.\n");
}

// Synchronous Task 2
void syncTask2() {
    printf("Synchronous Task 2 executed.\n");
}

// Asynchronous Task
void asyncTask() {
    printf("Asynchronous Task executed.\n");
}

// Simulating the event loop
void eventLoop(Event *events, int event_count) {
    for (int i = 0; i < event_count; i++) {
        if (events[i].is_sync) {
            // Execute synchronous tasks immediately
            events[i].callback();
            events[i].active = false;
        }
    }

    while (true) {
        time_t current_time = time(NULL);
        bool any_active = false;

        for (int i = 0; i < event_count; i++) {
            if (!events[i].is_sync && events[i].active && current_time >= events[i].trigger_time) {
                events[i].callback();  // Execute the asynchronous callback
                events[i].active = false;  // Deactivate the event after it's triggered
            }
            if (events[i].active) {
                any_active = true;  // Check if there are still active events
            }
        }

        if (!any_active) {
            break;  // Exit loop if no events are active
        }

        usleep(100000);  // Sleep for 100ms to simulate non-blocking behavior
    }
}

int main() {
    // Create synchronous tasks
    Event syncEvent1 = {syncTask1, 0, true, true};
    Event syncEvent2 = {syncTask2, 0, true, true};

    // Create an asynchronous task
    Event asyncEvent = {asyncTask, time(NULL) + 2, true, false};  // Trigger after 2 seconds

    // Array of events
    Event events[] = {syncEvent1, syncEvent2, asyncEvent};

    // Run the event loop
    eventLoop(events, 3);

    printf("Event loop finished.\n");
    return 0;
}

