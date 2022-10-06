# dmc3-snd-drv
Replacement snd.drv for vanilla dmc3 port based on soloud/sdl2/stb_vorbis. doesnt lag the game when loading a bgm track. used best with styleswitcher audio fix
# how it works
good question, i just stubbed every vtable function in snd.drv object until it kind of sort of worked, there's still 11 stubbed functions idk what they are supposed to do, game doesnt seem to call those
# building
build soloud as a static library, create a project in visual studio and set include/lib directory
cmakelists.txt is barebones and not finished sorry
# known issues
yes 😔
