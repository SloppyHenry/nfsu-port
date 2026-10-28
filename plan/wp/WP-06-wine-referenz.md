# WP-06 (optional) Original unter Wine/CrossOver als Referenz

**Phase 1 · Modell: Sonnet · Umfang: 1 Session · Voraussetzungen: keine · Entscheidung: D1**

## Ziel
Wenn das Original auf dem M1 unter Wine oder CrossOver läuft, gibt es Referenzmaterial: Bildschirmfotos aller Menüs und Strecken, Ladeabläufe, Dateizugriffe und D3D9-Aufrufprotokolle. Damit lässt sich später das Verhalten des Ports vergleichen.

## Schritte
1. Der Nutzer wählt und installiert Wine bzw. CrossOver (Installation nur auf Zusage).
2. Klären, welche `Speed.exe` gestartet wird (`game/pc/Speed.exe`). Das Spiel in Wine auf einer **Kopie** des Ordners starten, denn es schreibt evtl. Konfiguration oder Spielstände. Die vorliegende ist von 2014 und vermutlich ohne CD-Prüfung gepatcht (`patches.ini`, `speed.undo`). Notieren, ob sie unverändert läuft.
3. Startet das Spiel, dann aufnehmen:
   - Bildschirmfotos und kurze Videos nach `refs/screens/` (Frontend, eine Strecke je Gebiet, HUD, Garage)
   - Dateizugriffsprotokoll (`WINEDEBUG=+file` o. ä., gefiltert) → welche Dateien in welcher Reihenfolge beim Booten und beim Laden einer Strecke
   - optional ein D3D9-Protokoll eines Frames (Renderzustände, Shader, Texturformate)
4. Befunde nach `plan/FAKTEN.md` (Abschnitt „Laufzeitbeobachtungen“).

## Fertig, wenn
- Dokumentiert ist, ob und wie das Original läuft, oder warum nicht.
- Falls es läuft: Referenzbilder und Ladeprotokoll liegen vor.

## Später (nicht Teil dieses Pakets)
Eine Hook-DLL nach dem Vorbild von re3 (Originalfunktionen einzeln durch eigene ersetzen und im laufenden Spiel testen) bräuchte mingw-w64 und eine 32-Bit-Windows-Version des Codes. Das lohnt sich nur, wenn der Emulator (WP-05) für zustandsreiche Subsysteme nicht reicht. Diese Entscheidung fällt frühestens in Phase 3.
