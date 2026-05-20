# dwm 6.8 porting plan

This is the working checklist for porting upstream dwm `6.8` into this fork while
preserving the local patches.

Target branch: `latest-dwm-upgrade-analysis`

## Strategy

Use upstream `6.8` as the release target. Upstream `master` is currently a few
commits beyond `6.8`, but those should be considered optional follow-up fixes
after the release port is stable.

Work function-by-function. Prefer upstream fixes where they are safety,
correctness, or maintenance improvements, but keep local behavior where upstream
is unaware of this fork's patches.

Run `make clean && make` after each major cluster.

## Phase 1: Rendering layer

### 1. `drw.h`: color API declarations

Current repo:

- Color creation is alpha-aware.
- `drw_clr_create` accepts an `alpha`.
- `drw_scm_create` accepts `alphas[]`.

Incoming upstream:

- Adds cleanup helpers:
  - `drw_clr_free`
  - `drw_scm_free`

Conflict:

- Upstream assumes vanilla color creation.
- This fork needs alpha-aware color creation for ARGB/alpha/systray behavior.

Plan:

- Keep the alpha-aware create signatures.
- Add upstream cleanup declarations.
- Use `drw_scm_create(drw, colors[i], alphas[i], 3)` at call sites.
- Use `drw_scm_free(drw, scheme[i], 3)` during cleanup.

### 2. `drw_clr_create`

Current repo:

- Uses `drw->visual` and `drw->cmap`.
- Injects alpha into `dest->pixel`.

Incoming upstream:

- Uses `DefaultVisual` and `DefaultColormap`.

Conflict:

- Taking upstream directly would break alpha-aware drawing.

Plan:

- Keep this fork's implementation.
- Do not revert to upstream's default visual/colormap path.

### 3. `drw_scm_create`

Current repo:

- Accepts `alphas[]`.
- Calls alpha-aware `drw_clr_create`.

Incoming upstream:

- Uses vanilla color names only.
- Uses `sizeof(Clr)` allocation style.

Conflict:

- Function signature differs.

Plan:

- Keep this fork's signature.
- Adopt upstream's clearer allocation style if useful.
- Preserve alpha-aware behavior.

### 4. `drw_clr_free` / `drw_scm_free`

Current repo:

- Does not free individual Xft colors.

Incoming upstream:

- Frees each `XftColor` before freeing the scheme.

Conflict:

- No behavioral conflict, but upstream's implementation must use this fork's
  visual and colormap.

Plan:

- Add both functions.
- Use `drw->visual` and `drw->cmap`, not `DefaultVisual` and
  `DefaultColormap`.

### 5. `utf8decode` and `drw_text`

Current repo:

- Uses older dwm 6.2 text rendering.
- Has `has_libxft_bgra` behavior for color font handling.
- Uses this fork's ARGB visual and colormap when creating `XftDraw`.

Incoming upstream:

- Rewrites UTF-8 decoding.
- Improves invalid UTF-8 handling.
- Improves truncation/ellipsis behavior.
- Avoids repeated expensive font matching.
- Removes old color-font blocking workaround.

Conflict:

- Upstream text rendering should be adopted, but upstream drawing uses default
  visual/colormap and does not know about this fork's alpha path.

Plan:

- Adopt upstream `utf8decode`.
- Adopt upstream `drw_text` structure.
- Preserve `XftDrawCreate(drw->dpy, drw->drawable, drw->visual, drw->cmap)`.
- Decide whether to keep `has_libxft_bgra`.
  - Keep it if older target systems still need it.
  - Remove it if all target systems have modern libXft.

## Phase 2: shared macros and structs

### 6. `LENGTH`

Current repo:

- Defines `LENGTH` in `dwm.c`.

Incoming upstream:

- Moves `LENGTH` to `util.h`.

Conflict:

- Duplicate macro if both are kept.

Plan:

- Remove `LENGTH` from `dwm.c`.
- Use upstream's `util.h` definition.

### 7. `ISVISIBLE`

Current repo:

- Sticky windows are visible on all tags.

