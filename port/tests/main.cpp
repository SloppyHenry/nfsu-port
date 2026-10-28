#include "test.hpp"

namespace nfsu_test {
static int failures = 0;
std::vector<Case>& registry() { static std::vector<Case> r; return r; }
void fail(const char* file, int line, const char* expr) {
    std::printf("  FEHLER %s:%d: %s\n", file, line, expr);
    ++failures;
}
}  // namespace nfsu_test

int main() {
    for (auto& c : nfsu_test::registry()) {
        int before = nfsu_test::failures;
        c.fn();
        std::printf("[%s] %s\n", nfsu_test::failures == before ? " ok " : "FAIL", c.name);
    }
    std::printf("%zu Tests, %d Fehler\n", nfsu_test::registry().size(), nfsu_test::failures);
    return nfsu_test::failures ? 1 : 0;
}
