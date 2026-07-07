#include <cstring>
#include <stdexcept>
#include <vector>
#include <limits>
#include <algorithm>
#include <cstdio>

// -------------------- YCbCr --------------------

struct YCbCr8 {
    int Y, Cb, Cr;
};

inline YCbCr8 rgb_to_ycbcr(int color)
{
    int r = (color >> 16) & 255;
    int g = (color >> 8)  & 255;
    int b =  color        & 255;

    int Y  = ( 77 * r + 150 * g +  29 * b) >> 8;
    int Cb = ((-43 * r -  85 * g + 128 * b) >> 8) + 128;
    int Cr = ((128 * r - 107 * g -  21 * b) >> 8) + 128;

    return {Y, Cb, Cr};
}

// -------------------- metrics --------------------

struct Weights {
    float wY;
    float wCb;
    float wCr;
};

inline float dist2(const YCbCr8& a, const YCbCr8& b, const Weights& w)
{
    float dY  = float(a.Y  - b.Y);
    float dCb = float(a.Cb - b.Cb);
    float dCr = float(a.Cr - b.Cr);

    return dY*dY*w.wY + dCb*dCb*w.wCb + dCr*dCr*w.wCr;
}

inline float rgb_distance2(int c1, int c2)
{
    float dr = float(((c1 >> 16) & 255) - ((c2 >> 16) & 255));
    float dg = float(((c1 >> 8)  & 255) - ((c2 >> 8)  & 255));
    float db = float(( c1        & 255) - ( c2        & 255));

    return dr * dr + dg * dg + db * db;
}
// -------------------- FPS --------------------

std::vector<int> fps_ycbcr_palette(
    const std::vector<int>& candidates_rgb,
    int k,
    const Weights& w)
{
    int n = (int)candidates_rgb.size();

    std::vector<YCbCr8> pts(n);
    for (int i = 0; i < n; ++i)
        pts[i] = rgb_to_ycbcr(candidates_rgb[i]);

    std::vector<float> minDist(n, std::numeric_limits<float>::max());
    std::vector<int> selected;
    selected.reserve(k);

    // стартовая точка
    int first = 0;
    selected.push_back(candidates_rgb[first]);

    YCbCr8 last = pts[first];

    for (int i = 0; i < n; ++i)
        minDist[i] = dist2(pts[i], last, w);

    for (int it = 1; it < k; ++it)
    {
        int best = -1;
        float bestVal = -1.0f;

        for (int i = 0; i < n; ++i)
        {
            float d = dist2(pts[i], last, w);
            minDist[i] = std::min(minDist[i], d);

            if (minDist[i] > bestVal)
            {
                bestVal = minDist[i];
                best = i;
            }
        }

        selected.push_back(candidates_rgb[best]);
        last = pts[best];
    }

    return selected;
}

std::vector<int> fps_rgb_palette(
    const std::vector<int>& candidates_rgb,
    int k)
{
    const int n = (int)candidates_rgb.size();

    if (n == 0 || k <= 0)
        return {};

    if (k > n)
        k = n;

    std::vector<float> minDist(n, std::numeric_limits<float>::max());
    std::vector<bool> used(n, false);
    std::vector<int> selected;
    selected.reserve(k);

    // Первая точка
    int first = 0;
    used[first] = true;
    selected.push_back(candidates_rgb[first]);

    // Инициализация расстояний
    for (int i = 0; i < n; ++i)
        minDist[i] = rgb_distance2(candidates_rgb[i], candidates_rgb[first]);

    // Основной цикл FPS
    for (int it = 1; it < k; ++it)
    {
        int best = -1;
        float bestVal = -1.0f;

        for (int i = 0; i < n; ++i)
        {
            if (used[i])
                continue;

            if (minDist[i] > bestVal)
            {
                bestVal = minDist[i];
                best = i;
            }
        }

        if (best == -1)
            break;

        used[best] = true;
        selected.push_back(candidates_rgb[best]);

        // Обновляем минимальные расстояния
        for (int i = 0; i < n; ++i)
        {
            if (used[i])
                continue;

            float d = rgb_distance2(candidates_rgb[i], candidates_rgb[best]);
            if (d < minDist[i])
                minDist[i] = d;
        }
    }

    return selected;
}

// -------------------- main --------------------

int main(int argc, char **argv)
{
    if(argc < 3){
        throw new std::runtime_error("Arg count less than 3!\n");
    }

    int bits = atoi(argv[1]);
    if(bits < 1 || bits > 8){
        throw new std::runtime_error("Invalid bit count!");
    }
    int count = 1 << bits;
    int step = 256 / count;

    std::vector<int> candidates;
    candidates.reserve(256 * 256 * 256 / count);

    for (int r = 0; r < 256; r += step)
        for (int g = 0; g < 256; g += step)
            for (int b = 0; b < 256; b += step)
                candidates.push_back((r << 16) | (g << 8) | b);


    std::vector<int> palette;
    if(strcmp(argv[2], "rgb") == 0){
        palette = fps_rgb_palette(candidates, count);
    }
    else if(strcmp(argv[2], "ycbcr") == 0){
        /*Weights w{
            1.0f / 9.0f,    // Y
            1.0f / 81.0f,   // Cb
            1.0f / 81.0f    // Cr
        };*/
        Weights w{
            1,1,1
        };

        palette = fps_ycbcr_palette(candidates, count, w);
    }
    else{
        throw std::runtime_error("Invalid color mode!");
    }

    for (int i = 0; i < count; i++)
    {
        int c = palette[i];
        int r = (c >> 16) & 255;
        int g = (c >> 8)  & 255;
        int b =  c        & 255;

        printf("\"#%02X%02X%02X\",\n", r, g, b);
    }

    return 0;
}