Incoming upstream:

- Uses vanilla tag-only visibility.

Conflict:

- Upstream would break sticky windows.

Plan:

- Keep this fork's sticky-aware `ISVISIBLE`.

### 8. `struct Client`

Current repo:

- Adds fields for center, swallow, sticky, terminal detection, and PID tracking.

Incoming upstream:

- Adds `hintsvalid` for lazy size-hint recalculation.

Conflict:

- Same struct field area.

Plan:

- Merge both sets of fields.
- Add `hintsvalid` without removing local fields.

## Phase 3: setup, signals, and spawn

### 9. `setup`

Current repo:

- Calls `sigchld(0)`.
- Installs `SIGUSR1` for xrdb reload.

Incoming upstream:

- Replaces old SIGCHLD handler with `sigaction(SIGCHLD, SA_NOCLDWAIT)`.
- Runs one-time zombie cleanup with `waitpid`.

Conflict:

- SIGUSR1 must stay.
- Old `sigchld` path should be replaced.

Plan:

- Adopt upstream SIGCHLD handling.
- Keep `signal(SIGUSR1, sigusr1)`.

### 10. `sigchld`

Current repo:

- Defines a reusable SIGCHLD handler.

Incoming upstream:

- Removes this function.

Conflict:

- Upstream's safer process model makes the local handler unnecessary.

Plan:

- Remove `sigchld`.
- Keep `sigusr1`.

### 11. `spawn`

Current repo:

- Forks, closes the X connection, calls `setsid`, then `execvp`.
- Updates `dmenumon`.

Incoming upstream:

- Restores `SIGCHLD` to default in the child before `execvp`.
- Uses `die` for exec failure.

Conflict:

- No local feature conflict.

Plan:

- Adopt upstream child-side `sigaction(SIGCHLD, SIG_DFL)`.
- Preserve local `dmenumon` behavior.

## Phase 4: bar, status, and systray

### 12. `drawbar`

Current repo:

- Handles systray width.
- Shows tags only on `mastermon`.
- Draws monitor labels.
- Uses inactive monitor scheme.
- Supports interactive status text.
- Uses global `blw`.

Incoming upstream:

- Returns early when `m->showbar` is false.
- Removes global `blw`.
- Computes layout symbol width locally.

Conflict:

- Upstream bar logic does not know about systray, mastermon, hidden tag,
  inactive schemes, or interactive status.

Plan:

- Keep this fork's draw structure.
- Add upstream `if (!m->showbar) return;`.
- Prefer local `TEXTW(m->ltsymbol)` over global `blw`.
- Preserve systray width subtraction.
- Preserve mastermon-only tag rendering.
- Preserve monitor labels and inactive schemes.
- Preserve status text behavior.

### 13. `buttonpress`

Current repo:

- Handles mastermon tag area.
- Handles monitor label area.
- Dispatches status clicks through `clickstatus`.
- Adjusts for systray width.

Incoming upstream:

- Fixes status text click-area mismatch.
- Removes dependence on global `blw`.

Conflict:

- Upstream click math does not include systray, interactivestatusbar, or
  mastermon.

Plan:

- Do not replace wholesale.
- Port the specific upstream click-area fix.
- Keep systray-adjusted status coordinates.
- Keep `clickstatus(stext, x, button)`.

### 14. `updatebarpos`

Current repo:

- Systray-aware.

Incoming upstream:

- Vanilla bar geometry.
- 6.8 draw behavior assumes hidden bars are not drawn.

Conflict:

- Systray placement depends on local behavior.

Plan:

- Keep systray-aware behavior.
- Ensure hidden bars and systray movement still work together.

### 15. `updatesystray`, `getsystraywidth`, `systraytomon`

Current repo:

- Implements systray support.

Incoming upstream:

- No systray.

Conflict:

- No direct upstream function, but bar/status changes affect these indirectly.

Plan:

- Preserve these functions.
- After `drawbar`, `buttonpress`, and `updatebarpos`, manually test systray
  placement and status click coordinates.

## Phase 5: window management core

