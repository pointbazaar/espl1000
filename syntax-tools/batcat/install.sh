#https://github.com/sharkdp/bat

#process is similar to sublime text
#they accept sublime text packages

mkdir -p "$(batcat --config-dir)/syntaxes"
cd "$(batcat --config-dir)/syntaxes" || exit 1

# Put new '.sublime-syntax' language definition files
# in this folder (or its subdirectories), for example:
ln -s ~/smalldragon/syntax-tools/sublime-text/dragon.sublime-syntax .

batcat cache --build

#to remove it:
#batcat cache --clear
