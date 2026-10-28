// 32-Bit-Zeigerfelder für dateigebundene Strukturen (ADR-001).
#pragma once
#include <cstddef>
#include <cstdint>

namespace nfsu {

// Eine Arena ist ein zusammenhängender Speicherbereich (< 4 GiB). ptr32<T> speichert den
// Abstand zur Arena-Basis, 0 ist null. Offset 0 ist daher nie belegbar (die Arena reserviert
// ihre ersten 16 Byte).
struct Arena {
    uint8_t* base = nullptr;
    size_t size = 0;
};
inline Arena g_arena;  // genau eine Arena pro Prozess, wie der eine Heap des Originals

template <class T>
struct ptr32 {
    uint32_t off = 0;

    ptr32() = default;
    ptr32(T* p) { set(p); }
    void set(T* p) { off = p ? uint32_t(reinterpret_cast<uint8_t*>(p) - g_arena.base) : 0; }
    T* get() const { return off ? reinterpret_cast<T*>(g_arena.base + off) : nullptr; }
    T* operator->() const { return get(); }
    template <class U = T> U& operator*() const { return *get(); }
    explicit operator bool() const { return off != 0; }
    ptr32& operator=(T* p) { set(p); return *this; }
};
static_assert(sizeof(ptr32<int>) == 4, "ptr32 muss 4 Byte belegen");

}  // namespace nfsu
