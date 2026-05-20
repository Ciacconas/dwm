# dwm 6.8 upgrade notes

Analysis branch: `latest-dwm-upgrade-analysis`

Checked against official upstream `https://git.suckless.org/dwm` on 2026-05-20.
Latest release tag found: `6.8` (`f63cde9354504ee9cfecc07517c03736d0f90c26`).
Upstream `master` is currently 5 commits beyond 6.8, touching only `dwm.c`.

## Current base

This fork is effectively a dwm 6.2-era tree. `config.mk` still has `VERSION = 6.2`.
The local branch contains many patch stacks on top of that base:

- `systray+alpha`
- `autostart`
- `interactivestatusbar`
- `hiddentag`
- `mastermon`
- `tilegap`
- `restartsig`
- `libxft-bgra`
- `xrdb`
- `center`
- `fakefullscreen` / `fullscreen`
- `cyclelayouts`
- `swallow`
- `sticky`
- `pertag`
- local app rules, theme changes, and keybinding changes

## Size of upstream change

Official dwm `6.2..6.8`:

```text
10 files changed, 281 insertions(+), 225 deletions(-)
```

Main files touched:

- `dwm.c`: 118 insertions, 102 deletions
- `drw.c`: 128 insertions, 92 deletions
- `drw.h`: 3 insertions
- `util.c`: 19 insertions, 17 deletions
- `config.def.h`, `config.mk`, `Makefile`: small edits

Local fork relative to upstream 6.2:

```text
11 files changed, 2346 insertions(+), 235 deletions(-)
```

Most local divergence is in:

- `dwm.c`: 1319 insertions, 114 deletions
- `dwm_status`: 447 new lines
- `config.def.h`: 143 insertions, 40 deletions
- `README.md`: 252 new lines

## Dry merge result

Command used:

```sh
git merge --no-commit --no-ff upstream-6.8
```

Result: merge conflicts in only three files:

- `drw.c`
- `drw.h`
- `dwm.c`

Files that merged automatically:

- `LICENSE`
- `Makefile`
- `config.def.h`
- `config.mk`
- `util.c`
- `util.h`

The dry merge was aborted afterwards, so the branch is clean except for this notes file.

## Conflict themes

### `drw.c` / `drw.h`

Expected difficulty: medium.

Conflict causes:

- Upstream rewrote UTF-8 decoding and text truncation.
- Upstream removed the old color-font blocking workaround because newer libXft fixed it.
- This fork has alpha/ARGB visual handling and a `libxft-bgra` conditional color-font path.
- Upstream added explicit color scheme cleanup APIs:
  - `drw_clr_free`
  - `drw_scm_free`
- This fork has a different `drw_clr_create` / `drw_scm_create` signature to pass alpha values.

Likely resolution:

- Keep upstream's newer UTF-8/text rendering implementation.
- Preserve this fork's alpha-aware visual/cmap paths.
- Add upstream's `drw_clr_free` and `drw_scm_free`, adapted to alpha-aware colors.
- Re-evaluate whether the `has_libxft_bgra` workaround is still needed on target systems.

### `dwm.c`

Expected difficulty: medium to high.

Conflict causes:

- Upstream changed fullscreen focus locking, size hint caching, monitor geometry handling, key grabbing, child process handling, and bar drawing.
- This fork heavily modifies the same areas for systray, master monitor, pertag, sticky, swallow, xrdb, interactive status, restart, and fullscreen behavior.

Most important conflict regions:

- `ISVISIBLE`: upstream keeps vanilla tag visibility; fork adds sticky windows.
- `Client`: upstream adds `hintsvalid`; fork adds center/swallow/sticky/pid fields.
- `drawbar`: upstream avoids drawing hidden bars and removes global `blw`; fork adds systray width, monitor labels, master-monitor-only tag display, inactive scheme.
- `getatomprop`: upstream fixes atom property bounds; fork customizes this for XEmbed systray.
- `manage`: upstream uses work area (`wx/wy/ww/wh`) placement; fork uses center/swallow/rule logic.
- `setup` / `spawn`: upstream replaces signal-based SIGCHLD handling with `sigaction`; fork also uses SIGUSR1 for xrdb reload.
- `updategeom`: upstream fixes monitor removal/update logic; fork needs to preserve `mastermon`.

Likely resolution:

- Keep upstream safety fixes unless they directly break a local feature.
- Port local feature fields into the upstream `Client` shape, including `hintsvalid`.
- Carefully reapply systray and mastermon changes around `drawbar`, `updatebarpos`, `updategeom`, and event handling.
- Replace the fork's `sigchld` path with upstream's `sigaction` approach, while preserving `SIGUSR1 -> xrdb`.
- Keep upstream's `getatomprop` bounds checks and adapt the XEmbed special case on top.

## Recommendation

This upgrade is worthwhile, but it is not a one-shot merge. The upstream delta is small, but it lands in exactly the same files and functions that this fork patches heavily.

Best approach:

1. Start from the current fork and merge `upstream-6.8`.
2. Resolve `drw.c`/`drw.h` first because they are mostly mechanical and build-facing.
3. Resolve `dwm.c` in feature clusters:
   - data structures and prototypes
   - signal/spawn handling
   - bar/status/systray drawing
   - client manage/unmanage/property paths
   - monitor geometry/mastermon handling
4. Build after each cluster.
5. Test at least:
   - normal tiling and floating
   - fullscreen/fakefullscreen
   - systray icons
   - clickable status bar
   - multi-monitor mastermon behavior
   - swallow/unswallow
   - xrdb reload via `SIGUSR1`

Judgement: expect a moderate porting effort, not a rewrite. The source conflict surface is narrow, but the behavioral regression risk is real because the conflicts sit in core window-management paths.
