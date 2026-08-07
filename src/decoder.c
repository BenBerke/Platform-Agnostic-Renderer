//
// Created by berke on 8/6/2026.
//

#include "../headers/decoder.h"
#include "../headers/mmu.h"
#include "../headers/debug.h"

typedef struct {
    const u8 *data;
    isize size;
    isize byte_idx;
    int bit_idx;
    bool failed;
} BitStream;

static u32 read_bits(BitStream* bs, const int count) {
    if (bs->failed || count < 0 || count > 32) {
        g_print("DEFLATE EOF: byte=%l size=%l bit=%d\n", (i64)bs->byte_idx, (i64)bs->size, bs->bit_idx);

        bs->failed = true;
        return 0;
    }
    u32 val = 0;
    for (int i = 0; i < count; i++) {
        if (bs->byte_idx >= bs->size) {
            g_print("DEFLATE EOF: byte=%l size=%l bit=%d\n", (i64)bs->byte_idx, (i64)bs->size, bs->bit_idx);

            bs->failed = true;
            return 0;
        }
        u32 bit = (bs->data[bs->byte_idx] >> bs->bit_idx) & 1u;
        val |= bit << i;
        bs->bit_idx++;
        if (bs->bit_idx == 8) {
            bs->bit_idx = 0;
            bs->byte_idx++;
        }
    }

    return val;
}

static void align_bytes(BitStream* bs) {
    if (bs->bit_idx > 0) {
        bs->bit_idx = 0;
        bs->byte_idx++;
    }
}

typedef struct {
    u16 counts[16];
    u16 symbols[288];
} HuffmanTree;

static void build_huffman(HuffmanTree* ht, const u8* lengths, const int num_symbols) {
    g_memset(ht, 0, sizeof(HuffmanTree));
    u16 offset[16] = {0};

    for (int i = 0; i < num_symbols; i++)  if (lengths[i] > 0 && lengths[i] < 16) ht->counts[lengths[i]]++;

    u16 off = 0;
    for (int len = 1; len < 16; len++) {
        offset[len] = off;
        off += ht->counts[len];
    }
    for (int i = 0; i < num_symbols; i++) if (lengths[i] > 0 && lengths[i] < 16) ht->symbols[offset[lengths[i]]++] = (u16)i;
}

static int decode_symbol(BitStream* bs, const HuffmanTree* ht) {
    int code = 0;
    int first = 0;
    int symbol_index = 0;

    for (int len = 1; len < 16; len++) {
        code |= (int)read_bits(bs, 1);
        if (bs->failed) return -1;
        const int count = ht->counts[len];
        if (code >= first && code < first + count) {
            const int index = symbol_index + (code - first);

            if (index < 0 || index >= 288) return -1;

            return ht->symbols[index];
        }

        symbol_index += count;
        first = (first + count) << 1;
        code <<= 1;
    }

    return -1;
}

static const u16 LENGTH_BASE[29] = {
    3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
    35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258
};
static const u8 LENGTH_EXTRA[29] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
    3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0
};

static const u16 DIST_BASE[30] = {
    1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
    257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577
};
static const u8 DIST_EXTRA[30] = {
    0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
    7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13
};

static const u8 CL_ORDER[19] = {
    16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
};

static void get_fixed_trees(HuffmanTree* lt, HuffmanTree* dt) {
    u8 l_lengths[288];
    for (int i = 0; i <= 143; i++) l_lengths[i] = 8;
    for (int i = 144; i <= 255; i++) l_lengths[i] = 9;
    for (int i = 256; i <= 279; i++) l_lengths[i] = 7;
    for (int i = 280; i <= 287; i++) l_lengths[i] = 8;
    build_huffman(lt, l_lengths, 288);

    u8 d_lengths[32];
    for (int i = 0; i < 32; i++) d_lengths[i] = 5;
    build_huffman(dt, d_lengths, 32);
}

