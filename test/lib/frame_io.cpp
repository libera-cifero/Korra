#include "frame_io.hpp"
#include "json.hpp"
#include <fstream>
#include <ios>

using json = nlohmann::json;

void write_frame_expected(frame_meta config, const string &path){
    json j;
    j["bits_per_block"] = config.bits_per_block;
    j["block_size"] = config.block_size;
    j["frame_width"] = config.frame_width;
    j["frame_height"] = config.frame_height;
    j["frame_path"] = config.frame_path;
    for(int block : config.blocks){
        j["blocks"].push_back(block);
    }
    fstream blocks_file(path, ios_base::out);
    blocks_file<<j.dump(2);
    blocks_file.close();
}

frame_meta read_frame_expected(const string &path){
    fstream file(path, ios_base::in);
    string data;
    file >> data;
    file.close();

    json j = json::parse(data);
    frame_meta meta;
    meta.frame_path = j["frame_path"].get<string>();
    meta.frame_height = j["frame_height"].get<int>();
    meta.frame_width = j["frame_width"].get<int>();
    
    meta.block_size = j["block_size"];
    meta.bits_per_block = j["bits_per_block"].get<int>();
    meta.blocks = j["blocks"].get<vector<int>>();
    
    return meta;
}

void write_frame_data(uint8_t *data, int frame_width, int frame_height, const string &path){
    uint8_t bmp_file_header[14]{0x42, 0x4D};
    int file_size = frame_width * frame_height * 3;
    memcpy(bmp_file_header + 2, &file_size, 4);
    int reserved_bits = 0;
    memcpy(bmp_file_header + 6,&reserved_bits, 4);
    int pixel_offset = 54;
    memcpy(bmp_file_header + 10, &pixel_offset, 4);

    const int header_size = 40;
    uint8_t bmp_info_header[header_size];
    memcpy(bmp_info_header, &header_size, 4);
    memcpy(bmp_info_header + 4, &frame_width, 4);
    memcpy(bmp_info_header + 8, &frame_height, 4);
    int padding = (4 - (frame_width * 3) % 4) % 4;
    int row_size = frame_width * 3 + padding;
    int image_size = row_size * frame_height;
    int planes = 1, bits_per_pixel = 24, compression = 0, x_pixels_per_meter = 2835, y_pixels_per_meter = 2835;
    int colors_used = 0, important_colors = 0;
    memcpy(bmp_info_header + 12, &planes, 2);
    memcpy(bmp_info_header + 14, &bits_per_pixel, 2);
    memcpy(bmp_info_header + 16, &compression, 4);
    memcpy(bmp_info_header + 20, &image_size, 4);
    memcpy(bmp_info_header + 24, &x_pixels_per_meter, 4);
    memcpy(bmp_info_header + 28, &y_pixels_per_meter, 4);
    memcpy(bmp_info_header + 32, &colors_used, 4);
    memcpy(bmp_info_header + 36, &important_colors, 4);

    int bmp_size = 14 + header_size + image_size;
    uint8_t *bmp = new uint8_t[bmp_size];

    memcpy(bmp, bmp_file_header, 14);
    memcpy(bmp + 14, bmp_info_header, header_size);
    uint8_t *payload = bmp + 14 + header_size, *payload_src = data;
    int src_row_size = frame_width * 3;
    for(int y = 0; y < frame_height; y++) {
        memcpy(payload, payload_src, src_row_size);
        memset(payload + src_row_size, 0, row_size - src_row_size);

        payload_src += src_row_size;
        payload += row_size;
    }

    fstream frame_file(path, ios_base::out | ios_base::binary);
    frame_file.write(reinterpret_cast<char*>(bmp), bmp_size);
    frame_file.close();
    delete [] bmp;

}

uint8_t *read_frame_data(const string &path, int &width, int &height)
{
    width = 0;
    height = 0;

    std::fstream file(path, std::ios::binary | std::ios::in);
    if (!file.is_open())
        return nullptr;

    // ---------- BITMAPFILEHEADER ----------
    uint16_t signature;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;

    file.read(reinterpret_cast<char*>(&signature), sizeof(signature));
    file.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));
    file.read(reinterpret_cast<char*>(&reserved1), sizeof(reserved1));
    file.read(reinterpret_cast<char*>(&reserved2), sizeof(reserved2));
    file.read(reinterpret_cast<char*>(&dataOffset), sizeof(dataOffset));

    if (!file || signature != 0x4D42) // "BM"
        return nullptr;

    // ---------- BITMAPINFOHEADER ----------
    uint32_t headerSize;
    int32_t bmpWidth;
    int32_t bmpHeight;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t colorsImportant;

    file.read(reinterpret_cast<char*>(&headerSize), sizeof(headerSize));
    file.read(reinterpret_cast<char*>(&bmpWidth), sizeof(bmpWidth));
    file.read(reinterpret_cast<char*>(&bmpHeight), sizeof(bmpHeight));
    file.read(reinterpret_cast<char*>(&planes), sizeof(planes));
    file.read(reinterpret_cast<char*>(&bitsPerPixel), sizeof(bitsPerPixel));
    file.read(reinterpret_cast<char*>(&compression), sizeof(compression));
    file.read(reinterpret_cast<char*>(&imageSize), sizeof(imageSize));
    file.read(reinterpret_cast<char*>(&xPixelsPerMeter), sizeof(xPixelsPerMeter));
    file.read(reinterpret_cast<char*>(&yPixelsPerMeter), sizeof(yPixelsPerMeter));
    file.read(reinterpret_cast<char*>(&colorsUsed), sizeof(colorsUsed));
    file.read(reinterpret_cast<char*>(&colorsImportant), sizeof(colorsImportant));

    if (!file)
        return nullptr;

    if (headerSize != 40)
        return nullptr;

    if (planes != 1 || bitsPerPixel != 24 || compression != 0)
        return nullptr;

    width = bmpWidth;
    height = std::abs(bmpHeight);

    bool bottomUp = (bmpHeight > 0);

    const size_t rowDataSize = width * 3;
    const size_t rowSize = (rowDataSize + 3) & ~3;

    uint8_t *pixels = new uint8_t[rowDataSize * height];

    file.seekg(dataOffset, std::ios::beg);

    std::vector<uint8_t> row(rowSize);

    for (int y = 0; y < height; ++y)
    {
        file.read(reinterpret_cast<char*>(row.data()), rowSize);

        if (!file)
        {
            delete[] pixels;
            return nullptr;
        }

        int dstY = bottomUp ? (height - 1 - y) : y;

        memcpy(
            pixels + dstY * rowDataSize,
            row.data(),
            rowDataSize
        );
    }

    return pixels;
}