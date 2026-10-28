# NFSU-Port (plattformübergreifend)

Ziel: Need for Speed Underground (PC, 2003) als **natives, plattformübergreifendes Programm** (macOS, Linux, Windows; 64 Bit, x86-64 und arm64), das mit den Original-Spieldaten aus `game/pc/` läuft.
Namen, Typen und Quellbaum stammen aus der nicht gestrippten GameCube-`Speed.elf` (`game/gc/`, siehe `plan/ROADMAP.md`, Abschnitt „Schlüsselfund“).
Die Arbeit ist in Pakete zerlegt. Jede Session bearbeitet **genau ein Paket**.

## Session-Ablauf
1. Lies `plan/STATUS.md` (welches Paket bereit ist), danach `plan/wp/<WP>.md` und bei Bedarf `plan/FAKTEN.md`.
2. Arbeite nur im Umfang des Pakets. Was du außerhalb findest, notierst du unter „Funde für andere Pakete“ im Log.
3. Am Ende:
   - `plan/STATUS.md` aktualisieren (Status, Datum, ein Satz Ergebnis)
   - Log nach `plan/log/<datum>-<WP>.md` schreiben (erledigt, offen, Zahlen, Funde)
   - neue geprüfte Fakten in `plan/FAKTEN.md` eintragen
   - committen, sobald WP-00 erledigt ist
4. Wenn das Paket für eine Session zu groß ist: sauber abbrechen, den Rest im Log als „Fortsetzung“ beschreiben, Status `teilweise`.

## Verzeichnisse
| Pfad | Inhalt | Regel |
|---|---|---|
| `game/pc/` | PC-Spieldaten, `Speed.exe` (Verhaltensreferenz) | **nur lesen**, nie verändern, nie committen |
| `game/gc/` | GameCube-ISO mit nicht gestrippter `Speed.elf` (Namen, Typen) | **nur lesen**; Entpacktes nach `refs/gc/` |
| `plan/` | Roadmap, Status, Pakete, Logs, Fakten | Gedächtnis des Projekts |
| `symbols/` | Symbol-Datenbank (ab WP-01): Funktionen, Daten, Typen, Module | **Quelle der Wahrheit für Namen und Typen** |
| `tools/` | Hilfsskripte (`fn.py`, `progress.py`, Emulator …) | |
| `port/` | der Port (CMake, C++17, SDL3; plattformneutral, Plattformcode nur in `platform/` und `render/`) | ab WP-10 |
| `decompiled/full/out2/` | Ghidra-Vollexport aller 10 378 PC-Funktionen | Referenz, erzeugt, nicht committen |
| `decompiled/gc/` | Ghidra-Projekt und Export der GC-`Speed.elf` (ab WP-07) | Referenz, erzeugt, nicht committen |
| `decompiled/full/build2/` | altes Kompilierexperiment | **eingefroren**, siehe `plan/ROADMAP.md` |
| `decompiled/speed/` | frühere Handarbeit (≈350 benannte Funktionen, `NOTES.md`) | Namen sind in WP-01 zu übernehmen |
| `refs/` | externe Referenzen (z. B. nfsmw-Klon), entschlüsselte Shader | nicht committen |

## Harte Regeln
- **Nicht über MCP massenhaft dekompilieren.** `rea_batch_decompile` und `procedure_pseudo_code` kosten ca. 2 000 Token pro Funktion. Pseudocode kommt aus `decompiled/full/out2` (bzw. dem neueren Export aus WP-01), und zwar über `tools/fn.py <adresse|name>`. MCP nur für Einzelfragen wie Xrefs oder Bytes.
- **`all_functions.c` (12 MB) nie komplett lesen**, nur über `tools/fn.py` oder `grep`.
- **Namen in Ghidra-Sitzungen sind flüchtig.** Jeder Name und Typ gehört nach `symbols/`. Ghidra wird per Skript daraus befüllt.
- Rekonstruierter Code kommt nach `port/src/<modul>/`, in sauberem C++ mit echten Typen. Kein Ghidra-Pseudocode mit `in_EAX`, `undefined4` oder Originaladressen.
- Jede Funktion im Port bekommt den Kommentar `// orig: 0x<adresse> <name>`, und `symbols/functions.csv` wird nachgezogen.
- Nichts installieren und nichts herunterladen ohne Rückfrage, außer was eine Nutzerentscheidung in `plan/ROADMAP.md` (D1–D7) bereits erlaubt.
- `game/pc/Speed.exe` nicht direkt ausführen. Nur mit Entscheidung D1 unter Wine oder mit D2 als einzelne Funktionen im Emulator.
- **64-Bit-Regel (ADR-001, `port/docs/ADR-001-64bit.md`)**: Strukturen, die aus Dateien kommen oder per `size / N` zerlegt werden, behalten Größe und Feldoffsets (`#pragma pack`, `static_assert`), Zeigerfelder sind `ptr32<T>` (Arena-Offset, `core/ptr32.hpp`). Zeiger auf Dinge außerhalb der Arena (GPU, SDL) gehören nie in solche Felder, dort ein `uint32_t`-Handle. Reine Laufzeitobjekte nutzen native Zeiger.
- **Nichts aus `Speed.exe` einchecken** (Tabellen, Bytes, Pseudocode): Daten nur über einen Generator in `tools/gen_*.py`, den CMake zur Bauzeit gegen `NFSU_GAME_DIR` laufen lässt (`NFSU_GENERATED_SOURCES`).
- Ehrlich berichten: Skizze ≠ rekonstruiert ≠ getestet. Die Status-Werte stehen in `plan/wp/WP-01-symbol-db.md`.

## Plattformen
- Spielcode (`port/src/game`, `formats`, `core`) enthält **keinen** Plattformcode: kein `#ifdef __APPLE__/_WIN32`, keine OS-APIs. Alles läuft über SDL3 oder `port/src/platform/`.
- Shader werden für alle SDL_GPU-Backends gebaut (Metal, Vulkan, D3D12), siehe WP-19.
- Entwickelt und getestet wird auf macOS; Linux/Windows gelten erst nach einem echten Lauf als getestet.

## Umgebung (Entwicklungsrechner)
- Apple M1, macOS arm64. Ghidra: `~/rea-stack/ghidra_12.1.4_PUBLIC/support/analyzeHeadless`
- Für Ghidra vorher: `export JAVA_HOME=/opt/homebrew/Cellar/openjdk@21/21.0.12.1/libexec/openjdk.jdk/Contents/Home; export PATH=$JAVA_HOME/bin:$PATH`
- Lange Läufe (Ghidra-Export ca. 10 min) im Hintergrund starten, nicht mit `sleep` pollen.

## Git
- Remote `origin` = privates GitHub-Repo `SloppyHenry/NFSU`, Branch `main`. Nach jedem Paket committen und pushen.
- Nie Spieldaten, `refs/`, Exporte oder Ghidra-Projekte committen (`.gitignore`). Vor jedem Commit prüfen: `git status --short`.
