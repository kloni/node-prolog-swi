{
  "targets": [
    {
      "target_name": "libswipl",
      "sources": [
        "./src/libswipl.cc"
      ],
      "include_dirs": [
        "./src",
        "$(SWI_PROLOG_HOME)"
      ],
      "direct_dependent_settings": {
        "linkflags": [
          "-D_FILE_OFFSET_BITS=64",
          "-D_LARGEFILE_SOURCE"
        ]
      },
      "libraries": [
        "-lswipl"
      ]
    }
  ]
}
