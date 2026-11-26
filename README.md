![LegionCore - Reforged](https://www.zupimages.net/up/26/09/uy23.png)

<h1 align="center">LegionCore — Reforged</h1>

<p align="center">
  <b>World of Warcraft: Legion 7.3.5 (build 26972) server emulator</b><br>
  Preservation first: Legion as you remember it, refined.
</p>

<p align="center">
  <img alt="Client" src="https://img.shields.io/badge/client-7.3.5%20(26972)-1f6feb">
  <img alt="License" src="https://img.shields.io/badge/license-GPL--3.0-green">
  <img alt="Status" src="https://img.shields.io/badge/status-active%20development-orange">
</p>

---

## About

Legion 7.3.5 is gone from the official servers. **Reforged brings it back** and keeps it as close as possible to the original, with every piece of content playable.

**This is still Legion.** We do not rewrite the expansion or change how it plays. We refine it: small adjustments, often hard to notice, that fix what was not built right in the first place and make the experience better. Nothing that turns the game upside down.

The core descends from LegionCore 7.3.5, built on the 2020 UWOW source, itself derived from an early [TrinityCore](https://github.com/TrinityCore/TrinityCore).

## What is different

- **Progressive tiers, if you want them**: a realm can relive Legion patch by patch, from 7.0 to 7.3, with currencies, item levels, quests and zones following the active patch. It is an option: by default the realm runs the full 7.3.5 content.
- **Raids released at your own pace**: each raid, difficulty and LFR wing can be opened on its own.
- **Gear that keeps its value**: during the expansion, Blizzard kept raising dungeon item levels as a catch-up. Dungeons keep their launch item levels here, so raiding stays the main source of gear. A catch-up mode can still be turned on for events.
- **Artifact Knowledge back to its 7.0 design**: the late-patch boosts made Artifact Power explode and unbalanced the game. Knowledge follows the original pace, with an optional weekly Knowledge book.
- **Quality of life**: small improvements that stay within what Legion could have shipped.

## Contributing

**Your bug reports are what move this project forward.** Legion is a huge expansion and a small team cannot test all of it alone. Every report helps.

- **Test and report**: play, and whenever something does not behave as it did on retail, [open an issue](https://github.com/Titans-Project/LegionCore-Reforged/issues/new?template=bug_report.md) using the template. Describe what you did, what you expected and what happened; IDs and screenshots help a lot. Reports are read and fixed quickly.

## Content status

https://titans-project.github.io/

## Getting started

### Requirements
Same as TrinityCore:
[Windows](https://www.trinitycore.info/en/install/requirements/windows) ·
[Linux](https://www.trinitycore.info/en/install/requirements/linux) ·
[macOS](https://www.trinitycore.info/en/install/requirements/macos)

### Build
Follow the TrinityCore [core installation guide](https://www.trinitycore.info/en/install/Core-Installation).

### Database
Nothing to import by hand. The base databases ship in `sql/base`. With `Updates.AutoSetup = 1` (default), the worldserver creates the databases, imports them and applies `sql/updates`.

### Client
**Legion 7.3.5, build 26972**: [download (torrent)](https://www.dragonwar.fr/download/Legion7.3.5.zip.torrent)

### Data files
Run the extractors from the client folder, in this order:

1. `mapextractor`
2. `vmap4extractor`
3. `vmap4assembler`
4. `mmaps_generator`

The server needs all the generated folders: `dbc`, `maps`, `vmaps`, `mmaps`, `cameras` and `gt`.

## Credits

- [TrinityCore authors](https://github.com/TrinityCore/TrinityCore/blob/master/AUTHORS)
- UWOW, the Russian team that wrote the original LegionCore
- [LegionCore 7.3.5 contributors](https://github.com/dufernst/LegionCore-7.3.5/graphs/contributors)
- [Titans-Project contributors](https://github.com/Titans-Project/LegionCore-Reforged/graphs/contributors)

## License

**GNU General Public License v3.0**. See [LICENSE](LICENSE).

> Any commercial use of this code, and running a paid public server, is strictly prohibited. Both go against the terms of the game's original license and against the ethics of this preservation project.
