#include <3ds.h>
#include <sf2d.h>
#include <stdio.h>
#include <string.h>
#include "audio.h"
#include "textures.h"

int main() {
    uint8_t* buffer = NULL;
    uint32_t held;
    uint32_t umaru_pos_x = 0;
    uint32_t umaru_pos_y = 0;
    uint32_t audio_initialized = 0;

    typedef enum {UP, DOWN, LEFT, RIGHT} direction;
    direction umaru_direction = UP;

    sf2d_init();
    sf2d_set_vblank_wait(1);
    sf2d_set_3D(0);

    if(csndInit() == 0) { 
        audio_initialized = 1; 
    }
    if (audio_initialized) { 
        buffer = audio_load(buffer, "umaru.bin"); 
    }

    sf2d_texture* umaru = sf2d_create_texture_mem_RGBA8(umaru_chibi.pixel_data, 
                                                        umaru_chibi.width, 
                                                        umaru_chibi.height, 
                                                        TEXFMT_RGBA8, 
                                                        SF2D_PLACE_RAM);

    sf2d_texture* umaru_right = sf2d_create_texture_mem_RGBA8(
                                                  umaru_chibi_right.pixel_data, 
                                                  umaru_chibi_right.width, 
                                                  umaru_chibi_right.height, 
                                                  TEXFMT_RGBA8, 
                                                  SF2D_PLACE_RAM);

    sf2d_texture* umaru_left = sf2d_create_texture_mem_RGBA8(
                                                  umaru_chibi_left.pixel_data, 
                                                  umaru_chibi_left.width, 
                                                  umaru_chibi_left.height, 
                                                  TEXFMT_RGBA8, 
                                                  SF2D_PLACE_RAM);

    sf2d_texture* logo = sf2d_create_texture_mem_RGBA8(yellow_logo.pixel_data, 
                                                       yellow_logo.width, 
                                                       yellow_logo.height, 
                                                       TEXFMT_RGBA8, 
                                                       SF2D_PLACE_RAM);

    sf2d_set_clear_color(RGBA8(0xF, 0x4D, 0x8F, 0xFF));

    while(aptMainLoop()) {
        hidScanInput();
        held = hidKeysHeld();

        if (held & KEY_START) {
            break; // return to hbmenu
        }
        else if (held & KEY_DUP) {
            if (umaru_pos_y > 2) {
                umaru_pos_y-=2;
            }
            umaru_direction = UP;
        }
        else if (held & KEY_DDOWN) {
            umaru_pos_y+=2;
            if (umaru_pos_y > 209) {
                umaru_pos_y = 209;
            }
            umaru_direction = DOWN;
        }
        else if (held & KEY_DLEFT) {
            if (umaru_pos_x > 2) {
                umaru_pos_x-=2;
            }
            umaru_direction = LEFT;
        }
        else if (held & KEY_DRIGHT) {
            umaru_pos_x+=2;
            if (umaru_pos_x > 368) {
                umaru_pos_x = 368;
            }
            umaru_direction = RIGHT;
        }

        sf2d_start_frame(GFX_TOP, GFX_LEFT);
        if(umaru_direction == UP) {
            sf2d_draw_texture_scale(umaru, umaru_pos_x, umaru_pos_y, .4, .4);
        }
        else if(umaru_direction == DOWN) {
            sf2d_draw_texture_scale(umaru, umaru_pos_x, umaru_pos_y, .4, .4);
        }
        else if(umaru_direction == LEFT) {
            sf2d_draw_texture_scale(umaru_left, 
                                    umaru_pos_x, 
                                    umaru_pos_y, 
                                    .4, .4);
        }
        else if(umaru_direction == RIGHT) {
            sf2d_draw_texture_scale(umaru_right, 
                                    umaru_pos_x, 
                                    umaru_pos_y, 
                                    .4, .4);
        }
        sf2d_end_frame();

        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
        sf2d_draw_texture(logo, 0, 0);
        sf2d_end_frame();

        sf2d_swapbuffers();
    }

    sf2d_free_texture(umaru);
    sf2d_free_texture(umaru_right);
    sf2d_free_texture(umaru_left);
    sf2d_free_texture(logo);

    if (audio_initialized) {
        linearFree(buffer);
        csndExit();
    }

    sf2d_fini();
    return 0;
}
