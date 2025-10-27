project('openCapif', 'c',
  version : '0.1.0',
  meson_version : '>= 0.63.0',
)

# Include directories (public headers and cJSON)
inc = include_directories('include', 'cJSON')

# Sources: all C sources in src/ plus the bundled cJSON implementation
srcs = files(
  'src/opencapif_process.c',
  'src/publish_api.c',
  'src/general_utils.c',
  'src/register_provider.c',
  'src/retrieve_apis.c',
  'src/create_user.c',
  'src/delete_api.c',
  'src/getauth.c',
  'src/delete_provider.c',
  'src/delete_user.c',
  'cJSON/cJSON.c',
)

# External dependencies
curl_dep = dependency('libcurl', required : true)

# Compiler flags from the original Makefile
cflags = ['-Wall', '-Wextra', '-Werror', '-Iinclude', '-IcJSON']

executable('opencapif_process',
  srcs,
  include_directories : inc,
  dependencies : [curl_dep],
  c_args : cflags,
  install : true,
)
