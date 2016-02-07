#include "audio.h"
#include <3ds.h>
#include <stdio.h>

uint8_t* audio_load(uint8_t* buf, const char *audio) {
    FILE* file = fopen(audio, "rb");
    off_t size = 0;

    if (file != NULL) {
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        buf = linearAlloc(size);

        fseek(file, 0, SEEK_SET);
        fread(buf, 1, size, file);

        if (ferror(file)) {
            return NULL;
        }
        fclose(file);
    }

    uint8_t channel = 0x8;
    uint32_t sample_rate = 44100;
    uint32_t flags = SOUND_FORMAT_16BIT | SOUND_ONE_SHOT;

    //channel++;
    //channel%=8;

    GSPGPU_FlushDataCache(buf, size);
    csndPlaySound(channel, flags, sample_rate, 1.0, 0.0, buf, buf, size);
    return buf;
}
