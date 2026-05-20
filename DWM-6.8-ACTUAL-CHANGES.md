# dwm 6.8 actual changes

This file records the actual code changes made while following
`DWM-6.8-PORTING-PLAN.md`.

## Step 1: `drw.h` color API declarations

Status: completed

Files changed:

- `drw.h`

Current local behavior:

- Color creation is alpha-aware.
- `drw_clr_create` takes an `alpha` argument and uses the fork's ARGB visual and
  colormap.
- `drw_scm_create` takes `alphas[]` so every color in a scheme can carry its own
  alpha value.

Incoming upstream 6.8 behavior:

- Adds declarations for `drw_clr_free` and `drw_scm_free`.
- These allow allocated Xft colors to be released correctly during cleanup.

Conflict decision:

- Keep the local alpha-aware creation API.
- Add upstream cleanup declarations alongside it.
- Do not switch to upstream's non-alpha `drw_clr_create` or `drw_scm_create`
  signatures.

Expected visible behavior:

- No visible behavior change from this step alone.
- This prepares the code for a later cleanup change that should reduce leaked
  Xft color resources when dwm exits or reloads colors.

Actual code change:

- Added declarations for:
  - `drw_clr_free(Drw *drw, Clr *c)`
  - `drw_scm_free(Drw *drw, Clr *scm, size_t clrcount)`
- Preserved existing alpha-aware declarations for:
  - `drw_clr_create(..., unsigned int alpha)`
  - `drw_scm_create(..., const unsigned int alphas[], ...)`

Verification:

- `make clean`
- `make`
- Result: build passed.

## Step 10: remove old `sigchld`

Status: completed

Files changed:

- `DWM-6.8-ACTUAL-CHANGES.md`

Current local behavior before step 9:

- `sigchld` was a standalone signal handler.
- It reinstalled itself with `signal(SIGCHLD, sigchld)`.
- It reaped children with `waitpid`.

Incoming upstream 6.8 behavior:

- The standalone `sigchld` function no longer exists.
- Child cleanup is handled by the `sigaction` setup in `setup`.

Conflict decision:

- The source change was already completed in step 9.
- Keep `sigusr1` because it is local xrdb reload behavior.

Expected visible behavior:

- No additional behavior change beyond step 9.
- Spawned child cleanup is handled through upstream's `sigaction` model.

Actual code change:

- No new source change in this step.
- Verified there are no remaining `sigchld` declarations, definitions, or calls
  in `dwm.c`.

Verification:

- `rg -n 'sigchld|SIGCHLD|sigaction|signal\(' dwm.c`
- Source contains `SIGCHLD` only in the new `sigaction` setup.

## Step 11: reset child `SIGCHLD` handling in `spawn`

Status: completed

Files changed:

- `dwm.c`

Current local behavior before this step:

- `spawn` updated `dmenumon` for dmenu.
- The child closed the X connection, called `setsid`, and then called `execvp`.
- If `execvp` failed, it printed with `fprintf`/`perror` and exited
  successfully.

Incoming upstream 6.8 behavior:

- Declares a local `struct sigaction`.
- In the child process, restores `SIGCHLD` handling to `SIG_DFL` before
  `execvp`.
- Uses `die` if `execvp` fails.

Conflict decision:

- Adopt upstream's child-side signal reset.
- Preserve local `dmenumon` behavior.
- Adopt upstream's `die` failure path.

Expected visible behavior:

- Normal launching should feel unchanged.
- Programs spawned by dwm should not inherit dwm's ignored `SIGCHLD` behavior.
- If a spawn command is invalid, the failure path exits as an error instead of a
  successful child exit.

Actual code change:

- Added `struct sigaction sa` in `spawn`.
- Restored child `SIGCHLD` handling to `SIG_DFL` after `setsid`.
- Replaced `fprintf`/`perror`/`exit(EXIT_SUCCESS)` with upstream's `die` call.

Verification:

- `make clean`
- `make`
- Result: build passed.

## Step 9: update `setup` SIGCHLD handling

Status: completed

Files changed:

- `dwm.c`

Current local behavior before this step:

