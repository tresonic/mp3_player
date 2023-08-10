#pragma once

#define min(a, b) a > b ? b : a

namespace config {
    const int PIN_I2S_CLK_BASE = 26;
    const int PIN_I2S_DATA = 28;

    const int FILE_BUF_SIZE = 2048;

    const int AUDIOBUFFER_SIZE = 4096;
    const int AUDIOBUFFER_NUM = 4;

    const int MAX_OPEN_FILES = 5;
}