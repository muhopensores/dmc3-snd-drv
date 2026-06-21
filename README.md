# dmc3-snd-drv
Replacement snd.drv for vanilla dmc3 port based on original driver. doesnt lag the game when loading a bgm track. used best with styleswitcher audio fix
# how it works
spins a worker thread to call original snd.drv stuff so it doesnt freeze the game when loading ogg files
# usage
this thing assumes style switcher is installed. set `ForceMode0=1` in `StyleSwitcher.ini` and rename original `snd.drv` in dmc3se root to `snd.drv.original` then drop a new one next to it.
# known issues
maybe 😔
