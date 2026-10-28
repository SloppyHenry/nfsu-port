#include "game/misc/events.hpp"

#include <algorithm>

namespace nfsu {

// orig: 0x569270 emAddHandler: derselbe Handler zweimal erhöht nur den Zähler
bool EventManager::AddHandler(emHandlerFn fn, unsigned mask) {
    if (!fn || !mask) return false;
    for (auto& h : handlers_) {
        if (h.fn == fn) {
            ++h.refs;
            return true;
        }
    }
    handlers_.push_back({fn, mask, 1, 0.0});
    ++numHandlers;
    peakHandlers = std::max(peakHandlers, numHandlers);
    return true;
}

// orig: 0x569320 emRemoveHandler
void EventManager::RemoveHandler(emHandlerFn fn) {
    for (auto it = handlers_.begin(); it != handlers_.end(); ++it) {
        if (it->fn == fn) {
            if (--it->refs == 0) {
                handlers_.erase(it);
                --numHandlers;
            }
            return;
        }
    }
}

// orig: 0x569370 emAddEvent
emEvent* EventManager::AddEvent(int id) {
    target_->push_back(emEvent{id, 0, nullptr});
    ++numEvents;
    return &target_->back();
}

// orig: 0x5693c0 emProcessAllEvents
// Während der Verarbeitung gehen neue Ereignisse in eine eigene Liste und kommen erst danach hinten an die Warteschlange,
// Ereignisse mit keep != 0 davor. Alle anderen werden verworfen. Handlerlaufzeit wird mit aufsummiert.
void EventManager::ProcessAllEvents() {
    std::list<emEvent> fresh, kept;
    if (!queue_.empty()) {
        target_ = &fresh;
        while (!queue_.empty()) {
            emEvent& ev = queue_.front();
            for (auto& h : handlers_) {
                if ((static_cast<unsigned>(ev.id) & h.mask) == 0) continue;
                auto t0 = std::chrono::steady_clock::now();
                h.fn(&ev);
                h.seconds += std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
            }
            if (ev.keep != 0)
                kept.splice(kept.end(), queue_, queue_.begin());
            else
                queue_.pop_front();
        }
        queue_.splice(queue_.end(), kept);
        queue_.splice(queue_.end(), fresh);
    }
    target_ = &queue_;
}

}  // namespace nfsu
