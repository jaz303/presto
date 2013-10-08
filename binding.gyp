{
  "targets": [
    {
      "target_name": "presto_bindings",
      "include_dirs": ["src"],
      "sources": [
        "src/init.cpp",
        "src/display.cpp",
        "src/event_queue.cpp",
        "src/presto.cpp",
        "src/bitmap.cpp",
        "src/constants.cpp",
        "src/target.cpp"
      ],
      "conditions": [
        [ 'OS=="mac"', {
            'defines': [
              '__MACOSX__'
            ],
            'xcode_settings': {
              'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
            },
            'link_settings': {
              'libraries': [
                '-lallegro',
                '-lallegro_image',
                '-lallegro_font',
                '-lallegro_primitives',
                '-lallegro_audio'
              ],
            }
          }
        ]
      ]
    }
  ]
}