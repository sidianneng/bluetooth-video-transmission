set(srcs "pretty_effect.c"
    "ble_video_receiver.c"
    "decode_image.c"
    )

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "."
                    EMBED_FILES image.jpg)
