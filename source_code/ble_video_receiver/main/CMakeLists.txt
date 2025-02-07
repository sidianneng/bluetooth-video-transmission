set(srcs "ble_video_receiver.c"
    "decode_image.c"
    "ble_client.c"
    "key_input.c"
    )

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "."
                    EMBED_FILES image.jpg)
