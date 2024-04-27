#pragma once

#include "syati.h"
#define arrsize(item) sizeof(item)/sizeof(item[0])

/*
* JUTTextureHolder
*  
* A texture pointer array holder that constructs and destructs
* JUTTexture pointers as they are needed or no longer needed.
* 
* By Lord-Giganticus
*/

class JUTTextureHolder {
    public:
    const size_t SIZE;
    JUTTexture** Textures;
    void SetTexture(u8 pos, JUTTexture* texture) {
        // Get Address of pointers, delete old one if adresses dont match, otherwise do nothing.
        u64 ladr = (u64)Textures[pos];
        u64 radr = (u64)texture;
        if (ladr != radr) {
            if (ladr != NULL) {
                OSReport("(JUTTextureHolder) Texture %d has been deleted.\n", pos);
                delete Textures[pos];
            } else {
                OSReport("(JUTTextureHolder) Texture %d now has a value.\n", pos);
            }
            Textures[pos] = texture;
            delete texture;
        } else {
            OSReport("(JUTTextureHolder) Address did not change, not changing Texture %d\n", pos);
        }
    }

    JUTTexture* operator[](u8 pos) {
        return Textures[pos];
    }

    JUTTextureHolder(size_t size) : SIZE(size) {   
        OSReport("(JUTTextureHolder) Size: %d\n", SIZE);
        Textures = new JUTTexture*[size];
        for (int i = 0; i < SIZE; i++) {
            Textures[i] = NULL;
        }
    }

    ~JUTTextureHolder() {
        delete [] Textures;
    }
};