- `setup` called `sigchld(0)`.
- `sigchld` installed itself as the `SIGCHLD` handler using `signal`.
- The handler reaped exited children with `waitpid`.
- `setup` also installed `SIGUSR1` for xrdb reload.

Incoming upstream 6.8 behavior:

- Uses `sigaction` for `SIGCHLD`.
- Sets `SA_NOCLDWAIT` so children do not become zombies.
- Performs one immediate `waitpid` cleanup for inherited zombies.
- Removes the old `sigchld` function.

Conflict decision:

- Adopt upstream's safer `SIGCHLD` handling.
- Keep the local `SIGUSR1 -> xrdb` behavior.
- Leave child-side `spawn` signal reset for the next planned step.

Expected visible behavior:

- Normal spawning should feel unchanged.
- Exited child processes should be handled with less signal-handler complexity.
- `SIGUSR1` color reload should continue working.

Actual code change:

- Removed the `sigchld` prototype and function.
- Added `struct sigaction sa` in `setup`.
- Installed `SIGCHLD` handling with:
  `SA_NOCLDSTOP | SA_NOCLDWAIT | SA_RESTART`.
- Replaced `sigchld(0)` with upstream's immediate `waitpid` cleanup loop.

Verification:

- `make clean`
- `make`
- Result: build passed.

## Step 7: keep sticky-aware `ISVISIBLE`

Status: completed

Files changed:

- `DWM-6.8-ACTUAL-CHANGES.md`

Current local behavior:

- `ISVISIBLE(C)` returns true when a client is on the selected tag or when the
  client has `issticky` set.
- This supports the local sticky-window feature bound to `MODKEY|ShiftMask +
  XK_o`.

Incoming upstream 6.8 behavior:

- `ISVISIBLE(C)` only checks whether the client belongs to the selected tag.
- Upstream has no sticky-window concept.

Conflict decision:

- Keep the local sticky-aware macro unchanged.
- Do not adopt upstream's vanilla `ISVISIBLE`.

Expected visible behavior:

- No visible behavior change.
- Sticky windows continue appearing on all tags.
- If upstream's macro were used, sticky windows would stop working as expected.

Actual code change:

- No source code change required; current `dwm.c` already has the desired local
  behavior.

Verification:

- Inspected `dwm.c`.
- Compared against upstream `6.8`.
- No build required because no source code changed.

## Step 8: add `Client.hintsvalid`

Status: completed

Files changed:

- `dwm.c`

Current local behavior before this step:

- `Client` stored size-hint values such as base size, increments, min/max size,
  and aspect limits.
- Size hints were recalculated immediately in the event paths that noticed hint
  changes.

Incoming upstream 6.8 behavior:

- Adds `hintsvalid` to `Client`.
- Later upstream changes use this as a lazy invalidation flag:
  - `propertynotify` marks hints invalid.
  - `applysizehints` recalculates only when needed.
  - `updatesizehints` marks hints valid again.

Conflict decision:

- Add upstream's `hintsvalid` field while preserving all local fields for
  center, swallow, sticky, terminal detection, and PID tracking.

Expected visible behavior:

- No visible behavior change from this field alone.
- This prepares for later size-hint behavior, which should reduce unnecessary
  size-hint recalculation.

Actual code change:

- Changed the size-hint field line in `struct Client` from:
  `basew, baseh, incw, inch, maxw, maxh, minw, minh`
  to:
  `basew, baseh, incw, inch, maxw, maxh, minw, minh, hintsvalid`.

Verification:

- `make clean`
- `make`
- Result: build passed.

## Step 6: move `LENGTH` to `util.h`

Status: completed

Files changed:

- `util.h`
- `dwm.c`

Current local behavior before this step:

- `LENGTH(X)` was defined locally in `dwm.c`.
- The macro was only available to files that defined it themselves.

Incoming upstream 6.8 behavior:

- Defines `LENGTH(X)` in `util.h` next to `MAX`, `MIN`, and `BETWEEN`.
- Removes the local `dwm.c` definition.

Conflict decision:

- Adopt upstream's shared `util.h` definition.
- Remove the duplicate macro from `dwm.c`.

Expected visible behavior:

- No visible or runtime behavior change.
- This is a compile-time cleanup that prevents duplicate macro conflicts and
  makes `LENGTH` available consistently to other source files.