### 16. `getatomprop`

Current repo:

- Has special handling for systray/XEmbed properties.

Incoming upstream:

- Fixes heap/bounds issues by checking returned item count before reading.

Conflict:

- Local XEmbed special case reads property data differently.

Plan:

- Start from upstream's safer version.
- Reapply the XEmbed special case.
- Preserve the upstream `nitems > 0` guard.

### 17. `manage`

Current repo:

- Applies local rules.
- Finds terminal for swallow.
- Centers selected floating windows.
- Swallows child windows.
- Uses old monitor boundary correction.

Incoming upstream:

- Changes placement correction to use monitor work area:
  - `wx`
  - `wy`
  - `ww`
  - `wh`

Conflict:

- Center and swallow code live in the same function.

Plan:

- Keep local rule, center, and swallow flow.
- Replace only the boundary correction block with upstream's work-area version.

### 18. `unmanage`

Current repo:

- Handles swallow/unswallow.
- Handles systray removal paths.

Incoming upstream:

- Adds `XSelectInput(dpy, c->win, NoEventMask)` before restoring unmanaged
  windows.

Conflict:

- Swallow changes ownership-like behavior around windows.

Plan:

- Add upstream `XSelectInput` only in the normal non-destroyed client path.
- Be careful not to apply it incorrectly to swallowed placeholder clients.

### 19. `propertynotify`

Current repo:

- Handles systray icon state.
- Handles title updates and related local behavior.

Incoming upstream:

- Changes `XA_WM_NORMAL_HINTS` handling from immediate recalculation to lazy
  invalidation with `c->hintsvalid = 0`.

Conflict:

- No feature conflict once `hintsvalid` exists.

Plan:

- Adopt lazy invalidation.

### 20. `applysizehints`

Current repo:

- Uses old immediate hint behavior.

Incoming upstream:

- Calls `updatesizehints(c)` only if hints are invalid.

Conflict:

- None after `hintsvalid` is added.

Plan:

- Adopt upstream lazy hint check.

### 21. `updatesizehints`

Current repo:

- Does not mark hints as valid.

Incoming upstream:

- Sets `c->hintsvalid = 1`.

Conflict:

- None.

Plan:

- Add `c->hintsvalid = 1` at the end.

## Phase 6: focus, fullscreen, and layout

### 22. `focusstack`

Current repo:

- Local fullscreen/fakefullscreen behavior may affect focus.

Incoming upstream:

- Adds `lockfullscreen` behavior so focus does not move away from fullscreen
  client.

Conflict:

- Needs testing with fakefullscreen and forced fullscreen.

Plan:

- Adopt upstream guard using `lockfullscreen`.
- Test browser fullscreen, forced fullscreen, fakefullscreen, and focusstack.

### 23. `setmfact`

Current repo:

- Pertag stores `mfact` per tag.

Incoming upstream:

- Expands allowed range from `0.1..0.9` to `0.05..0.95`.

Conflict:

- No conflict if pertag storage is preserved.

Plan:

- Adopt upstream range.
- Preserve assignment into `selmon->pertag->mfacts[...]`.

### 24. `tile`

Current repo:

- Modified by tilegap and border behavior.

Incoming upstream:

- Adds guards to avoid cumulative layout overflow.

Conflict:

- Upstream vanilla `tile` does not include tilegap math.

Plan:

- Do not paste upstream wholesale.
- Port the overflow guards into this fork's tilegap-aware function.
- Verify gaps remain consistent.

### 25. `zoom`

Current repo:

- Has local keybinding/config changes.

Incoming upstream:

- Simplifies null and floating checks.

Conflict:

- Probably none, but mastermon behavior should be checked.

Plan:

- Adopt simplification if it does not affect mastermon behavior.

## Phase 7: monitor geometry and master monitor

### 26. `updategeom`

Current repo:

- Modified heavily for `mastermon`.
- Must avoid dangling `mastermon` after monitor changes.

Incoming upstream:

- Fixes monitor addition/removal/update logic.

Conflict:

