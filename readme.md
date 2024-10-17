# Audio Effects Plugin

## Overview
This project is an Audio Plugin developed using JUCE and C++. The plugin provides musicians with real-time audio effects that can be applied during recordings. The supported effects include:

- **Echo**: Adds a delay to the audio, creating an echo effect.
- **Reverb**: Simulates the effect of sound reflecting off surfaces in a room, adding depth to the audio.
- **Noise Reduction**: Implements a noise gate to remove unwanted low-level noise.

The plugin is designed to work in Digital Audio Workstations (DAWs) such as Ableton Live, Logic Pro, and FL Studio.

## Features
- **Echo Effect**: Adjustable delay time and feedback parameters to control the number of echoes and intensity.
- **Reverb Effect**: Adjustable room size and damping to simulate different acoustic environments.
- **Noise Gate**: Adjustable threshold to reduce unwanted background noise.

## Requirements
- **JUCE Framework**: JUCE 6 or later.
- **C++ Compiler**: A C++17 compatible compiler (e.g., Visual Studio 2019, Xcode 12, or later).
- **DAW for Testing**: Any DAW that supports VST3, AU, or AAX plugin formats.

## Getting Started
1. **Install JUCE**: Make sure you have the JUCE framework installed and set up on your system. You can download JUCE from [JUCE's official website](https://juce.com/).

3. **Open the Project in Projucer**:
   - Open the project in Projucer (a tool included with JUCE).
   - Configure the desired export target (e.g., Visual Studio, Xcode).

4. **Build the Plugin**:
   - Export the project to your IDE (Visual Studio or Xcode).
   - Compile the plugin as a VST3 or AU.

5. **Test in a DAW**:
   - Load the compiled plugin in a DAW that supports the VST3 or AU format.
   - Test the plugin's effects using different parameter settings.

## Plugin Parameters
- **Delay Time** (`delayTime`): Controls the delay time for the echo effect (0.01 to 1.0 seconds).
- **Feedback** (`feedback`): Controls the intensity of the echo effect (0.0 to 0.95).
- **Room Size** (`roomSize`): Controls the size of the virtual room for the reverb effect (0.0 to 1.0).
- **Noise Gate Threshold** (`threshold`): Controls the level below which the noise gate reduces signal (0.0 to 1.0).

## File Structure
- **Source Code**: The main plugin logic is implemented in `AudioEffectsProcessor` class.
- **Editor**: The plugin interface is created using JUCE's `AudioProcessorEditor` class, providing sliders for controlling parameters.

## Additional Notes
- The echo effect uses a delay buffer of 1 second to store delayed audio samples.
- Reverb and noise gate are processed in real-time, with all parameters adjustable via the plugin's interface.

## Acknowledgments
- **JUCE Framework**: The JUCE team for providing a robust framework for audio plugin development.

## Contributing
Contributions are welcome! Feel free to submit a pull request or open an issue to discuss changes or suggestions.

