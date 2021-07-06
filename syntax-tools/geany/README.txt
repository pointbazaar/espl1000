#https://wiki.geany.org/config/start
#how to achieve smalldragon syntax highlight in geany

#step 1:

cd ~/.config/geany/filedefs
ln -s ~/smalldragon/syntax-tools/geany/filetypes.smalldragon.conf 

#step 2:
In Geany:
go to Tools -> Configuration Files -> filetype_extensions.conf

put 
  #~ smalldragon=*.dg;

directly under 
  #~ [Extensions]

#step 3: 
#if the step 2 did not give you syntax highlight after
#restart, you can also edit global config files
#and perform step 2 that way 

sudo vim/usr/share/geany/filetype_extensions.conf