- High-risk conflict because both versions change monitor lifecycle logic.

Plan:

- Use upstream's structure for:
  - adding monitors
  - updating changed monitor geometry
  - removing monitors
- Reapply local `mastermon` policy.
- If `mastermon` is removed, choose a valid replacement.
- Never leave `mastermon` pointing to a freed monitor.

### 27. `cleanupmon`

Current repo:

- Accounts for `mastermon`.

Incoming upstream:

- Vanilla cleanup behavior.

Conflict:

- If the cleaned monitor is `mastermon`, local behavior must win.

Plan:

- Preserve local `mastermon` reassignment.
- Verify it aligns with the updated `updategeom` removal order.

### 28. `sendmon`

Current repo:

- May be affected by mastermon, sticky, fullscreen, or swallow behavior.

Incoming upstream:

- Release `6.8` has no major `sendmon` conflict.
- Upstream `master` after `6.8` includes a fullscreen resize fix.

Conflict:

- Optional follow-up, not part of strict `6.8`.

Plan:

- Finish the `6.8` port first.
- Consider later porting upstream master's fullscreen `sendmon` fix.

## Phase 8: key grabbing and utility functions

### 29. `grabkeys`

Current repo:

- Likely close to vanilla.

Incoming upstream:

- Handles keysyms that map to multiple keycodes.

Conflict:

- No local feature conflict expected.

Plan:

- Adopt upstream implementation.

### 30. `gettextprop`

Current repo:

- Old structure.

Incoming upstream:

- Small cleanup and safer string handling.

Conflict:

- None expected.

Plan:

- Adopt upstream implementation.

### 31. `maprequest`

Current repo:

- Has systray activation handling before normal map request handling.

Incoming upstream:

- Combines two early returns:
  - failed `XGetWindowAttributes`
  - `wa.override_redirect`

Conflict:

- Must not lose systray activation block.

Plan:

- Keep systray pre-block.
- Then use upstream combined guard.

### 32. `updateclientlist`

Current repo:

- Uses old-style `updateclientlist()`.

Incoming upstream:

- Changes declaration and definition to `updateclientlist(void)`.

Conflict:

- None.

Plan:

- Adopt upstream signature.

## Phase 9: config and build files

### 33. `config.mk`

Current repo:

- Has extra libraries for alpha, systray, and swallow:
  - `-lXrender`
  - `-lX11-xcb`
  - `-lxcb`
  - `-lxcb-res`

Incoming upstream:

- Vanilla dwm does not include those libraries.
- Updates version and feature-test macro.

Conflict:

- Removing local libraries would break this fork.

Plan:

- Keep local `LIBS`.
- Adopt `VERSION = 6.8`.
- Adopt `_XOPEN_SOURCE=700L`.

### 34. `config.def.h`

Current repo:

- Very customized.

Incoming upstream:

- Adds:
  - `lockfullscreen`
  - `refreshrate`
  - `static const Key keys[]`
  - `static const Button buttons[]`

Conflict:

- `const` is safe only if local code does not mutate those arrays.

Plan:

- Add `lockfullscreen`.
- Add `refreshrate`.
- Convert `keys` and `buttons` to `static const` only after confirming no local
  code mutates them.

### 35. `Makefile`

Current repo:

- Close to vanilla.

Incoming upstream:

- Removes the `options` target.

Conflict:

- None.

Plan:

- Adopt or skip. This is not behaviorally important.

## Verification checklist

After each phase:

```sh
make clean
make
```

After the full port:

- Start dwm under nested X if possible.
- Open and close terminals.
- Test tiling, monocle, floating, and gaps.
- Test systray icons.
- Test clickable status bar widgets.
- Test multi-monitor `mastermon` behavior.
- Test fullscreen and fakefullscreen.
- Test swallow and unswallow.
- Send `SIGUSR1` and confirm xrdb reload.
- Restart dwm through the configured keybinding.

## Highest-risk functions

- `drawbar`
- `buttonpress`
- `manage`
- `getatomprop`
- `updategeom`

Handle these slowly and test immediately after each one.
