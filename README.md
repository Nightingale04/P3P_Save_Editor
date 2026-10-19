# P3P Save Editor

A save editor for Persona 3 Portable, built with Qt 6.

## Disclaimer
Always back up your save files before using this tool. I\'m not responsible for any lost or corrupted save data.

## Features
- Character stats editor (yen, active personas, level, and social stats).
- Inventory editor.
    - Key items don't show up in-game when added because they likely require changing a flag in the save alongside the edited value. This hasn't been implemented in the editor.
- Compendium editor.

## Save Support
- [x] PSP (Decrypted save)
- [x] PS4 (Decrypted save)
- [x] PC

## Usage 
### Linux
- Run the AppImage from the [Releases](https://github.com/Nightingale04/P3P_Save_Editor/releases) page.
### Windows
- Download and unzip the `.zip` file from the [Releases](https://github.com/Nightingale04/P3P_Save_Editor/releases) page.
- Open the bin folder and run `P3P_Save_Editor.exe`.

## Requirements
To build this project, you need:
- C++17 compliant compiler
- CMake 3.19 and above
- Qt 6.5 and above
### How to Build:
```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## Credits
- [nlohmann json library](https://github.com/nlohmann/json) - JSON parser library for C++.
- [P4G Save Tool](https://github.com/fendevel/P4G-Save-Tool) - Reference for the save data structure and inspiration for the UI design.
- [Amicita Wiki](https://amicitia.miraheze.org/wiki/Main_Page) - Data source used to build the item, persona, skill, and arcana JSON databases.
- [P3FES Decomp](https://github.com/snaku/Persona3-FES-Decompilation) - Reference used to understand how to calculate the XP thresholds.

## License
GPLv3. See [LICENSE](LICENSE).
