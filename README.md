Colejak 
=======

ANSI keyboard layout to maximize actions per minute.

- Colemak-DH foundation
- Improved for thumb typing
- Better-placed modifier keys
- Navigation row
- Dead keys for symbols
- Number Layer

![Layout preview](
https://github.com/james-orcales/colejak/blob/main/assets/colejak.png)

### Finger Placements

![Layout finger placements](
https://github.com/james-orcales/colejak/blob/main/assets/colejak-finger-placement.png)

### Dead Key Mappings

![Layout dead key mappings](
https://github.com/james-orcales/colejak/blob/main/assets/colejak-xcompose.png)

Linux
-----

> [!NOTE]
> These will overwrite existing destination files. Your original files such as *evdev.xml*
will be saved as *evdev.xml.bak*

### Copy

```
git clone --filter=blob:none --depth=1 https://github.com/james-orcales/colejak
cd colejak
mkdir --parents $HOME/.config/xkb/rules
mkdir --parents $HOME/.config/xkb/symbols
cp --suffix=.bak rules/* $HOME/.config/xkb/rules/
cp --suffix=.bak symbols/* $HOME/.config/xkb/symbols/
cp --suffix=.bak .XCompose $HOME/.XCompose
```

### Symbolic Link

```
git clone https://github.com/james-orcales/colejak
cd colejak
mkdir --parents $HOME/.config/xkb
ln --symbolic --suffix=.bak $(realpath rules) $HOME/.config/xkb/rules
ln --symbolic --suffix=.bak $(realpath symbols) $HOME/.config/xkb/symbols
ln --symbolic --suffix=.bak $(realpath .XCompose) $HOME/.XCompose
```

<details>
<summary>Sway</summary>

```
input type:keyboard {
    xkb_layout colejak(default)
}
```

</details>


<details>
<summary>Gnome</summary>

Search for *Colejak* in `gnome-control-center > Keyboard > Input Sources`

</details>

Application Remaps (Optional)
------------------------------

<details>
<summary>Neovim</summary>

### Remaps

```lua
local override = function(modes, new, default, desc, custom_behavior)
    local behavior = default
    if custom_behavior then
        behavior = custom_behavior
    end

    vim.keymap.set(modes, default, "<nop>")
    vim.keymap.set(modes, new, behavior, { desc = desc })
end

override({ "n", "v", "o" }, "<C-Y>", "<C-R>", "Redo")
vim.keymap.set("n", "<C-S>", "<CMD>w<CR>", { desc = "Save File" })
vim.keymap.set("i", "<C-S>", "<ESC><CMD>w<CR>", { desc = "Save File" })
vim.keymap.set("n", "<C-Q>", "<CMD>q<CR>", { desc = "Quit Pane" })
vim.keymap.set("n", "<C-S-Q>", "<CMD>qa!<CR>", { desc = "Force Quit Vim" })

override({ "n", "v", "o" }, "<C-Left>", "b", "Jump previous word")
override({ "n", "v", "o" }, "<S-Left>", "B", "Jump previous whitespace")
override({ "n", "v", "o" }, "<C-Right>", "w", "Jump next word")
override({ "n", "v", "o" }, "<S-Right>", "W", "Jump next whitespace")

override({ "n", "v", "o" }, "<C-Home>", "gg", "Jump first line")
override({ "n", "v", "o" }, "<C-End>", "G", "Jump last line")
vim.keymap.set({ "n", "v", "o" }, "<Home>", "^", { desc = "Jump to first char of current line" })
vim.keymap.set({ "i" }, "<Home>", "<C-o>^", { desc = "Jump to first char of current line" })

override({ "i", "c" }, "<C-H>", "<C-W>", "Kill word before cursor")
vim.keymap.set({ "n" }, "<C-H>", "db", { desc = "Kill word before cursor" })
vim.keymap.set({ "n" }, "<C-BS>", "db", { desc = "Kill word before cursor" })
override({ "i", "c" }, "<C-BS>", "<C-W>", "Kill word before cursor")
vim.keymap.set({ "i" }, "<C-Del>", "<Esc><Right>dwi", { desc = "Kill next word from cursor" })
vim.keymap.set({ "n" }, "<C-Del>", "dw", { desc = "Kill next word from cursor" })
vim.keymap.set({ "n" }, "<S-Del>", "dW", { desc = "Kill to whitespace from cursor" })

override("n", "<BS>", "x", "Kill char before cursor", "<Left>x")
override("v", "<BS>", "x", "Remap x to Backspace")
```

### My Neovim Config

https://github.com/james-orcales/init.lua

</details>


<details>
<summary>Vimium</summary>

### Remaps

```
unmapAll

map h scrollLeft
map <down> scrollDown
map <up> scrollUp
map <s-right> scrollRight
map <s-left> scrollLeft
map <home> scrollToTop
map <end> scrollToBottom
map <s-down> scrollPageDown
map <s-up> scrollPageUp


#focusing
map se focusInput
map t LinkHints.activateMode
map T LinkHints.activateModeWithQueue
map yt  LinkHints.activateModeToCopyLinkUrl

#tabs
map <left> previousTab
map <right> nextTab
map <c-left> moveTabLeft
map <c-right> moveTabRight
map wt moveTabToNewWindow

map ? showHelp
```

### Characters used for hints

```
nplseriaocm
```

</details>

<details>
<summary>Lesskey (Man Pages)</summary>

```
#command
\kl goto-line # left
\e[1;2B forw-scroll # shift down 
\e[1;2A back-scroll # shift up
\kr goto-end # right
h quit

```
</details>

Design
------

### Using Dead Keys for All Symbols

Cons:

- Each period, comma, and whatever else common symbol that you use will all 
take two keystrokes.
- Any behavior involving modifier key + symbol will not work through the dead 
key. 
- This is hard/impossible to setup on Windows and MacOS.
- If you need dead keys for international symbols, you'd be using the worse Compose key which
takes three keystrokes total. Although we do have three dead key types (grave,
tilde, acute) so hopefully you don't run out lol.
- High learning curve
- Some browsers don't respect .XCompose (Firefox LTS on Debian). It does work
in Brave though.

This is a tradeoff for:

- Significant increase in accuracy
- Fingers stay at/near homerow

Things to take note of:

- Vim keybinds and any application keybinds with symbols still work as long
as it's the symbol alone. This means typing ":" in vim through dead keys
still bring up the command line. In some websites however, this is finicky. 
(Discussed later)

Found inside `.XCompose`

### Number Layer

`Modifier_key + number` such as `Ctrl + 1` won't work with this layer. A workaround
would be to have a Colejak variant. A.k.a you switch layouts everytime you want to
use to `Modifier_key + number` and switch back. 

> [!NOTE]
> On Sway, you can add `Mod5` to your config. `Mod4 + 1` -> `Mod4 + Mod5 + 1`

Found inside `symbols/colejak`

### Navigation Row

We've now essentially brought the number row to the home row. This unlocks
a whole row for remapping. What better keys to map than to place navigation-related
keys here. Pros:

- Layout agnostic hjkl movemet + Home/End (no more getting confused with gg and G!).
- Arrow keys enable vim motions in **ALL** applications and textboxes.
- Vim motions in insert mode(??? for the lazy a.k.a me)

Especially within textboxes, here are available motions beyond hjkl:

- Word skipping - `Control+arrow`
- Start/End of a line - `Home` / `End`
- Start/End of textbox - `Control+Home` / `Control+End`
- Visual mode  - Hold shift while doing all the goodies above.

FAQ
---

### What are dead keys and Compose keys?

Dead Keys and the Compose Key are special "combo keys" that allow you to type 
a character by combining multiple key presses. Dead Keys are two-combo keys, 
commonly used for international symbols and diacritical marks, and come in types 
such as grave, tilde, and acute, which should not be confused with the standard 
grave and tilde keys. When pressed followed by a letter, they produce a special 
character, like `dead_tilde + n` outputting `ñ`. Unlike the Shift key, which modifies 
the next key press only when held down simultaneously, Dead Keys modify the next 
key press even after they've been released.

### How do I take full advantage of this layout?

- Use vim motions in your editor
- Install vimium in your browser
- Use a tiling window manager
- Install warpd

You'd reach for the mouse maybe 1-20 times a week. And if you're on a
laptop, the trackpad is within thumb's reach. Guaranteed productivity
gains.

### Recommended Practice Routine

I did it in 3 weeks just typing my brains out,
averaging 100 wpm after 50 hours of typing tests. Here's how:

- Monkeytype English until 50 wpm average.
- Keybr. Apply these settings: 
    - 60 wpm target speed. 
    - Unlock next key only when the previous keys are also above the target speed. 
    - Beat all letters.
- Monkeytype `English 5k` until 80 wpm average.
- PR 132 wpm on `Monkeytype English`.
- Diversify wordsets
    - Monkeytype `code rust` for numbers and symbols
    - Monkeytype `quotes`
    - Monkeytype `code javascript` for common programming words

### Does this apply to TTY as well?

No. And I don't recommend doing anything this complicated to your TTY
settings... ***here be dragons***

Motivation
----------

I don't have money for a $400 ergonomic keyboard. Also, maximizing ergonomics
within the constraint of an ANSI layout means that this is guaranteed to work and feel better
on an ortholinear, split, or whatever keyboard as is.

Resources
---------

[XKB Configuration Files Documentation](https://www.charvolant.org/doug/xkb/html/node5.html#SECTION00054000000000000000)

[Libxkbcommon commit: Allow for custom rulesets through include files](https://github.com/xkbcommon/libxkbcommon/pull/108/commits/bc4a691cb9f45c3309c78c997e00212f0978d082)

[Setting up $HOME/.config/xkb/](https://who-t.blogspot.com/2020/02/user-specific-xkb-configuration-part-1.html)

[Creating evdev.xml](https://who-t.blogspot.com/2020/07/user-specific-xkb-configuration-part-2.html)

[Extending system layouts with custom variants](https://who-t.blogspot.com/2020/08/user-specific-xkb-configuration-part-3.html)

[Youtube video where I discovered the blog posts from](https://www.youtube.com/watch?v=utqpa_8SXkA)

[This only works on Wayland and XWayland](https://who-t.blogspot.com/2020/09/no-user-specific-xkb-configuration-in-x.html)

[Where I discovered about XCompose and Dead Keys (DreymaR's Big Bag)](https://dreymar.colemak.org/)

Acknowledgement
---------------

### Keyboard Layout Pictures
- Created with [keyboard-layout-editor.com][keyboard-layout-editor]. 
- JSON for all layouts are available in [assets][assets].

### KCX Qwerty

- [ KCX Qwerty ][kcx-qwerty] is my old layout. Improved with modifier/special keys closer to the homerow.
This instead extends the default `symbols/us(basic)` layout which some may find useful.

[keyboard-layout-editor]: http://www.keyboard-layout-editor.com/
[assets]: https://github.com/james-orcales/dotfiles/tree/master/.config/xkb/assets
[keyboard-layout-analyzer]: https://stevep99.github.io/keyboard-layout-analyzer/#/main
[kcx-qwerty]: https://github.com/james-orcales/kcx-qwerty