static int inflate_block_data(BitStream* bs, HuffmanTree* lt, HuffmanTree* dt, u8** outbuf, isize* outlen, isize* cap) {
    for (;;) {
        int sym = decode_symbol(bs, lt);

        if (bs->failed) {
            g_print("inflate_block_data: bistream exhausted\n");
            return -1;
        }
        if (sym < 0) {
            g_print("inflate_block_data: invalid literal_symbol\n");
            return -1;
        }
        if (sym == 256) break;

        if (sym < 256) {
            if (*outlen >= *cap) {
                g_print("DEFLATE OUTPUT OVERFLOR: out=%l, cap=%l\n", *outlen, *cap);
                return -1;
            }
            (*outbuf)[(*outlen)++] = (u8)sym;
        }
        else if (sym >= 257 && sym <= 285) {
            const int l_ldx = sym - 257;
            const int length = LENGTH_BASE[l_ldx] + read_bits(bs, LENGTH_EXTRA[l_ldx]);

            if (bs->failed) {
                g_print("Failed reading length bits\n");
                return -1;
            }

            const int d_sym = decode_symbol(bs, dt);
            if (d_sym < 0 || d_sym > 29) {
                g_print("INVALID DISTANCE SYMBOL: %d byte=%l bit =%d\n", d_sym, bs->byte_idx, bs->bit_idx);
                return -1;
            }

            const int distance = DIST_BASE[d_sym] + read_bits(bs, DIST_EXTRA[d_sym]);

            if (bs->failed) {
                g_print("Failed reading distance bit \n");
                return -1;
            }
            if (distance <= 0 || distance > *outlen) {
                g_print("INVALID DISTANCE: distance=%d, out=%l\n", distance, *outlen);
                return -1;
            }
            if (*outlen > *cap || length > *cap - *outlen) {
                g_print("DEFLATE OUTPUT OVERFLOW. out=%l length=%d, cap=%l\n", *outlen, length, *cap);
                return -1;
            }

            for (int i = 0; i < length; i++) {
                (*outbuf)[*outlen] = (*outbuf)[*outlen - distance];
                (*outlen)++;
            }
        }
        else {
            g_print("INVALUD LITERAL/LENGTH SYMBOl %d\n", sym);
            return -1;
        }
    }
    return 0;
}

