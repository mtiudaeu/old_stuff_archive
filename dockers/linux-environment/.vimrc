" ---------- NEEDED?? ----------
set wildmode=longest,list,full
set wildmenu
" ---------- NEEDED?? ----------

" ---------- CONFIG ----------

set number
set tabstop=1
set shiftwidth=1
set expandtab
set incsearch
set hlsearch
set clipboard+=unnamed
syntax on

" Filename as status line.
set statusline=%F
set laststatus=2

" ---------- HOTKEYS ----------

" Switch between header and source file.
" map <F4> :e %:p:s,.h$,.X123X,:s,.C$,.h,:s,.X123X$,.C,<CR>

map <F7> :let handle = ch_open('localhost:8765')<CR>
map <F8> O//MDTMP
map <F12> :qa<CR>
map <C-e> :tabe

" ---------- TAG ----------
" set tags=../../tags
" Tag on a new Tab
" map <C-\> :tab split<CR>:exec("tag ".expand("<cword>"))<CR>

" ---------- CLANG-FORMAT ----------
map <C-K> :!/usr/bin/clang-format-3.5 -style=Google -assume-filename=@%<cr>