Actual code change:

- Added `#define LENGTH(X) (sizeof (X) / sizeof (X)[0])` to `util.h`.
- Removed `#define LENGTH(X) (sizeof X / sizeof X[0])` from `dwm.c`.

Verification:

- `make clean`
- `make`
- Result: build passed.

## Step 5: `utf8decode` and `drw_text`

Status: completed

Files changed:

- `drw.c`
- `drw.h`

Current local behavior before this step:

- Text rendering used the older dwm 6.2 UTF-8 decoder.
- Long text was shortened through a fixed local buffer.
- Missing glyph lookup did not cache known misses.
- Color-font fallback was blocked unless a runtime Arch-specific
  `pacman -Q libxft | grep bgra` check passed.
- Fallback font matching forced `FC_COLOR` to false.

Incoming upstream 6.8 behavior:

- Rewrites UTF-8 decoding with stricter invalid-sequence handling.
- Reworks text overflow and ellipsis placement.
- Adds invalid-glyph rendering behavior for bad UTF-8.
- Caches known missing glyphs to avoid repeated expensive font matching.
- Adds `drw_fontset_getwidth_clamp`.
- Removes the old color-font workaround because libXft fixed it in 2.3.5.

Conflict decision:

- Adopt upstream's new `utf8decode`.
- Adopt upstream's new `drw_text` behavior.
- Preserve this fork's ARGB draw target by keeping:
  `XftDrawCreate(drw->dpy, drw->drawable, drw->visual, drw->cmap)`.
- Remove this fork's `has_libxft_bgra` runtime check because this system has
  `libxft 2.3.9`, newer than the upstream fix threshold.
- Remove forced `FC_COLOR = FcFalse` fallback matching so modern color glyphs can
  be used by Xft/fontconfig.
- Use an ASCII byte literal for the Unicode replacement character in source:
  `"\357\277\275"`.

Expected visible behavior:

- Long window titles and status text should truncate with cleaner ellipsis
  behavior.
- Invalid UTF-8 in titles or status text should degrade more gracefully.
- Missing glyphs should become cheaper after repeated encounters.
- Color emoji/color glyph fallback is now allowed on this system's modern
  libXft.
- Existing alpha/transparent bar rendering should be unchanged.

Actual code change:

- Removed the old `UTF_SIZ`, `utfbyte`, `utfmask`, `utfmin`, `utfmax`,
  `utf8decodebyte`, and `utf8validate` machinery.
- Added upstream's newer `utf8decode`.
- Removed the global `has_libxft_bgra`.
- Removed the `pacman` runtime check from `drw_fontset_create`.
- Removed the color-font rejection block from `xfont_create`.
- Replaced `drw_text` with the upstream 6.8 renderer, adapted for this fork's
  ARGB visual/colormap.
- Added `drw_fontset_getwidth_clamp` declaration and implementation.

Verification:

- `pkg-config --modversion xft` reported `2.3.9`.
- `pacman -Q libxft` reported `libxft 2.3.9-1`.
- `make clean`
- `make`
- Result: build passed.

## Step 5 follow-up: restore non-color emoji fallback

Status: completed

Files changed:

- `drw.c`
- `config.def.h`

Observed regression:

- After allowing color-font fallback, emoji and private-use symbols in the bar
  rendered as rectangular boxes.
- Fontconfig selects `JoyPixels` for emoji, and `JoyPixels` is a color font.
- On this dwm/Xft/ARGB drawing path, that color font does not render usefully
  even though the installed libXft is modern enough to avoid the old crash.

Fix decision:

- Keep the upstream 6.8 UTF-8 and text rendering improvements.
- Restore filtering of color fonts in `xfont_create`.
- Restore `FC_COLOR = FcFalse` during fallback font matching.
- Do not restore the old Arch-specific `pacman -Q libxft | grep bgra` runtime
  check.
- Revert the partial interrupted `config.def.h` font-family edit so this fix
  only addresses rendering behavior.

Expected visible behavior:

- Emoji/status icons should fall back to monochrome/symbol fonts again instead
  of JoyPixels boxes.
- 6.8 truncation, invalid UTF-8 handling, and missing-glyph caching remain.

Verification:

- `make clean`
- `make`
- Result: build passed.

## Step 4: `drw_clr_free` / `drw_scm_free`

Status: completed

Files changed:

- `drw.c`
- `dwm.c`

Current local behavior:

- `cleanup` freed each scheme with plain `free(scheme[i])`.
- `xrdb` reload allocated new schemes over the old `scheme[i]` pointers without
  freeing the old Xft colors first.
- The array memory was partly handled, but the Xft color resources were not
  released correctly.

Incoming upstream 6.8 behavior:

- Adds `drw_clr_free` to release a single Xft color.
- Adds `drw_scm_free` to release all colors in a scheme, then free the scheme
  array.
- Uses those helpers from cleanup.

Conflict decision:

- Adopt upstream's cleanup model.
- Adapt it to this fork's alpha/ARGB path by using `drw->visual` and
  `drw->cmap`, matching `drw_clr_create`.
- Also apply the cleanup helper in `xrdb` reload, because this fork can recreate
  schemes at runtime.

Expected visible behavior:

- Normal bar colors and transparency should look unchanged.
- On exit or xrdb reload, dwm should release Xft color resources more correctly.
- The user-facing difference is mostly stability/resource hygiene; after many
  xrdb reloads, dwm should avoid accumulating old color allocations.

Actual code change:

- Implemented `drw_clr_free`.
- Implemented `drw_scm_free`.
- Replaced `free(scheme[i])` in `cleanup` with `drw_scm_free`.
- Updated `xrdb` reload to free the old scheme before assigning a newly created
  one.

Verification:

- `make clean`
- `make`
- Result: build passed.

## Step 2: `drw_clr_create`

Status: completed

Files changed:

- None

Current local behavior:

- `drw_clr_create` allocates colors with this fork's selected `drw->visual` and
  `drw->cmap`.
- The function writes the configured alpha into the Xft color pixel:
  `(dest->pixel & 0x00ffffffU) | (alpha << 24)`.
- This supports transparent/ARGB bar colors and matches the visual chosen by
  `xinitvisual`.

Incoming upstream 6.8 behavior:

- `drw_clr_create` uses `DefaultVisual` and `DefaultColormap`.
- It does not accept an alpha argument.
- It does not modify the allocated pixel alpha.

Conflict decision:

- Keep the local implementation unchanged.
- Do not adopt upstream's non-alpha function signature or default visual path.

Expected visible behavior:

- No visible behavior change, because the function was intentionally left as-is.
- If upstream's version were used here, the bar could lose transparency or draw
  against the wrong visual/colormap on systems using ARGB visuals. Keeping the
  local function preserves the current transparent bar behavior.

Verification:

- Inspected local `drw_clr_create`.
- Compared against upstream `6.8`.
- No build required because no source code changed in this step.

## Step 3: `drw_scm_create`

Status: completed

Files changed:

- `drw.c`

Current local behavior:

- `drw_scm_create` creates one color scheme from color names plus matching
  alpha values.
- It calls the local alpha-aware `drw_clr_create` for every scheme color.
- Callers pass `colors[i]` and `alphas[i]`, so normal/selected/inactive
  schemes can each have configured opacity.

Incoming upstream 6.8 behavior:

- Keeps the same high-level purpose: allocate a color scheme array.
- Uses `sizeof(Clr)` instead of `sizeof(XftColor)`.
- Does not pass alpha values because vanilla dwm has no alpha patch.

Conflict decision:

- Keep the local alpha-aware function signature and loop.
- Adopt upstream's `sizeof(Clr)` allocation style.
- Update the stale comment because the later cleanup path will use
  `drw_scm_free`, not plain `free`.

Expected visible behavior:

- No visible behavior change.
- The bar colors and transparency should look exactly the same.
- This makes the code line up better with upstream's color abstraction before
  adding actual scheme cleanup in the next step.

Actual code change:

- Changed allocation from `sizeof(XftColor)` to `sizeof(Clr)`.
- Updated the comment from "caller has to call free(3)" to "Create color
  schemes" because the next cleanup step will use `drw_scm_free`.
- Preserved the local alpha-aware function signature and `alphas[i]` call.

Verification:

- `make clean`
- `make`
- Result: build passed.
