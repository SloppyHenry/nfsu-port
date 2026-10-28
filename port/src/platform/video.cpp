#include "platform/video.hpp"

#include <SDL3/SDL.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/channel_layout.h>
#include <libavutil/md5.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
}

namespace nfsu {
namespace {

struct Movie {
    AVFormatContext* fmt = nullptr;
    AVCodecContext *vc = nullptr, *ac = nullptr;
    int vi = -1, ai = -1;
    SwrContext* swr = nullptr;
    AVPacket* pkt = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();
    ~Movie() {
        av_frame_free(&frame);
        av_packet_free(&pkt);
        swr_free(&swr);
        avcodec_free_context(&vc);
        avcodec_free_context(&ac);
        avformat_close_input(&fmt);
    }
};

bool open_codec(AVFormatContext* f, int idx, AVCodecContext*& ctx, std::string& err) {
    AVCodecParameters* par = f->streams[idx]->codecpar;
    const AVCodec* codec = avcodec_find_decoder(par->codec_id);
    if (!codec) { err = "kein Decoder für Strom " + std::to_string(idx); return false; }
    ctx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(ctx, par);
    if (avcodec_open2(ctx, codec, nullptr) < 0) { err = "Decoder lässt sich nicht öffnen"; return false; }
    return true;
}

bool open_movie(Movie& m, const std::string& path, MovieInfo& info, std::string& err) {
    if (avformat_open_input(&m.fmt, path.c_str(), nullptr, nullptr) < 0) { err = "kann " + path + " nicht öffnen"; return false; }
    if (avformat_find_stream_info(m.fmt, nullptr) < 0) { err = "keine Stromdaten"; return false; }
    m.vi = av_find_best_stream(m.fmt, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    m.ai = av_find_best_stream(m.fmt, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (m.vi < 0) { err = "kein Videostrom"; return false; }
    if (!open_codec(m.fmt, m.vi, m.vc, err)) return false;
    info.width = m.vc->width;
    info.height = m.vc->height;
    AVRational r = m.fmt->streams[m.vi]->avg_frame_rate;
    info.fps = r.den ? av_q2d(r) : 0;
    if (m.ai >= 0) {
        if (!open_codec(m.fmt, m.ai, m.ac, err)) return false;
        info.audio_rate = m.ac->sample_rate;
        info.audio_channels = m.ac->ch_layout.nb_channels;
        AVChannelLayout out;
        av_channel_layout_default(&out, 2);
        if (swr_alloc_set_opts2(&m.swr, &out, AV_SAMPLE_FMT_S16, m.ac->sample_rate, &m.ac->ch_layout,
                                m.ac->sample_fmt, m.ac->sample_rate, 0, nullptr) < 0 || swr_init(m.swr) < 0) {
            err = "Resampler"; return false;
        }
    }
    return true;
}

// Liest Pakete, ruft on_video / on_audio je dekodiertem Frame. on_* geben false zum Abbrechen.
template <class V, class A>
void pump(Movie& m, V on_video, A on_audio) {
    bool go = true;
    auto drain = [&](AVCodecContext* c, bool video) {
        while (go && avcodec_receive_frame(c, m.frame) == 0) {
            go = video ? on_video(m.frame) : on_audio(m.frame);
            av_frame_unref(m.frame);
        }
    };
    while (go && av_read_frame(m.fmt, m.pkt) >= 0) {
        if (m.pkt->stream_index == m.vi) { avcodec_send_packet(m.vc, m.pkt); drain(m.vc, true); }
        else if (m.pkt->stream_index == m.ai && m.ac) { avcodec_send_packet(m.ac, m.pkt); drain(m.ac, false); }
        av_packet_unref(m.pkt);
    }
    if (go) { avcodec_send_packet(m.vc, nullptr); drain(m.vc, true); }
    if (go && m.ac) { avcodec_send_packet(m.ac, nullptr); drain(m.ac, false); }
}

std::string hex(const uint8_t* d, int n) {
    static const char* h = "0123456789abcdef";
    std::string s;
    for (int i = 0; i < n; ++i) { s += h[d[i] >> 4]; s += h[d[i] & 15]; }
    return s;
}

}  // namespace

bool movie_scan(const std::string& path, MovieInfo& info, std::vector<std::string>* md5s, std::string& err) {
    Movie m;
    if (!open_movie(m, path, info, err)) return false;
    AVMD5* md5 = av_md5_alloc();
    pump(m,
         [&](AVFrame* f) {
             ++info.video_frames;
             if (md5s) {
                 av_md5_init(md5);
                 for (int p = 0; p < 3; ++p) {
                     int w = p ? (f->width + 1) / 2 : f->width, h = p ? (f->height + 1) / 2 : f->height;
                     for (int y = 0; y < h; ++y) av_md5_update(md5, f->data[p] + (size_t)y * f->linesize[p], w);
                 }
                 uint8_t out[16];
                 av_md5_final(md5, out);
                 md5s->push_back(hex(out, 16));
             }
             return true;
         },
         [&](AVFrame* f) { info.audio_samples += f->nb_samples; return true; });
    av_free(md5);
    return true;
}

int movie_play(const std::string& path, int max_frames) {
    Movie m;
    MovieInfo info;
    std::string err;
    if (!open_movie(m, path, info, err)) { std::fprintf(stderr, "%s\n", err.c_str()); return 1; }
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) { std::fprintf(stderr, "SDL: %s\n", SDL_GetError()); return 1; }
    SDL_Window* win = nullptr;
    SDL_Renderer* ren = nullptr;
    // 512x256 im Original ist anamorph auf 4:3 gedehnt dargestellt; hier 2x in 16:9-freundlicher Größe.
    if (!SDL_CreateWindowAndRenderer("NFSU", 1024, 768, SDL_WINDOW_RESIZABLE, &win, &ren)) {
        std::fprintf(stderr, "SDL: %s\n", SDL_GetError()); return 1;
    }
    SDL_SetRenderLogicalPresentation(ren, 1024, 768, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_Texture* tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, info.width, info.height);

    // Ton: Beim Demuxen liegt er hinter dem Bild, ein Strom, der nur mitläuft, wäre ständig leer (Ruckeln).
    // Darum in einem ersten Durchlauf komplett dekodieren und vorab in den Audiostrom legen (Filme <= ~20 s).
    SDL_AudioStream* as = nullptr;
    int64_t total_bytes = 0;
    if (m.ac) {
        Movie ma;
        MovieInfo ia;
        if (open_movie(ma, path, ia, err)) {
            SDL_AudioSpec spec{SDL_AUDIO_S16, 2, info.audio_rate};
            as = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
            std::vector<uint8_t> pcm;
            if (as)
                pump(ma, [](AVFrame*) { return true; }, [&](AVFrame* f) {
                    pcm.resize((size_t)f->nb_samples * 2 * sizeof(int16_t));
                    uint8_t* o = pcm.data();
                    int n = swr_convert(ma.swr, &o, f->nb_samples, (const uint8_t**)f->extended_data, f->nb_samples);
                    if (n > 0) { SDL_PutAudioStreamData(as, pcm.data(), n * 4); total_bytes += (int64_t)n * 4; }
                    return true;
                });
        }
    }

    // Bildtakt: Uhr beginnt, sobald das Gerät den Ton abzuholen beginnt (sonst sofort).
    if (as) {
        SDL_ResumeAudioStreamDevice(as);
        const uint64_t w0 = SDL_GetTicksNS();
        while (SDL_GetAudioStreamQueued(as) >= total_bytes && SDL_GetTicksNS() - w0 < 500000000ull) SDL_DelayNS(500000);
    }
    const double frame_s = info.fps > 0 ? 1.0 / info.fps : 1.0 / 30.0;
    const uint64_t t0 = SDL_GetTicksNS();
    uint64_t shown = 0;
    bool quit = false;
    double late_max = 0, late_sum = 0;

    pump(m,
         [&](AVFrame* f) {
             const double due = (double)shown * frame_s;  // Soll-Zeit des Bildes
             for (;;) {
                 SDL_Event e;
                 while (SDL_PollEvent(&e))
                     if (e.type == SDL_EVENT_QUIT || e.type == SDL_EVENT_KEY_DOWN || e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) quit = true;
                 if (quit) return false;
                 double now = (double)(SDL_GetTicksNS() - t0) / 1e9;
                 if (now >= due) { late_max = std::max(late_max, now - due); late_sum += now - due; break; }
                 SDL_DelayNS((Uint64)std::min((due - now) * 1e9, 4e6));
             }
             SDL_UpdateYUVTexture(tex, nullptr, f->data[0], f->linesize[0], f->data[1], f->linesize[1], f->data[2], f->linesize[2]);
             SDL_RenderClear(ren);
             SDL_RenderTexture(ren, tex, nullptr, nullptr);
             SDL_RenderPresent(ren);
             ++shown;
             return !(max_frames && (int)shown >= max_frames);
         },
         [](AVFrame*) { return true; });  // Ton schon im Strom
    if (std::getenv("NFSU_AVSTATS") && shown)
        std::fprintf(stderr, "Bilder %llu, Verspaetung zur Sollzeit: mittel %.1f ms, max %.1f ms\n", (unsigned long long)shown,
                     late_sum / (double)shown * 1e3, late_max * 1e3);
    if (as && !quit) while (SDL_GetAudioStreamQueued(as) > 0 && !quit) SDL_Delay(20);
    if (as) SDL_DestroyAudioStream(as);
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    return 0;
}

}  // namespace nfsu
