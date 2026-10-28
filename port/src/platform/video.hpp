// Filmabspieler (EA MAD, siehe ADR-002). Demux, Bild- und Tondekodierung über libavformat/libavcodec.
#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace nfsu {

struct MovieInfo {
    int width = 0, height = 0;
    double fps = 0;
    int audio_rate = 0, audio_channels = 0;
    uint64_t video_frames = 0;
    uint64_t audio_samples = 0;  // je Kanal
};

// Dekodiert den ganzen Film ohne Fenster. md5s erhält je Bild den MD5 der gepackten Y/U/V-Ebenen
// (hex), gleichbedeutend mit `ffmpeg -f framemd5` auf yuv420p. Rückgabe false mit err bei Fehlern.
bool movie_scan(const std::string& path, MovieInfo& info, std::vector<std::string>* md5s, std::string& err);

// Spielt den Film in einem Fenster mit Ton. Abbruch mit Esc, Leertaste oder Eingabe; Rückgabe Exit-Code.
int movie_play(const std::string& path, int max_frames = 0);

}  // namespace nfsu
