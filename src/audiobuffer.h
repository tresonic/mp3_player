#pragma once

#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"

#include "config.h"

using config::AUDIOBUFFER_SIZE;
using config::AUDIOBUFFER_NUM;

class Audiobuffer {
public:
    void reset() {
        mWrIdx = 0;
        mDmaLockedBuffer = 1;
        //memset();
    }

    int getNumWritableSamples() {
        int dmaIdx = mDmaLockedBuffer * AUDIOBUFFER_SIZE;
        if (dmaIdx < mWrIdx) {
            return count_of(mBuffer) - mWrIdx + dmaIdx;
        }
        return dmaIdx - mWrIdx;
    }

    int writeSamples(int16_t* src, int maxSamples) {
        int n = min(maxSamples, getNumWritableSamples());
        for (int i=0; i<n; ++i) {
            mBuffer[mWrIdx++] = src[i];
            if (mWrIdx >= count_of(mBuffer)) {
                mWrIdx = 0;
            }
        }
        return n;
    }

    int16_t* getNextDmaBlock() {
        int newLockedBuffer = mDmaLockedBuffer + 1;
        if (newLockedBuffer >= AUDIOBUFFER_NUM) {
            newLockedBuffer = 0;
        }

        if (mWrIdx >= newLockedBuffer * AUDIOBUFFER_SIZE && mWrIdx <= (newLockedBuffer + 1) * AUDIOBUFFER_SIZE) {
            puts("dma overtook write!!!");
            return NULL;
        }
        printf("new dma block: %i\n", newLockedBuffer);
        mDmaLockedBuffer = newLockedBuffer;
        return &mBuffer[mDmaLockedBuffer*AUDIOBUFFER_SIZE];
    }
private:
    int16_t mBuffer[AUDIOBUFFER_NUM * AUDIOBUFFER_SIZE];
    int mWrIdx = 0;
    int mDmaLockedBuffer = 1;
};

Audiobuffer audiobuffer;