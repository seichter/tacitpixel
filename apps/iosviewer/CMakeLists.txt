

include_directories(../../include)

add_executable(
	TacitViewer
	MACOSX_BUNDLE
	main.mm appdelegate.h appdelegate.mm)
	

set(PRODUCT_NAME "TacitViewer")
set(PRODUCT_IDENTIFIER "com.technotecture.tacitviewer")
set(EXECUTABLE_NAME TacitViewer)

configure_file(Info.cmake.plist
	${CMAKE_BINARY_DIR}/Info.plist
	)

set_target_properties(TacitViewer PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_BINARY_DIR}/Info.plist )
	
target_link_libraries(TacitViewer 
	tacit 
	"-framework Foundation"
	"-framework CoreGraphics"
	"-framework QuartzCore"
	"-framework UIKit"
	"-framework OpenGLES"
	)

# code signing
set_target_properties(TacitViewer PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