static u8* inflate_zlib(const u8 *in, usize in_size, usize expected_size, usize *out_size) {
    g_print("inflate zlib. Input size = %l", in_size);
    if (in_size < 6) return NULL;

    const u8 cmf = in[0];
    const u8 flg = in[1];

    if ((cmf & 0x0F) != 8 || ((cmf * 256 + flg) % 31) != 0 || (flg & 0x20)) return NULL;

    BitStream bs = {
        .data = in + 2,
        .size = (isize)in_size - 6,
        .byte_idx = 0,
        .bit_idx = 0,
        .failed = false
    };

    isize cap = (isize)expected_size;
    isize len = 0;

    u8* out = (u8*)g_malloc((usize)cap);
    if (!out) {
        g_print("Failed to allocate output %l bytes\n", cap);
        return NULL;
    }

    int bfinal = 0;
    while (!bfinal) {
        // g_print("block start byte = %l, bit=%d, out=%l, cap = %l\n", (u64)bs.byte_idx, bs.bit_idx, len, cap);
        bfinal = read_bits(&bs, 1);
        int btype = read_bits(&bs, 2);

        // g_print("block header: final=%d, type %d\n", bfinal, btype);

        if (bs.failed) {
            g_print("block header failed to read");
            g_free(out);
            return NULL;
        }

        if (btype == 0) { // Uncompressed
            align_bytes(&bs);
            if (bs.byte_idx + 4 > bs.size) { g_free(out); return NULL; }
            u16 blen = bs.data[bs.byte_idx] | (bs.data[bs.byte_idx + 1] << 8);
            u16 bnlen = bs.data[bs.byte_idx + 2] | (bs.data[bs.byte_idx + 3] << 8);
            bs.byte_idx += 4;
            if ((u16)blen != (u16)~bnlen || bs.byte_idx + blen > bs.size) {g_free(out); return NULL; }
            if (len > cap || blen > cap - len) {
                g_print("UNCOMPRESSED OUTPUT OVERFLOW len=%l, block=%l cap =%l\n", len, blen, cap);
                g_free(out);
                return NULL;
            }
            g_memcpy(out + len, bs.data + bs.byte_idx, blen);
            len += blen;
            bs.byte_idx += blen;
        }
        else if (btype == 1) { // Fixed huffman
            HuffmanTree lt, dt;
            get_fixed_trees(&lt, &dt);
            if (inflate_block_data(&bs, &lt, &dt, &out, &len, &cap) != 0) { g_free(out); return NULL; }
        }
        else if (btype == 2) { // Dynamic huffman
            int hlit = read_bits(&bs, 5) + 257;
            int hdist = read_bits(&bs, 5) + 1;
            int hclen = read_bits(&bs, 4) + 4;

            if (bs.failed || hlit > 286 || hdist > 32 || hclen > 19) {
                g_print("INVALID DYNAMIC HEADER: failed %d, hlit %d, hdist %d, hclen=%d\n", bs.failed, hlit, hdist, hclen);
                g_free(out); return NULL;
            }

            u8 cl_lengths[19] = {0};
            for (int i = 0; i < hclen; i++) {
                cl_lengths[CL_ORDER[i]] = read_bits(&bs, 3);
                if (bs.failed) {
                    g_print("FAILED READING CODE LENGTH %d\n", i);
                    g_free(out);
                    return NULL;
                }
            }
            HuffmanTree cl_tree;
            build_huffman(&cl_tree, cl_lengths, 19);

            u8 combined[320] = {0};
            const int num_total = hlit + hdist;
            int idx = 0;
            while (idx < num_total) {
                const int sym = decode_symbol(&bs, &cl_tree);
                if (sym < 0) {
                    g_print("CODE-LENGTH symbol failed idx %d, total %d, byte %l bit %d\n", idx, num_total, (u64)bs.byte_idx, bs.bit_idx);
                    g_free(out);
                    return NULL;
                }
                if (sym < 16) combined[idx++] = (u8)sym;
                else if (sym == 16) {
                    if (idx == 0) {
                        g_print("INVALID REPEAT-16 IS AT ZERO\n");
                        g_free(out);
                        return NULL;
                    }
                    int count = (int)read_bits(&bs, 2) + 3;
                    const u8 prev = idx > 0 ? combined[idx - 1] : 0;
                    while (count-- && idx < num_total) combined[idx++] = prev;
                }
                else if (sym == 17) {
                    int count = read_bits(&bs, 3) + 3;
                    if (idx + count > num_total) g_print("REPEAT 17 overflow idx=%d, count %d, total %d \n", idx, count, num_total);
                    while (count-- && idx < num_total) combined[idx++] = 0;
                }
                else if (sym == 18) {
                    int count = read_bits(&bs, 7) + 11;
                    if (idx + count > num_total) g_print("REPEAT 18 overflow idx=%d, count %d, total %d \n", idx, count, num_total);
                    while (count-- && idx < num_total) combined[idx++] = 0;
                }
                else {
                    g_print("INVALID CODE SYMBOL: %d\n", sym);
                    g_free(out);
                    return NULL;
                }

                if (bs.failed) {
                    g_print("BITSTREAM FAILED DURING CODE LENFTH idx = %d\n", idx);
                    g_free(out);
                    return NULL;
                }
            }

           //  g_print("Dynamic tree decoded idx %d total %d byte %l bit %d\n", idx, num_total, (u64)bs.byte_idx, bs.bit_idx);

            HuffmanTree lt, dt;
            build_huffman(&lt, combined, hlit);
            build_huffman(&dt, combined + hlit, hdist);
            if (inflate_block_data(&bs, &lt, &dt, &out, &len, &cap) != 0) {g_free(out); return NULL;}
        }
        else {
            g_print("UNKNOWN BTYPE type=%d", btype);
            g_free(out);
            return NULL;
        }
    }

    if ((usize)len != expected_size) {
        g_print("DEFLATE SIZE MISMATCH: got=%l, expected %l\n", (u64)len, (u64)expected_size);
        g_free(out);
        return NULL;
    }

    *out_size = len;
    return out;
}

static u8 paeth_predictor(const int a, const int b, const int c) {
    const int p = a + b - c;
    const int pa = abs(p - a), pb = abs(p - b), pc = abs(p - c);
    if (pa <= pb && pa <= pc) return (u8)a;
    if (pb <= pc) return (u8)b;
    return (u8)c;
}

static u32 read_u32_be(const u8 *p) {
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | ((u32)p[3]);
}

