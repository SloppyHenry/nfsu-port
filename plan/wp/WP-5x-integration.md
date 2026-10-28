# WP-50 … WP-53 Integration bis zum spielbaren Programm (macOS-`.app`, Linux, Windows)

**Phase 5 · Modell: Opus · Voraussetzungen: siehe je Paket**

## WP-50 Boot bis Frontend
- Voraussetzungen: Basis-, Datei-, Ereignis- und Frontend-Module aus Phase 3, WP-40, 41, 43, (16 für Intros)
- `apps/nfsu` durchläuft `game_main` → `game_init_subsystems` → Zustandsmaschine (`LoadingFrontEnd`) und zeigt das Hauptmenü
- Fertig, wenn: Das Hauptmenü ist bedienbar, und der Vergleich mit den Referenzbildern (WP-06, falls vorhanden) ist im Log.

## WP-51 Erstes Rennen
- Voraussetzungen: Streaming, Welt und Render, Kamera, Fahrzeug und Physik, Rennlogik, HUD, Audio
- Schnelles Rennen (Circuit) auf einer Strecke fahren und beenden
- Physik gegen das Original prüfen: Gleiche Eingabefolge → gleiche Fahrzeugbahn. Mit dem Emulator (WP-05) über einzelne Physikschritte vergleichen.

## WP-52 Vollständigkeit
- Alle Rennmodi, die Karriere (Underground-Modus), Tuning bzw. Garage, Speicherstand laden und speichern, Optionen, alle Strecken
- `progress.py`: Alle `rekonstruieren`-Module sind fertig.
- Fehlerliste aus Spieltests als `plan/BUGS.md`, als Grundlage für Nacharbeit-Pakete

## WP-53 Pakete je Plattform
- macOS: `NFSU.app` mit Icon (aus `NFSU_icon.ico`), Info.plist, Ad-hoc-Signatur
- Linux: Programm + `.desktop`-Datei (AppImage optional); Windows: `.exe` mit Icon-Ressource
- CI (GitHub Actions) baut alle drei Plattformen ohne Spieldaten bis auf die Tabellenerzeugung (braucht `Speed.exe`, daher in der CI nur Kompilierprüfung mit Platzhaltern)
- Die Spieldaten werden **nicht** eingebettet. Beim ersten Start wird der Ordner gewählt und der Pfad gespeichert.
- Fertig, wenn: Das Programm startet auf jeder Plattform per Doppelklick auf einem frischen Benutzerkonto, nachdem man den Datenordner gewählt hat.
