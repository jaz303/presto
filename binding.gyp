{
  "targets": [
    {
      "target_name": "presto_bindings",
      "include_dirs": ["src"],
      "sources": [
        "src/entry.cpp",
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
                '-lpthread',
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