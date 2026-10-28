#pragma once
// EventManager.cpp: Ereigniswarteschlange mit Handlern, die über eine Bitmaske auf Ereignis-IDs hören.
#include <chrono>
#include <list>

namespace nfsu {

struct emEvent {
    int id = 0;        // orig +0x0c (EVENT_ID, auch Bitmaske des Handlers)
    int keep = 0;      // orig +0x08: ≠ 0 bleibt nach der Verarbeitung in der Warteschlange
    const void* trigger = nullptr;  // orig +0x10: auslösender Eintrag der Strecke
};

using emHandlerFn = void (*)(emEvent*);

class EventManager {
public:
    struct Handler {
        emHandlerFn fn;
        unsigned mask;
        int refs;
        double seconds;  // aufsummierte Laufzeit (orig +0x14, Profiling)
    };

    int numHandlers = 0, peakHandlers = 0, numEvents = 0;

    bool AddHandler(emHandlerFn fn, unsigned mask);   // orig: 0x569270 emAddHandler
    void RemoveHandler(emHandlerFn fn);               // orig: 0x569320 emRemoveHandler
    emEvent* AddEvent(int id);                        // orig: 0x569370 emAddEvent
    void ProcessAllEvents();                          // orig: 0x5693c0 emProcessAllEvents
    size_t QueueSize() const { return queue_.size(); }
    const std::list<Handler>& handlers() const { return handlers_; }

private:
    std::list<Handler> handlers_;
    std::list<emEvent> queue_;
    std::list<emEvent>* target_ = &queue_;  // orig: 0x6e2c68, Ziel von emAddEvent
};

}  // namespace nfsu
