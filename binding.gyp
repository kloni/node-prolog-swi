{
  "targets": [
    {
      "target_name": "libswipl",
      "product_prefix": "lib",
      "sources": [
        "./src/libswipl.cc"
      ],
      "include_dirs": [
        "./src",
        "$(SWI_HOME_DIR)"
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
