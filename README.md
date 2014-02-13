# uniq
##Simplified text the editor  簡易テキストモディファイア

ローカルで uniq sort reverse sort , splitしたかった自分用

###uniq

    $ cat hoge | uniq 

っぽいことができる  

![uniq](https://raw.github.com/katakk/uniq/master/doc/uniq.png)

![uniq](https://raw.github.com/katakk/uniq/master/doc/uniqa.png)

####単語ごとにuniq


![uinqw](https://raw.github.com/katakk/uniq/master/doc/uinqwa.png)

###ずれてるの整形したりできる  

sprintf %-50s %-50s %-50s っぽいかんじ  

![format](https://raw.github.com/katakk/uniq/master/doc/format.png)

![format](https://raw.github.com/katakk/uniq/master/doc/formata.png)

####ソートできる　　


論理ソート(StrCmpLogicalW API使う奴)と物理ソート(ただのqsort)があります　　

![sort](https://raw.github.com/katakk/uniq/master/doc/sort.png)


####ログ監視とか

    $ tail -f log 風味

![sort](https://raw.github.com/katakk/uniq/master/doc/tailf.png)
