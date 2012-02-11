project(libvorbis)

set(vorbis_pub_hdrs
	include/vorbis/codec.h
	include/vorbis/vorbisenc.h
	include/vorbis/vorbisfile.h
	)
	

set(vorbis_srcs
	lib/analysis.c
#	lib/barkmel.c
	lib/bitrate.c
	lib/block.c
	lib/codebook.c
	lib/envelope.c
	lib/floor0.c
	lib/floor1.c
	lib/info.c
	lib/lookup.c
	lib/lpc.c
	lib/lsp.c
	lib/mapping0.c
	lib/mdct.c
	lib/psy.c
#	lib/psytune.c
	lib/registry.c
	lib/res0.c
	lib/sharedbook.c
	lib/smallft.c
	lib/synthesis.c
#	lib/tone.c
#	lib/vorbisenc.c
	lib/vorbisfile.c
	lib/window.c
	)
	
set(vorbis_hdrs
	lib/backends.h
	lib/bitrate.h
	lib/codebook.h
	lib/codec_internal.h
	lib/envelope.h
	lib/highlevel.h
	lib/lookup.h
	lib/lookup_data.h
	lib/lpc.h
	lib/lsp.h
	lib/masking.h
	lib/mdct.h
	lib/misc.h
	lib/os.h
	lib/psy.h
	lib/registry.h
	lib/scales.h
	lib/smallft.h
	lib/window.h
	)
	
include_directories(include ${CMAKE_CURRENT_SOURCE_DIR}/../libogg-1.1.4/include)

add_library(vorbis STATIC ${vorbis_hdrs} ${vorbis_srcs} ${vorbis_pub_hdrs})