// EA-Audiostreams (.ast): Folge von Strömen, je SCHl (Kopf), SCCl (Blockzahl), SCDl* (Daten), SCEl (Ende),
// danach Nullen bis zur nächsten Ausrichtung. Alle Tags und Größen little-endian, Größe inkl. 8-Byte-Kopf.
// Codec in allen Dateien: EA-XA R2 (ADPCM, 28 Samples je Frame), siehe plan/FAKTEN.md.
#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace nfsu::ea {

struct StreamHeader {
    uint32_t channels = 1;
    uint32_t sample_rate = 0;
    uint32_t num_samples = 0;   // je Kanal, laut Kopf (Element 0x85)
    uint32_t codec = 0;         // Element 0x80, 2 = EA-XA R2
};

struct Stream {
    size_t offset = 0;          // Beginn des SCHl
    size_t size = 0;            // bis einschließlich SCEl
    StreamHeader hdr;
    uint32_t blocks = 0;        // Zahl der SCDl
};

// Zerlegt eine .ast-Datei. Leer bei Erfolg, sonst Fehlertext.
std::string scan_streams(const uint8_t* buf, size_t len, std::vector<Stream>& out);

// Dekodiert einen Strom zu interleaved int16 (channels * Samples). Leer bei Erfolg.
std::string decode_stream(const uint8_t* buf, size_t len, const Stream& s, std::vector<int16_t>& pcm);

// RIFF/WAVE (PCM16) in einen Puffer schreiben.
std::vector<uint8_t> make_wav(const int16_t* pcm, size_t frames, uint32_t channels, uint32_t rate);

// --- Soundbänke (.abk) ---------------------------------------------------------------------------
// ABKC-Kopf (alle Felder u32 LE): +4 Version (0x101), +0x14 Dateigröße, +0x18 Größe des Objektabbilds,
// +0x20/+0x24 Offset/Größe der Sampledatenfläche A, +0x28/+0x2c dasselbe für Fläche B, +0x30/+0x34/+0x38
// Offsets der Relozierungstabellen (je u32 Anzahl + Einträge). Die Fläche beginnt mit "BNKl":
// +4 u16 Version (5), +6 u16 Soundzahl, +0x14 Tabelle u32 rel (0 = leer), Ziel = Position des
// Tabelleneintrags + rel, dort "PT\0\0" + Elemente (Tag, Länge, Wert BE) bis 0xFF.
struct BnkSound {
    int area = 0;
    uint32_t index = 0;
    StreamHeader hdr;           // Rate Standard 22 050, immer 1 Kanal
    uint32_t loop_start = 0, loop_end = 0;   // Elemente 0x86 / 0x87
    uint32_t codec2 = 0xFFFFFFFFu;           // Element 0xA0: 8 = PCM16 LE, fehlt = EA-XA R2
    size_t data_offset = 0;                  // absolut in der Datei
    size_t data_limit = 0;                   // Beginn des nächsten Sounds bzw. Flächenende
};

struct Bank {
    uint32_t version = 0, file_size = 0, header_size = 0;
    uint32_t area_off[2] = {0, 0}, area_size[2] = {0, 0};
    uint32_t table_count[3] = {0, 0, 0};
    std::vector<BnkSound> sounds;
};

std::string parse_abk(const uint8_t* buf, size_t len, Bank& bank);
// used (optional): tatsächlich gelesene Bytes.
std::string decode_bnk_sound(const uint8_t* buf, size_t len, const BnkSound& s, std::vector<int16_t>& pcm,
                             size_t* used = nullptr);

}  // namespace nfsu::ea
