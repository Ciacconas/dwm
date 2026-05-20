# AGENTS.md

## Project

This is a customized dwm fork currently being ported from a 6.2-era base toward
upstream dwm 6.8.

## Workflow

- Work in small, function-focused steps following `DWM-6.8-PORTING-PLAN.md`.
- Record actual changes in `DWM-6.8-ACTUAL-CHANGES.md`.
- For each step, explain:
  - current local behavior
  - incoming upstream behavior
  - conflict decision
  - expected visible behavior
- Preserve local patches unless the user explicitly agrees to change them.
- If a conflict is broad or user-visible, ask before changing code.
- Make a proper git commit after each completed step.

## Build

Run after every code change:

```sh
make clean
make
```

## Important Local Behavior

- Keep alpha/ARGB rendering paths using `drw->visual` and `drw->cmap`.
- Keep sticky-aware `ISVISIBLE`.
- Keep non-color font fallback for bar glyphs; color fonts rendered as boxes on
  this setup.
- Do not revert unrelated user changes.

## Notes

- `config.h` may exist as an ignored local build file. Check whether changes need
  `config.def.h`, `config.h`, or both before assuming a rebuild uses defaults.
