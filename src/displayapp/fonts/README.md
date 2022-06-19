# Fonts

* [Jetbrains Mono](https://www.jetbrains.com/fr-fr/lp/mono/)
* [Awesome font from LVGL](https://lvgl.io/assets/others/FontAwesome5-Solid+Brands+Regular.woff)
* [Open Sans Light from Google](https://fonts.google.com/specimen/Open+Sans)

<<<<<<< HEAD
### How to add new symbols:

* Browse [this cheatsheet](https://fontawesome.com/cheatsheet/free/solid) and pick symbols
* For each symbol, add its hex code (0xf641 for the 'Ad' icon, for example) to the *Range* list (or the symbol list when its simple enough) in the `fonts.json` file
=======
## Generate the fonts:

* Open the [LVGL font converter](https://lvgl.io/tools/fontconverter)
* Name : jetbrains_mono_bold_20
* Size : 20
* Bpp : 1 bit-per-pixel
* Do not enable font compression and horizontal subpixel hinting
* Load the file `JetBrainsMono-Bold.tff` (use the file in this repo to ensure the version matches) and specify the following range : `0x20-0x7f, 0x410-0x44f`
* Add a 2nd font, load the file `FontAwesome5-Solid+Brands+Regular.woff` and specify the following
  range : `0xf293, 0xf294, 0xf244, 0xf240, 0xf242, 0xf243, 0xf241, 0xf54b, 0xf21e, 0xf1e6, 0xf54b, 0xf017, 0xf129, 0xf03a, 0xf185, 0xf560, 0xf001, 0xf3fd, 0xf069, 0xf1fc, 0xf45d, 0xf59f, 0xf5a0, 0xf029, 0xf027, 0xf028, 0xf6a9, 0xf04b, 0xf04c, 0xf048, 0xf051, 0xf095, 0xf3dd, 0xf04d, 0xf2f2, 0xf024, 0xf252, 0xf569, 0xf201, 0xf06e, 0xf015, 0xf072, 0xf1ec`
* Click on Convert, and download the file `jetbrains_mono_bold_20.c` and copy it in `src/DisplayApp/Fonts`
* Add the font .c file path to src/CMakeLists.txt
* Add an LV_FONT_DECLARE line in src/libs/lv_conf.h

Add new symbols:

* Browse the [cheatsheet](https://fontawesome.com/cheatsheet/free/solid) and find your new symbols
* For each symbol, add its hex code (0xf641 for the 'Ad' icon, for example) to the *Range* list (Remember to keep this
  readme updated with newest range list)
>>>>>>> 6ff0f94385ffe28e3abce43e335d0f793f4358c9
* Convert this hex value into a UTF-8 code
  using [this site](http://www.ltg.ed.ac.uk/~richard/utf-8.cgi?input=f185&mode=hex)
* Define the new symbols in `src/displayapp/screens/Symbols.h`:

```
static constexpr const char* newSymbol = "\xEF\x86\x85";
```

### the config file format:

inside `fonts`, there is a dictionary of fonts,
and for each font there is:
* sources - list of file,range(,symbols) wanted (as a dictionary of those)
* bpp - bits per pixel.
* size - size.
* patches - list of extra "patches" to run: a path to a .patch file. (may be relative)
* compress - optional. default disabled. add `"compress": true` to enable

### Navigation font

`navigtion.ttf` is created with the web app [icomoon](https://icomoon.io/app) by importing the svg files from `src/displayapp/icons/navigation/unique` and generating the font. `lv_font_navi_80.json` is a project file for the site, which you can import to add or remove icons.

