#
#			AuxWiden Build System
#			"Gotta build the Stereo Widener!!" - inpinseptipin
#
#			BSD 3-Clause License
#
#			Copyright (c) 2025, Satyarth Arora
#			All rights reserved.
#
#			Redistribution and use in source and binary forms, with or without
#			modification, are permitted provided that the following conditions are met:
#
#			1. Redistributions of source code must retain the above copyright notice, this
#			   list of conditions and the following disclaimer.
#
#			2. Redistributions in binary form must reproduce the above copyright notice,
#			   this list of conditions and the following disclaimer in the documentation
#			   and/or other materials provided with the distribution.
#
#			3. Neither the name of the copyright holder nor the names of its
#			   contributors may be used to endorse or promote products derived from
#			   this software without specific prior written permission.
#
#			THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#			AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#			IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#			DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
#			FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#			SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#			CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
#			OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#			OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME "AuxShaper" CACHE STRING "Project Name")

project(${PROJECT_NAME})


include(FetchContent)

FetchContent_Declare(
	auxport
	GIT_REPOSITORY https://github.com/inpinseptipin/AuxPort
	GIT_TAG	master
)
FetchContent_MakeAvailable(auxport)

FetchContent_Declare(
	JUCE
	GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
	GIT_TAG origin/master
	GIT_SHALLOW ON
	FIND_PACKAGE_ARGS 8.0.0 GLOBAL
)

FetchContent_MakeAvailable(JUCE)


juce_add_plugin(${PROJECT_NAME}
VERSION 0.01
COMPANY_NAME "AuxAudio"
IS_SYNTH FALSE
NEEDS_MIDI_INPUT FALSE
NEEDS_MIDI_OUTPUT FALSE
IS_MIDI_EFFECT FALSE
PLUGIN_MANUFACTURER_CODE auxa
PLUGIN_CODE auxs
FORMATS VST3 Standalone
PRODUCT_NAME ${PROJECT_NAME}
)

juce_generate_juce_header(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
	PRIVATE	
	${auxport_SOURCE_DIR}/Extensions/AuxJuce/AuxParam.h
	${auxport_SOURCE_DIR}/Extensions/AuxJuce/AuxParam.cpp
	src/AuxBezier.h
	src/AuxBezier.cpp
	src/AuxShaper.h
	src/AuxShaper.cpp
	src/AuxSearch.h
	src/AuxSearch.cpp
	src/AuxWaveShape.h
	src/AuxWaveShape.cpp
	src/PluginEditor.h
	src/PluginEditor.cpp
	src/PluginProcessor.h
	src/PluginProcessor.cpp
)

target_compile_definitions(${PROJECT_NAME}
	PUBLIC
	JUCE_WEB_BROWSER=0
	JUCE_USE_CURL=0
	JUCE_VST3_CAN_REPLACE_VST2=0
)



target_link_libraries(${PROJECT_NAME}
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)



target_link_libraries(${PROJECT_NAME} PUBLIC AuxPort)
target_include_directories(${PROJECT_NAME} PUBLIC "${auxport_SOURCE_DIR}/AuxPort")
target_include_directories(${PROJECT_NAME} PUBLIC "${auxport_SOURCE_DIR}/Extensions/AuxJuce")
