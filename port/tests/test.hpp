// Minimales Test-Framework ohne Fremdbibliothek.
#pragma once
#include <cstdio>
#include <vector>

namespace nfsu_test {
struct Case { const char* name; void (*fn)(); };
std::vector<Case>& registry();
void fail(const char* file, int line, const char* expr);
struct Registrar { Registrar(const char* n, void (*f)()) { registry().push_back({n, f}); } };
}  // namespace nfsu_test

#define TEST(name) \
    static void test_##name(); \
    static nfsu_test::Registrar reg_##name(#name, test_##name); \
    static void test_##name()
#define CHECK(expr) do { if (!(expr)) nfsu_test::fail(__FILE__, __LINE__, #expr); } while (0)
