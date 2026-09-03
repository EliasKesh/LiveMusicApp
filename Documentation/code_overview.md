# LiveMusicApp Code Documentation

## Core MIDI Handling

### AlsaUtils.h / AlsaUtils.c
Core MIDI functionality using ALSA:
- MIDI port initialization and management 
- MIDI message sending/receiving
- Volume control
- Device enumeration
- MIDI controller mappings for various hardware

### APlayMidi.h / APlayMidi.c
MIDI file playback functionality:
- MIDI file loading and parsing
- Playback control (play/pause/stop)
- Tempo control
- MIDI event scheduling
- Loop functionality

### Connections.h / Connections.c 
MIDI connection management:
- Port connection setup
- MIDI routing configuration
- Device connection tracking
- Port subscription handling

## Musical Tools

### Chorder.c
Musical chord and scale visualization tool:
- Fretboard display
- Chord and scale pattern visualization
- Musical interval calculations  
- Note name handling
- Interactive chord/scale selection
- Integration with GTK UI

## Default Configuration

### DefPrefs.h
Default application preferences and configurations:
- MIDI routing defaults
- Port configurations
- UI layouts
- Control mappings
- Application settings

### GenPrefs.h  
Generated preferences structure:
- Live configuration storage
- Runtime settings
- Current state information
- MIDI mappings
- Device configurations

## Key Features

- Comprehensive MIDI routing and control
- Musical visualization tools
- Support for multiple MIDI controllers
- Customizable layouts and configurations
- Real-time MIDI processing
- Interactive UI for musical tools
- MIDI file playback with looping
- Volume and expression control
- Device auto-detection

## Architecture

The codebase is organized around:

1. Core MIDI Engine
   - ALSA MIDI interface
   - Event processing
   - Device management

2. Musical Tools
   - Chord visualization
   - Scale patterns
   - Fretboard display

3. Configuration
   - Default settings
   - Runtime preferences
   - Layout management

4. User Interface
   - GTK-based UI
   - Interactive controls
   - Visual feedback

## Dependencies

- ALSA MIDI library
- GTK toolkit
- Standard C libraries
- POSIX threads
- Cairo graphics

## Building

The code uses autotools for building:
```
./configure
make
make install
```

## Contributing

When modifying code:
1. Follow existing code style
2. Document changes clearly
3. Test MIDI functionality
4. Update relevant configuration files
5. Maintain backward compatibility where possible
