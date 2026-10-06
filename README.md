# dwm - dynamic window manager

`dwm` is an extremely fast, small, and dynamic window manager for X.

## Patches

Each patch applied to this version of dwm lives in a separate branch
in this repository. Such a branch can either be a vanilla patch like
they can be downloaded from the suckless website, or a modified patch,
or a completely custom patch. Below you find all the applied patches
in the order they were merged into the master branch of this
repository.

#### systray+alpha

- download [my combined patch](https://github.com/flaport/dwm/compare/upstream..systray+alpha.diff) ‧ [my alpha patch](https://github.com/flaport/dwm/compare/upstream..alpha.diff) ‧ [my systray patch](https://github.com/flaport/dwm/compare/upstream..systray.diff)
- see [combined branch](https://github.com/flaport/dwm/tree/systray+alpha) ‧ [alpha branch](https://github.com/flaport/dwm/tree/alpha) ‧ [systray branch](https://github.com/flaport/dwm/tree/systray)

Combining the `alpha` patch with the `systray` patch is not easy.
Hence, I first combined them in a separate branch, `systray+alpha`,
before merging them into master. I highly recommend using the combined
patch if you want both `alpha` and `systray` at the same time.

#### autostart

- download [my modified patch](https://github.com/flaport/dwm/compare/upstream..autostart.diff)
- see [branch](https://github.com/flaport/dwm/tree/autostart)
- see example [dwm_autostart](dwm_autostart) script.

The vanilla autostart patch from the suckless website is already very
minimal. In this modified patch, I removed the blocking call and
assumed the script `dwm_autostart` is added to the path. An example
[`dwm_autostart`](dwm_autostart) script can be found in this repository.

#### interactivestatusbar

- download [my custom patch](https://github.com/flaport/dwm/compare/upstream..interactivestatusbar.diff)
- see [branch](https://github.com/flaport/dwm/tree/interactivestatusbar)
- see example [dwm_status](dwm_status) script.

This is a completely custom patch I use to make the `dwm` status bar
interactive. The statusbar is still set with `xsetroot`, however a
unique character is used as delimiter between each widget. By using
this delimiter, `dwm` can figure out which widget was clicked and
calls in turn `dwm_status` (which should be placed in your path) with
two arguments: the widget index and the mouse button. The `dwm_status`
script can then use that info to start a specific action. An example
[`dwm_status`](dwm_status) script can be found in this repository.
Note that the supplied `dwm_status` script requires
[FontAwesome](https://github.com/gabrielelana/awesome-terminal-fonts/blob/master/fonts/fontawesome-regular.ttf)
to render the status bar icons.

#### hiddentag

- download [my custom patch](https://github.com/flaport/dwm/compare/upstream..hiddentag.diff)
- see [branch](https://github.com/flaport/dwm/tree/hiddentag)

This small custom patch adds a hidden (invisible) tag to the tagset.

#### mastermon

- download [my custom patch](https://github.com/flaport/dwm/compare/upstream..mastermon.diff)
- see [branch](https://github.com/flaport/dwm/tree/mastermon)

The mastermon patch introduces a "master" monitor. The master monitor
is the only monitor with tags. All other monitors will have no tags.
The tags from the master can be accessed (enabled/disabled/send to)
with the normal keybindings from any monitor. This makes the tag
system a lot less confusing in a multi monitor setup.

Why is this useful? In most multi-monitor setups you'll usually have a
preferred monitor to work on anyway. Other monitors will be less
important and will mostly be used to station content in view. If this
resembles your workflow, this patch is for you!

Some new kind of keybindings were added: `mod+[z|x|c|v]` to move focus
to monitor 1,2,3 or 4 and `mod+shift+[z|x|c|v]` to move the focused
window to monitor 1,2,3 or 4. Moreover, `mod+ctrl+m` can be used to "promote"
the current monitor to the master monitor and `mod+ctrl+tab` to
quickly swap the content between two adjacent monitors.

#### tilegap

- download [my modified patch](https://github.com/flaport/dwm/compare/upstream..tilegap.diff)
- see [branch](https://github.com/flaport/dwm/tree/tilegap)

Gaps between windows. Who doesn't want them? I removed the borders
from non-active windows as well.

#### restartsig

- download [patch](https://github.com/flaport/dwm/compare/upstream..restartsig.diff)
- see [branch](https://github.com/flaport/dwm/tree/restartsig)

Restart `dwm` with `mod+ctrl+shift+q`

#### libxft-bgra

- download [my custom patch](https://github.com/flaport/dwm/compare/upstream..libxft-bgra.diff)
- see [branch](https://github.com/flaport/dwm/tree/libxft-bgra)

Enables color emojis when
[`libxft-bgra`](https://gitlab.freedesktop.org/xorg/lib/libxft/-/merge_requests/1)
is installed on the system (currently only checks this by doing a
Pacman query). If `libxft-bgra` is not installed, color emojis will be
ignored.

#### xrdb

- download [my modified patch](https://github.com/flaport/dwm/compare/upstream..xrdb.diff)
- see [branch](https://github.com/flaport/dwm/tree/xrdb)

Read in colors from Xresources and apply them to dwm.

#### center

- download [my modified patch](https://github.com/flaport/dwm/compare/upstream..center.diff)
- see [branch](https://github.com/flaport/dwm/tree/center)

Place floating windows in the center of the screen. I fixed this patch
for multi-monitor setups.

#### fakefullscreen

- download [patch](https://github.com/flaport/dwm/compare/upstream..fakefullscreen.diff)
- see [branch](https://github.com/flaport/dwm/tree/fakefullscreen)

Only allow clients to fullscreen into space currently given to them.

#### fullscreen

- download [my modified patch](https://github.com/flaport/dwm/compare/upstream..fullscreen.diff)
- see [branch](https://github.com/flaport/dwm/tree/fullscreen)

Due to `fakefullscreen`, which limits the fullscreen of an application
to its window size, we need a way to force fullscreen when we want it:
`mod+ctrl+f`. I

#### cyclelayouts

- download [my modified patch](https://github.com/flaport/dwm/compare/upstream..cyclelayouts.diff)
- see [branch](https://github.com/flaport/dwm/tree/cyclelayouts)

Simply cycle through the available layouts with `mod+;`.

#### swallow

- download [patch](https://github.com/flaport/dwm/compare/upstream..swallow.diff)
- see [branch](https://github.com/flaport/dwm/tree/swallow)

If a terminal spawns a process without disowning it, the terminal will try to "swallow" the program,
i.e. the terminal hides itself behind the window it spawned until the spawned window/process is stopped.

#### sticky

- download [patch](https://github.com/flaport/dwm/compare/upstream..sticky.diff)
- see [branch](https://github.com/flaport/dwm/tree/sticky)

Make a window 'sticky', i.e. show it on _all_ tags with `mod+shift+o`.

#### pertag

- download [patch](https://github.com/flaport/dwm/compare/upstream..pertag.diff)
- see [branch](https://github.com/flaport/dwm/tree/pertag)

Default 'pertag' patch: this patch keeps layout, mwfact, barpos and nmaster per tag.

## Older versions

This repository also contains some older versions of my dwm build as
seperate branches of this repository:

#### dwm v2

- download as [patch](https://github.com/flaport/dwm/compare/upstream..v2.diff)
- see [branch](https://github.com/flaport/dwm/tree/v2)

the current version (should be more or less up to date with master)

#### dwm v1

- download as [patch](https://github.com/flaport/dwm/compare/upstream..v1.diff)
- see [branch](https://github.com/flaport/dwm/tree/v1)

The previous version: a build with similar features as v2 but with
more bugs and less clean separation of the applied patches.

#### dwm v0

- download as [patch](https://github.com/flaport/dwm/compare/upstream..v0.diff)
- see [branch](https://github.com/flaport/dwm/tree/v0)

My first attempt at customizing `dwm`. Only here to archive. I do not
recommend building this one.

#### upstream

This branch attempts to be up to date with upstream dwm:
[git.suckless.org/dwm](http://git.suckless.org/dwm).

## Requirements

In order to build `dwm` you need the Xlib header files.

## Installation

Edit config.mk to match your local setup (`dwm` is installed into
the `/usr/local` namespace by default).

Afterwards enter the following command to build and install `dwm` (if
necessary as root):

```
    make clean install
```

## Running dwm

Add the following line to your `.xinitrc` to start `dwm` using `startx`:

```
    exec dwm
```

In order to connect `dwm` to a specific display, make sure that
the `DISPLAY` environment variable is set correctly, e.g.:

```
    DISPLAY=foo.bar:1 exec dwm
```

(This will start `dwm` on display `:1` of the host `foo.bar`.)

In order to display status info in the bar, you can do something
like this in your `.xinitrc`:

```
    while xsetroot -name "`date` `uptime | sed 's/.*,//'`"
    do
    	sleep 1
    done &
    exec dwm
```

## Configuration

The configuration of `dwm` is done by creating a custom `config.h`
and (re)compiling the source code.

## Credits

This `dwm` fork is based on the suckless upstream: [https://dwm.suckless.org/](https://dwm.suckless.org/)

## Bar colors and transparency

The bar colors are defined in `config.h` (with defaults in `config.def.h`).
The values in the tables below are the tracked defaults in `config.def.h`;
the local `config.h` may differ. There are four base color values:

| Value | Default | Where it appears |
| --- | --- | --- |
| `black` | `#222222` | Normal bar background and selected text |
| `gray` | `#777777` | Selected tags on an unfocused monitor and normal window borders |
| `white` | `#ffffff` | Normal and inactive text |
| `selcolor` | `#005577` | Selected tag and monitor highlights and focused window borders |

At startup, dwm reads the X resource `color14` into `selcolor` and
`color15` into `white`, if they contain valid `#RRGGBB` values. The
other two colors use the values in `config.h`. Reload the Xresources
colors in a running dwm with `xsetroot -name "fsignal:3"` or `SIGUSR1`.
Editing colors in `config.h` requires rebuilding dwm.

The four values are combined into three schemes. Each scheme has a
foreground (text), background, and border color:

| Scheme | Foreground | Background | Border | Main use |
| --- | --- | --- | --- | --- |
| `SchemeNorm` | `white` | `black` | `gray` | Unselected tags, layout symbol, status text, and unfocused borders |
| `SchemeSel` | `black` | `selcolor` | `selcolor` | Selected tags on the focused monitor, its label, and focused borders |
| `SchemeInactive` | `white` | `gray` | Unset | Selected tags and monitor label on an unfocused monitor |

A tag containing a window gets a small occupancy square in its scheme's
foreground color. That square alone does not mean the window requested
attention. If a window on a tag is urgent, dwm swaps that tag's scheme
foreground and background. For an unselected tag on the current monitor,
this uses `SchemeNorm`: the whole tag background becomes `white` (the
`color15` X resource, if set), and its text and occupancy square become
`black`. This is the off-screen tag notification; it does not use
`SchemeInactive` or the `gray` value. `SchemeInactive` applies when a
selected tag is drawn on an unfocused monitor. With the `mastermon`
patch, numbered tags appear only on the master monitor.

Alpha is configured separately in `config.h`, on a scale from 0 (fully
transparent in the alpha channel) to 255 (fully opaque):

| Scheme | Foreground alpha | Background alpha | Border alpha |
| --- | ---: | ---: | ---: |
| `SchemeNorm` | 255 | 50 (about 20% opaque) | 255 |
| `SchemeSel` | 255 | 30 (about 12% opaque) | 255 |
| `SchemeInactive` | 0 (implicit pixel alpha) | 0 (implicit) | 0 (unused) |

`SchemeInactive` has no explicit row in the `alphas` array, so C sets
its entries to zero. Its border color is unset, so the border alpha is
unused. dwm uses a 32-bit ARGB visual when one is available.
A compositor such as picom displays the configured transparency. If dwm
falls back to a visual without an alpha channel, these opacity settings
cannot produce the intended transparency.
