cmake_minimum_required(VERSION 3.22)
project(Session10 VERSION 1.0.0 LANGUAGES C CXX)

set(JUCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/JUCE")
if(NOT EXISTS "${JUCE_DIRECTORY}/CMakeLists.txt")
    message(FATAL_ERROR
        "JUCE is missing at ${JUCE_DIRECTORY}. "
        "Run git submodule update --init --recursive from the course root.")
endif()
add_subdirectory("${JUCE_DIRECTORY}" "${CMAKE_BINARY_DIR}/JUCE")

set(PLUGIN_FORMATS Standalone VST3)
if(APPLE)
    list(APPEND PLUGIN_FORMATS AU)
endif()
set(AAX_SDK_PATH "" CACHE PATH "Optional AAX SDK directory")
if(AAX_SDK_PATH)
    juce_set_aax_sdk_path("${AAX_SDK_PATH}")
    list(APPEND PLUGIN_FORMATS AAX)
endif()

juce_add_plugin(HelloJUCE
    COMPANY_NAME "HFM"
    BUNDLE_ID "de.hfm.hellojuce"
    PLUGIN_MANUFACTURER_CODE Hfmk
    PLUGIN_CODE Hjuc
    FORMATS ${PLUGIN_FORMATS}
    PRODUCT_NAME "Hello JUCE"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE)

juce_generate_juce_header(HelloJUCE)

file(GLOB PLUGIN_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h")
target_sources(HelloJUCE PRIVATE ${PLUGIN_SOURCES})
target_compile_features(HelloJUCE PRIVATE cxx_std_17)
target_compile_definitions(HelloJUCE PUBLIC
    JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)
target_link_libraries(HelloJUCE PRIVATE juce::juce_audio_utils
    PUBLIC juce::juce_recommended_config_flags juce::juce_recommended_warning_flags)