Texture png_to_bitstream(const char* filepath) {
    Texture ret_texture = {
        .data = NULL,
        .w = -1,
        .h = -1,
    };
    char* file_data = NULL;
    u64 fsize = 0;

    if (!g_read_file(filepath, &file_data, &fsize)) return ret_texture;

    static const u8 PNG_SIG[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
    if (fsize < 8 || g_memcmp(file_data, PNG_SIG, 8) != 0) { g_free(file_data); return ret_texture; }

    usize pos = 8;
    u32 width = 0, height = 0;
    u8 bit_depth = 0, color_type = 0;

    u8* idat_buf = NULL;
    usize idat_size = 0, idat_cap = 0;

    while (pos + 12 <= (usize)fsize) {
        u32 len = ((u32)((u8*)file_data)[pos + 0] << 24) | ((u32)((u8*)file_data)[pos + 1] << 16) |
                  ((u32)((u8*)file_data)[pos + 2] << 8)  | ((u32)((u8*)file_data)[pos + 3]);

        const u8 *type = (u8*)file_data + pos + 4;
        const u8 *data = (u8*)file_data + pos + 8;
        if (pos + 12 + len > (usize)fsize) break;

        if (g_memcmp(type, "IHDR", 4) == 0) {
            width = ((u32)data[0] << 24) | ((u32)data[1] << 16) | ((u32)data[2] << 8) | ((u32)data[3]);
            height = ((u32)data[4] << 24) | ((u32)data[5] << 16) | ((u32)data[6] << 8) | ((u32)data[7]);
            bit_depth = data[8];
            color_type = data[9];

            if (bit_depth != 8 || (color_type != 2 && color_type != 6) || data[12] != 0) {
                g_free(idat_buf);
                g_free(file_data);
                return ret_texture;
            }
        }
        else if (g_memcmp(type, "IDAT", 4) == 0) {
            if (idat_size + len > idat_cap) {
                while (idat_size + len > idat_cap) idat_cap = idat_cap == 0 ? 8192 : idat_cap * 2;
                idat_buf = (u8*)g_realloc(idat_buf, idat_cap);
            }
            g_memcpy(idat_buf + idat_size, data, len);
            idat_size += len;
        }
        else if (g_memcmp(type, "IEND", 4) == 0) break;

        pos += 12 + len;
    }

    g_free(file_data);
    if (!idat_buf) return ret_texture;

    const int bpp = (color_type == 6) ? 4 : 3;
    const isize stride = (isize)width * bpp;
    const isize expected_size = (isize)height * (1 + stride);

    // g_print("PNG: width=%d, height=%d, bpp=%d, expected = %l\n", width, height, bpp, expected_size);

    usize raw_size = 0;
    u8 *raw_data = inflate_zlib(idat_buf, idat_size, expected_size, &raw_size);
    g_free(idat_buf);

    if (!raw_data) return ret_texture;

    if (raw_size < expected_size) {
        g_free(raw_data);
        return ret_texture;
    }

    u8 *recon = (u8*)g_malloc(height * stride);
    for (u32 y = 0; y < height; y++) {
        u8 filter_type = raw_data[y * (1 + stride)];
        const u8 *src_line = raw_data + y * (1 + stride) + 1;
        u8 *dst_line = recon + y * stride;
        const u8 *prev_line = (y > 0) ? recon + (y - 1) * stride : NULL;

        for (usize x = 0; x < stride; x++) {
            const u8 a = (x >= (usize)bpp) ? dst_line[x-bpp] : 0;
            const u8 b = prev_line ? prev_line[x] : 0;
            const u8 c = (prev_line && x >= (usize)bpp) ? prev_line[x-bpp] : 0;
            const u8 raw = src_line[x];

            switch (filter_type) {
                case 0: dst_line[x] = raw; break;
                case 1: dst_line[x] = raw + a; break;
                case 2: dst_line[x] = raw + b; break;
                case 3: dst_line[x] = raw + ((a + b) / 2); break;
                case 4: {
                    const int p = a + b - c;
                    const int pa = abs(p-a), pb = abs(p-b), pc = abs(p-c);
                    const u8 pred = (pa <= pb && pa <= pc) ? a : (pb <= pc ? b : c);
                    dst_line[x] = raw + pred;
                    break;
                }
                default:
                    g_print("INVALID PNG FILTER filter=%d, row=%d, offset=%l\n", filter_type, y, (u64)y * (1 + stride));
                    g_free(recon);
                    g_free(raw_data);
                    return ret_texture;
                    break;
            }
        }
    }

    g_free(raw_data);

    u32 *pixels = (u32*)g_malloc(width * height * sizeof(u32));
    for (u32 y = 0; y < height; y++) {
        for (u32 x = 0; x < width; x++) {
            const isize p = y * width + x;
            const isize r_idx = y * stride + x * bpp;

            const u32 r = recon[r_idx + 0];
            const u32 g = recon[r_idx + 1];
            const u32 b = recon[r_idx + 2];
            const u32 a = (bpp == 4) ? recon[r_idx + 3] : 255;

            pixels[p] = (r << 24) | (g << 16) | (b << 8) | a;
        }
    }

    g_free(recon);

    ret_texture.data = pixels;
    ret_texture.w = width;
    ret_texture.h = height;

    return ret_texture;
}