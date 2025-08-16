- [Colejak ](#colejak)
   * [Software-Only Remap](#software-only-remap)
      + [Linux (XKB)](#linux-xkb)
      + [MacOS (Karabiner-Elements)](#macos-karabiner-elements)
      + [Windows](#windows)
   * [QMK](#qmk)
   * [ZMK](#zmk)
   * [Application Remaps (Optional)](#application-remaps-optional)
      + [Neovim](#neovim)
      + [Alacritty](#alacritty)
      + [Vimium](#vimium)
      + [Lesskey](#lesskey)
   * [Design](#design)
      + [Symbol Layer](#symbol-layer)
      + [Number Layer](#number-layer)
      + [Navigation Row](#navigation-row)
   * [Recommended Practice Routine](#recommended-practice-routine)
   * [FAQ](#faq)
      + [How do I take full advantage of this layout?](#how-do-i-take-full-advantage-of-this-layout)
      + [Does this apply to TTY as well?](#does-this-apply-to-tty-as-well)
      + [What are dead keys and Compose keys?](#what-are-dead-keys-and-compose-keys)
      + [Creating a New Layout From Scratch](#creating-a-new-layout-from-scratch)
      + [Thumb Typing Experiment](#thumb-typing-experiment)
      + [Colemak vs Colemak-DH](#colemak-vs-colemak-dh)
   * [Motivation](#motivation)
   * [Resources](#resources)
      + [Keyboard Layout Pictures](#keyboard-layout-pictures)
      + [KCX Qwerty](#kcx-qwerty)

# Colejak 

Colemak-DH variant optimized for programming and keyboard-centric navigation. Compatible with ANSI, Lily58, and ZSA-Voyager. Colejak repositions symbols and
numbers to accessible layers near the home row. This frees the top row for navigation and editing keys, placing arrows, Home, End, and Delete closer to the home
position for efficiency across applications.

![ANSI](https://github.com/user-attachments/assets/b877417e-3e25-44c8-a2e8-203e04c417ab)

![ZSA Voyager](https://github.com/user-attachments/assets/aba9c2d7-a504-440b-9aa2-100a2462bf2a)

<img width="1761" height="670" alt="lily58" src="https://github.com/user-attachments/assets/194eeb6d-f7c0-4bc3-a02e-0d33b5bce1db" />

## Software-Only Remap

### Linux (XKB)

> [!NOTE]
> These will overwrite existing destination files. Your original files such as *evdev.xml*
will be saved as *evdev.xml.bak*

```
git clone --filter=blob:none --depth=1 https://github.com/james-orcales/colejak
cd colejak
mkdir --parents            $HOME/.config/xkb/rules
mkdir --parents            $HOME/.config/xkb/symbols
cp --suffix=.bak rules/*   $HOME/.config/xkb/rules/
cp --suffix=.bak symbols/* $HOME/.config/xkb/symbols/
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

### MacOS (Karabiner-Elements)

TODO

### Windows

TODO


## QMK

Assumes RP2040 microcontroller.

```sh
#!/usr/bin/env dash

assert() {
        eval "$*" > /dev/null || { echo "ASSERTION FAILED: $*"; exit 1; }
}

assert test "$(basename "$(pwd)")" = "colejak" 
assert test -d ./.git 
assert command -v curl 
assert command -v git  

qmk_version="1.1.8"
if ! command -v qmk > /dev/null; then
        # Latest versions as of July 18, 2025
        if ! command -v uv > /dev/null; then
                curl --fail --silent --show-error --location https://astral.sh/uv/0.7.22/install.sh | UV_NO_MODIFY_PATH=1 sh || exit 1
                assert command -v uv
        fi
        uv tool install qmk=="$qmk_version" || exit 1
        assert command -v qmk
fi

# This does the same thing as `qmk clone <repo>` which is part of `qmk setup` but doesnt fetch the whole repo history.
export QMK_HOME="$HOME/.config/qmk"
if ! test -d "$QMK_HOME"; then
        echo "qmk repository is not yet installed"
        # Ideally, this should be pinned to a commit sha but recursive submodule cloning works best if everything is pointing at the branch tip
        # https://stackoverflow.com/questions/2144406/how-to-make-shallow-git-submodules
        git clone --depth=1 --recurse-submodules --shallow-submodules https://github.com/qmk/qmk_firmware "$QMK_HOME"
        cd "$QMK_HOME"
        git remote rename origin upstream
        cd -
fi

if ! test -d "$QMK_HOME/keyboards/lily58/keymaps/colejak/"; then
        assert test -d ./qmk/colejak 
        cp -R ./qmk/colejak "$QMK_HOME/keyboards/lily58/keymaps/"
fi

qmk setup || exit 1
qmk config user.keyboard=lily58/rev1 || exit 1
qmk config user.keymap=colejak || exit 1


assert test "$(qmk --version)" = "$qmk_version"
qmk compile || exit 1
assert test -f "$QMK_HOME/lily58_rev1_colejak.uf2" 

echo
echo "Instructions from James:"
echo "     At this point, enter bootloader mode while keyboard is plugged into your computer"
echo "     A new removable storage device will appear. This is your keyboard in bootloader mode."
echo "     Simply copy the file into the storage device and the firmware is automatically flashed."
echo "     Refer to https://docs.qmk.fm/flashing#raspberry-pi-rp2040-uf2"
echo "Opening file explorer"
open "$QMK_HOME"
```

## ZMK

TODO


## Application Remaps (Optional)

<details>
<summary>Neovim</summary>

### Neovim

```lua
local override = function(modes, new, default, desc, custom_behavior)
    local behavior = default
    if custom_behavior then
        behavior = custom_behavior
    end

    vim.keymap.set(modes, default, "<nop>")
    vim.keymap.set(modes, new, behavior, { desc = desc })
end

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

https://github.com/james-orcales/init.lua

</details>

<details>
<summary>Alacritty</summary>

### Alacritty

```toml
[keyboard]
bindings = [
    { key = "Enter",    mods = "Shift",           chars = "\u001B[13;2u"      },
    { key = "Enter",    mods = "Control",         chars = "\u001B[13;5u"      },
    { key = "Delete",                             chars = "\u001B[3~"         },
    { key = "Delete",   mods = "Shift",           chars = "\u001B[3;2u"       },

    { key = "Home",     mode = "AppCursor",       chars = "\u001BOH"          },
    { key = "Home",     mode = "~AppCursor",      chars = "\u001B[H"          },
    { key = "End",      mode = "AppCursor",       chars = "\u001BOF"          },
    { key = "End",      mode = "~AppCursor",      chars = "\u001B[F"          },

    { key = "Left",     mods = "Shift",           chars = "\u001B[1;2D"       },
    { key = "Left",     mods = "Control",         chars = "\u001B[1;5D"       },
    { key = "Left",     mods = "Alt",             chars = "\u001B[1;3D"       },
    { key = "Left",     mode = "~AppCursor",      chars = "\u001B[D"          },
    { key = "Left",     mode = "AppCursor",       chars = "\u001BOD"          },

    { key = "Right",    mods = "Shift",           chars = "\u001B[1;2C"       },
    { key = "Right",    mods = "Control",         chars = "\u001B[1;5C"       },
    { key = "Right",    mods = "Alt",             chars = "\u001B[1;3C"       },
    { key = "Right",    mode = "~AppCursor",      chars = "\u001B[C"          },
    { key = "Right",    mode = "AppCursor",       chars = "\u001BOC"          },

    { key = "Up",       mods = "Shift",           chars = "\u001B[1;2A"       },
    { key = "Up",       mods = "Control",         chars = "\u001B[1;5A"       },
    { key = "Up",       mods = "Alt",             chars = "\u001B[1;3A"       },
    { key = "Up",       mode = "~AppCursor",      chars = "\u001B[A"          },
    { key = "Up",       mode = "AppCursor",       chars = "\u001BOA"          },

    { key = "Down",     mods = "Shift",           chars = "\u001B[1;2B"       },
    { key = "Down",     mods = "Control",         chars = "\u001B[1;5B"       },
    { key = "Down",     mods = "Alt",             chars = "\u001B[1;3B"       },
    { key = "Down",     mode = "~AppCursor",      chars = "\u001B[B"          },
    { key = "Down",     mode = "AppCursor",       chars = "\u001BOB"          },

    { key = "Tab",      mods = "Shift",           chars = "\u001B[Z"          },
    { key = "Back",     mods = "Alt",             chars = "\u001B\u007F"      },

    { key = "RBracket", mods = "Shift",           chars = "\u0002n"           },
    { key = "LBracket", mods = "Shift",           chars = "\u0002p"           },

    { key = "V",        mods = "Control | Shift", action = "Paste"            },
    { key = "C",        mods = "Control | Shift", action = "Copy"             },

    { key = "Equals",   mods = "Control",         action = "IncreaseFontSize" },
    { key = "Minus",    mods = "Control",         action = "DecreaseFontSize" },
    { key = "Minus",    mods = "Control|Shift",   action = "ResetFontSize"    },
]


```
</details>

<details>
<summary>Vimium</summary>

### Vimium

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

Characters used for hints: `nseriaoplfuwyqjdcxz`

</details>

<details>
<summary>Lesskey (Man Pages)</summary>

### Lesskey

```
#command
\kl goto-line # left
\e[1;2B forw-scroll # shift down 
\e[1;2A back-scroll # shift up
\kr goto-end # right
h quit

```
</details>

## Design

### Symbol Layer

There's 32 symbols on the keyboard. I work with a lot of different languages so I use all of these symbols on a reqular basis. That's why it's important that I
keep them all close to the homerow to not only maintain speed, but most importantly, maintain accuracy. My symbol layout is optimized for the languages that I
commonly use, I encourage you to personalize this layer to fit your on workflow as well. These things affected my symbol layout the most:

- Neovim
- Odin
- Golang
- Bash
- YAML (especially Github Actions)

### Number Layer

Same concept with the symbol layer here. You acces this layer by combining the Symbol Layer and Shift to get to Layer 4. You will notice however the sequence of
the numbers `4321 8065` (`97` above `20` respectively). I tried to put the most common numbers in programming, based on common expressions and data types, on
the strongest fingers while balancing an easy-to-memorize layout. If you want a hyperoptimized sequence, try this `7612 3045 (98)`. Again, I encourage you to
personalize this layer based on your own usage patterns.

`Modifier_key + number` such as `Ctrl + 1` won't work with this layer. A workaround would be to have a Colejak variant where the numbers are on the first layer.
A.k.a you switch layouts everytime you want to use to `Modifier_key + number` and switch back. 

> [!NOTE]
> On Sway, you can add `Mod5` to your config. `Mod4 + 1` -> `Mod4 + Mod5 + 1`

Found inside `symbols/colejak`

### Navigation Row

We've now essentially brought the number row to the home row. This unlocks a whole row for remapping. What better keys to map than to place navigation-related
keys here. Pros:

- Layout agnostic hjkl movemet + Home/End (no more getting confused with gg and G!).
- Arrow keys enable vim motions in **ALL** applications and textboxes.
- Vim motions in insert mode(??? for the lazy a.k.a me)

Especially within textboxes, here are available motions beyond hjkl:

- Word skipping - `Control+arrow`
- Start/End of a line - `Home` / `End`
- Start/End of textbox - `Control+Home` / `Control+End`
- Visual mode  - Hold shift while doing all the goodies above.

## Recommended Practice Routine

Here's how I did it in 3 weeks, averaging 100 wpm after 50 hours of typing tests. Who knows how optimal this is, I'm simply showing the path I took:

1) Monkeytype English until 50 wpm average.
2) Keybr. Apply these settings: 
    - 60 wpm target speed. 
    - Unlock next key only when the previous keys are also above the target speed. 
    - Beat all letters.
3) Monkeytype `English 5k` until 80 wpm average.
4) PR 132 wpm on `Monkeytype English`.
5) Diversify wordsets
    - Monkeytype `code rust` for numbers and symbols
    - Monkeytype `quotes`
    - Monkeytype `code javascript` and `code odin` for common programming words

I recommend that you stop training once you can consistently hit 100-120 WPM. Personally, I always regress to this speed when I stop typing tests and focus on
programming again.

## FAQ

### How do I take full advantage of this layout?

- Use a PDE such as Neovim or Emacs.
- Install vimium in your browser
- Use a tiling window manager

### Does this apply to TTY as well?

No. And I don't recommend doing anything this complicated to your TTY settings... ***here be dragons***


### What are dead keys and Compose keys?

Dead Keys and the Compose Key are special "combo keys" that allow you to type a character by combining multiple key presses. Dead Keys are two-combo keys,
commonly used for international symbols and diacritical marks, and come in types such as grave, tilde, and acute, which should not be confused with the standard
grave and tilde keys. When pressed followed by a letter, they produce a special character, like `dead_tilde + n` outputting `ñ`. Unlike the Shift key, which
modifies the next key press only when held down simultaneously, Dead Keys modify the next key press even after they've been released.

I experimented with using Dead Keys before settling on my current symbol and number layer approach. In practice, they slowed down typing and, more importantly,
added cognitive load compared to layered key mappings. Another drawback was the need to dedicate 1-2 keys for the dead key/compose triggers—an unacceptable
trade-off when pursuing compatibility with a smaller keyboard like the ZSA Voyager.

### Creating a New Layout From Scratch

I spent a good 3 months of my life obsessing over creating a new keyboard layout from scratch. The goals were to make a Dvorak-Colemak baby by maximizing finger
alternation and minimizing same-finger bigrams. I concluded that hyperoptimizing for typing is not worth it as one must also be concerned with keyboard shortcut
ergnomics in which Colemak-DH is the best balance of these metrics.

### Thumb Typing Experiment

I've tried designing layouts that place letters on the thumbs, including using the thumbs to type the lower-center letters of the Colemak-DH layout. In my
experience, this approach actually slows down typing. It only proves effective with the QWERTY layout, where I used this exact method to reach 152 wpm.

### Colemak vs Colemak-DH

The difference is marginal except for a specific use case. On Colemak, typing `pub` or `public` is objectively inferior to Colemak-DH because of the huge jump
that the left index finger has to make between `p` and `b`.

## Motivation

I don't have money for a $400 ergonomic keyboard. Also, maximizing ergonomics within the constraint of an ANSI layout means that this is guaranteed to work and
feel better on an ortholinear, split, or whatever keyboard as is.

## Resources

[XKB Configuration Files Documentation](https://www.charvolant.org/doug/xkb/html/node5.html#SECTION00054000000000000000)

[Libxkbcommon commit: Allow for custom rulesets through include files](https://github.com/xkbcommon/libxkbcommon/pull/108/commits/bc4a691cb9f45c3309c78c997e00212f0978d082)

[Setting up $HOME/.config/xkb/](https://who-t.blogspot.com/2020/02/user-specific-xkb-configuration-part-1.html)

[Creating evdev.xml](https://who-t.blogspot.com/2020/07/user-specific-xkb-configuration-part-2.html)

[Extending system layouts with custom variants](https://who-t.blogspot.com/2020/08/user-specific-xkb-configuration-part-3.html)

[Youtube video where I discovered the blog posts from](https://www.youtube.com/watch?v=utqpa_8SXkA)

[This only works on Wayland and XWayland](https://who-t.blogspot.com/2020/09/no-user-specific-xkb-configuration-in-x.html)

[Where I discovered about XCompose and Dead Keys (DreymaR's Big Bag)](https://dreymar.colemak.org/)

## Acknowledgement

### Keyboard Layout Pictures
- Created with [keyboard-layout-editor.com][keyboard-layout-editor]. [(GitHub)][keyboard-layout-editor-github]
- JSON for all layouts are available in [assets][assets].

### KCX Qwerty

- [ KCX Qwerty ][kcx-qwerty] is my old layout. Improved with modifier/special keys closer to the homerow. This instead
  extends the default `symbols/us(basic)` layout which some may find useful.

[keyboard-layout-editor]: http://www.keyboard-layout-editor.com/
[keyboard-layout-editor-github]: https://github.com/ijprest/keyboard-layout-editor
[assets]: https://github.com/james-orcales/dotfiles/tree/master/.config/xkb/assets
[keyboard-layout-analyzer]: https://stevep99.github.io/keyboard-layout-analyzer/#/main
[kcx-qwerty]: https://github.com/james-orcales/kcx-qwerty
