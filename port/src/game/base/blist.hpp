#pragma once
// ../indep/bWare/Src/bList.cpp: doppelt verkettete Ringliste mit Kopfknoten (WP-30.2).
// Reines Laufzeitobjekt (ADR-001): native Zeiger. Dateigebundene Listen gibt es im Spiel nicht.
#include <cstddef>

namespace nfsu {

struct bNode {
    bNode* Next;  // 0x0
    bNode* Prev;  // 0x4
};

struct bList {
    bNode HeadNode;

    bList() { HeadNode.Next = HeadNode.Prev = &HeadNode; }
    bList(const bList&) = delete;
    bList& operator=(const bList&) = delete;

    bool IsEmpty() const { return HeadNode.Next == &HeadNode; }
    bNode* EndOfList() { return &HeadNode; }
    bNode* GetHead() const { return HeadNode.Next == &HeadNode ? nullptr : HeadNode.Next; }
    bNode* GetTail() const { return HeadNode.Prev == &HeadNode ? nullptr : HeadNode.Prev; }

    // orig: 0x401b40 (Ghidra "FUN_00401b40") bList::AddTail(bNode*)
    void AddTail(bNode* n) {
        bNode* tail = HeadNode.Prev;
        tail->Next = n;
        HeadNode.Prev = n;
        n->Prev = tail;
        n->Next = &HeadNode;
    }
    // orig: 0x598150 bList::AddTail(bList*): hängt alle Knoten von `other` hinten an, `other` bleibt leer
    void AddTail(bList* other) {
        bNode* first = other->HeadNode.Next;
        bNode* last = other->HeadNode.Prev;
        if (first != &other->HeadNode) {
            bNode* tail = HeadNode.Prev;
            last->Next = &HeadNode;
            first->Prev = tail;
            tail->Next = first;
            HeadNode.Prev = last;
            other->HeadNode.Next = other->HeadNode.Prev = &other->HeadNode;
        }
    }
    // orig: 0x598180 bList::AddSorted: weiter, solange `after(neu, vorhanden)` ungleich 0 ist; vor dem ersten Knoten
    // einfügen, bei dem es 0 liefert (am Ende, wenn keiner)
    bNode* AddSorted(int (*after)(bNode* neu, bNode* vorhanden), bNode* n) {
        bNode* p = HeadNode.Next;
        while (p != &HeadNode) {
            if (after(n, p) == 0) break;
            p = p->Next;
        }
        bNode* before = p->Prev;
        before->Next = n;
        p->Prev = n;
        n->Next = p;
        n->Prev = before;
        return n;
    }
    // orig: 0x598120 (Ghidra "list_index_of"): 1-basierte Position; 0, wenn nicht enthalten; bei n == nullptr die Länge
    int IndexOf(const bNode* n) const {
        int i = 0;
        for (const bNode* p = HeadNode.Next;; p = p->Next) {
            if (p == &HeadNode) return n != nullptr ? 0 : i;
            ++i;
            if (p == n) return i;
        }
    }
};

// orig: 0x401b60 (Ghidra "list_unlink_front"): ersten Knoten aus der Liste lösen
inline void bUnlinkFront(bList* l) {
    bNode* first = l->HeadNode.Next;
    bNode* prev = first->Prev;
    prev->Next = first->Next;
    first->Next->Prev = prev;
}

}  // namespace nfsu
