CREATE TABLE IF NOT EXISTS analysis (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    color_codec_path TEXT NOT NULL,
    compression_id INTEGER,
    frame_expected_in TEXT NOT NULL,
    frame_data_out TEXT NOT NULL,
    metric_result_id INTEGER,
    experiment_id INTEGER,

    FOREIGN KEY (compression_id) REFERENCES compression_types(id),
    FOREIGN KEY (metric_result_id) REFERENCES metric_results(id)
);

CREATE TABLE IF NOT EXISTS compression_types (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS metric_results (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    error_percent REAL NOT NULL,
    average_error REAL NOT NULL,
    median_error REAL NOT NULL,
    decoding_time_ms REAL NOT NULL
);

CREATE TABLE IF NOT EXISTS color_codecs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    bits_per_number INTEGER NOT NULL,
    file_path TEXT NOT NULL,
    CHECK (bits_per_number > 0), 
    UNIQUE (file_path)
);

CREATE TABLE IF NOT EXISTS palettes (
    id INTEGER PRIMARY KEY AUTOINCREMENT
);

CREATE TABLE IF NOT EXISTS colors (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    r INTEGER NOT NULL,
    g INTEGER NOT NULL,
    b INTEGER NOT NULL,
    palette_id INTEGER NOT NULL,
    palette_order INTEGER NOT NULL,
    FOREIGN KEY (palette_id) REFERENCES palettes(id),
    UNIQUE (palette_id, palette_order)
);

CREATE TABLE IF NOT EXISTS palette_codecs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    color_codec_id INTEGER NOT NULL,
    palette_id INTEGER NOT NULL,

    FOREIGN KEY (color_codec_id) REFERENCES color_codecs(id),
    FOREIGN KEY (palette_id) REFERENCES palettes(id),
);

CREATE TABLE IF NOT EXISTS rgb_palette_codecs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    palette_codec_id INTEGER NOT NULL,

    FOREIGN KEY (palette_codec_id) REFERENCES palette_codecs(id)
);

CREATE TABLE IF NOT EXISTS ycbcr_palette_codecs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    palette_codec_id INTEGER NOT NULL,

    FOREIGN KEY (palette_codec_id) REFERENCES palette_codecs(id)
);

CREATE TABLE IF NOT EXISTS hsv_codecs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    palette_codec_id INTEGER NOT NULL,

    FOREIGN KEY (palette_codec_id) REFERENCES palette_codecs(id)
);

CREATE TABLE IF NOT EXISTS color_codec_wrappers (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    not_null_column_index INTEGER NOT NULL,
    hsv_codec_id INTEGER,
    rgb_palette_codec_id INTEGER,
    ycbcr_palette_codec_id INTEGER,

    FOREIGN KEY (hsv_codec_id) REFERENCES hsv_codecs(id),
    FOREIGN KEY (rgb_palette_codec_id) REFERENCES rgb_palette_codec(id),
    FOREIGN KEY (ycbcr_palette_codec_id) REFERENCES ycbcr_palette_codec(id),
    CHECK (hsv_codec_id != NULL or rgb_palette_codec_id != NULL or ycbcr_palette_codec_id != NULL)
);