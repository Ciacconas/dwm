/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int stextdelim = 32;      /* ascii code of delimiter character used in status bar */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 20;       /* gap pixel between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = {
    "monospace:pixelsize=14:antialias=true:autohint=true",
	"Symbola:pixelsize=12:antialias=true:autohint=true",
	"Pomodoro:pixelsize=12:antialias=true:autohint=true",
	"FontAwesome:pixelsize=12:antialias=true:autohint=true",
	"Octicons:pixelsize=12:antialias=true:autohint=true",
	"Icomoon:pixelsize=12:antialias=true:autohint=true",
	"Hack Nerd Font:pixelsize=12:antialias=true:autohint=true",
	"PowerlineSymbols:pixelsize=12:antialias=true:autohint=true",
};
static const char dmenufont[]       = "monospace:size=10";
static char black[]					= "#222222";
static char gray[]					= "#777777";
static char white[]					= "#ffffff";
static char selcolor[]           	= "#005577";
static char *colors[][3] = {
       /*               fg          bg          border   */
    [SchemeNorm] = 		{ white, 	black,  	black },
    [SchemeSel]  = 		{ white,  	selcolor,  	selcolor  },
	[SchemeInactive] = 	{ white, 	gray, 		NULL },
};
static const unsigned int alphas[][3]      = {
	/*               fg    bg  border  */
	[SchemeNorm] = { 255,  50,   0   },
	[SchemeSel]  = { 255,  180, 255  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "[hidden]"};
static const char *montags[] = { "Z", "X", "C", "V"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Anki",           NULL,       NULL,       0,            1,           -1 }, /* Anki */
	{ "Arandr",         NULL,       NULL,       0,            1,           -1 }, /* Arandr */
	{ "Barrier",        NULL,       NULL,       0,            1,           -1 }, /* Barrier */
	{ "Blueman",        NULL,       NULL,       0,            1,           -1 }, /* Blueman */
	{ "CfgDFInput",     NULL,       NULL,       0,            1,           -1 }, /* PCSXR */
	{ "Conky",          NULL,       NULL,       0,            1,           -1 }, /* Conky */
	{ "Fceux",          NULL,       NULL,       0,            1,           -1 }, /* Fceux NES emulator */
	{ "Gimp",           NULL,       NULL,       0,            1,           -1 }, /* Gimp */
	{ "Helpdesk DICT",  NULL,       NULL,       0,            1,           -1 }, /* Steam */
	{ "mpv",            NULL,       NULL,       0,            1,           -1 }, /* MPV Media Player */
	{ "Nextcloud",      NULL,       NULL,       0,            1,           -1 }, /* Nextcloud client */
	{ "Nm-applet",      NULL,       NULL,       0,            1,           -1 }, /* NetworkManager Applets */
	{ "Nm-connection-editor", NULL, NULL,       0,            1,           -1 }, /* NetworkManager Connections */
	{ "Pavucontrol",    NULL,       NULL,       0,            1,           -1 }, /* Pavucontrol */
	{ "PCSXR",          NULL,       NULL,       0,            1,           -1 }, /* PCSXR */
	{ "SimpleScreenRecorder", NULL, NULL,       0,            1,           -1 }, /* SimpleScreenRecorder */
	{ "Steam",          NULL,       NULL,       0,            1,           -1 }, /* Steam */
	{ "Wfica",          NULL,       NULL,       0,            1,           -1 }, /* ICA Client */

	{ NULL,             "float",    NULL,       0,            1,           -1 }, /* st -n float */

	{ NULL,             NULL,       "Figure",   0,            1,           -1 }, /* Matplotlib */
	{ NULL, NULL, "Microsoft Teams Notification", 0,          1,           -1 }, /* Microsoft Teams Notifications */
	{ NULL, NULL, "Message from webpage",       0,            1,           -1 }, /* SAP notifications */
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",      tile },    /* first entry is default */
	{ "",      NULL },    /* no layout function means floating behavior */
	{ "",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define MONKEYS(KEY,TAG) \
	{ MODKEY,                  KEY,      focusmon,    {.i = TAG+1} }, \
	{ MODKEY|ShiftMask,        KEY,      tagmon,      {.i = TAG+1} }, \
	{ MODKEY|ControlMask,      KEY,      swapmon,     {.i = TAG+1} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", white, "-nf", black, "-sb", selcolor, "-sf", black, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} },
	{ MODKEY,                       XK_t,      setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ControlMask,           XK_m,      setmastermon,   {.i = 0} },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ControlMask,           XK_space,  swapmon,        {.i = +1} },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ControlMask,           XK_comma,  setmastermon,   {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ControlMask,           XK_period, setmastermon,   {.i = +1 } },
	{ MODKEY,                       XK_o,      winview,        {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	TAGKEYS(                        XK_grave,                  9)
	MONKEYS(                        XK_z,                      1)
	MONKEYS(                        XK_x,                      2)
	MONKEYS(                        XK_c,                      3)
	MONKEYS(                        XK_v,                      4)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signum>"` */
static Signal signals[] = {
	/* signum       function        argument  */
	{ 1,            quit,      		{.i = 0} },
	{ 2,            quit,      		{.i = 1} },
	{ 3,            xrdb,      		{.v = NULL} },
};
